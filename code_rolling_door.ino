#include <BlynkSimpleEsp32.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
WebServer webServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

#include <SPI.h>
#include <MFRC522.h>   
#define SS_PIN  5 
#define RST_PIN 4 
MFRC522 mfrc522(SS_PIN, RST_PIN);
#define buzzer 16
String idcard;
String namecard;
int setupMode=0;

#include "loginPage.h"
#include "setupPage.h"
#include "dataPage.h"
#include "homePage.h"
String ssid_ap,pass_ap;
String user_login,pass_login;
String ssid_sta,pass_sta,auth_token;
const byte btSetup=15;
const byte ledSignal=2;
boolean btSetupState=HIGH;
unsigned long timesConfig=millis();
boolean writeFile(fs::FS &fs, const char * path, String json);
String readFile(fs::FS &fs, const char * path);
boolean wifiMode=0;
WidgetLED LEDCONNECT(V0);
#define BTOPEN      V1
#define BTCLOSE     V2
#define BTSTOP      V3
#define DOORSTATE   V4
#define IPCONNECT   V5
//---------------STEPPER-----------------
#include <AccelStepper.h>
const int btOpen=14;
const int btClose=27;
const int btStops=26;
const int swStop1=13;  //Công tắc hành trình ở trên
const int swStop2=12;  //Công tắc hành trình ở dưới
const int en=32; 
const int dir=33;
const int pul=25; 
AccelStepper stepper(1, pul, dir);
boolean runState=0;
int numStep=400;      //Số bước/1vong (điều khiển vi bước)
int speeds=1;          //Tốc độ vòng/giây
const int speedsMax=10; //Tốc độ max vòng/giây
long pos;              //Vị trí sẽ quay đến
int disMax=10*numStep; //Vị trí tối đa sẽ quay đến 10 vòng
int dirStep=1;         //Hướng quay -1:open 1:close
unsigned long timesReadA0=millis(); 
unsigned long timesDelay=millis();
unsigned long timesUpdate=millis();
boolean btOpenState=HIGH;
boolean btCloseState=HIGH;
boolean btStopState=HIGH;


void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  SPI.begin(); // init SPI bus
  mfrc522.PCD_Init(); // init MFRC522
  if(!SPIFFS.begin()){
      Serial.println("SPIFFS Mount Failed");
      return;
  }
  pinMode(btSetup,INPUT_PULLUP);
  pinMode(ledSignal,OUTPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,LOW);
  readConfig();
  setupWifi();
  if(wifiMode==1){ //Nếu đã kết nối được wifi thì kết nối blynk
    Blynk.config(auth_token.c_str(),"blynk.cloud", 80);
    Blynk.connect();
  }
  setupWebServer();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  //----------------------------------------
  pinMode(btOpen,INPUT_PULLUP);
  pinMode(btClose,INPUT_PULLUP);
  pinMode(btStops,INPUT_PULLUP);
  pinMode(swStop1,INPUT_PULLUP);
  pinMode(swStop2,INPUT_PULLUP);
  stepper.setMaxSpeed(numStep*100); //Số vòng/1giay
  int adc = analogRead(A0);
  speeds = map(adc,0,1023,1,speedsMax); //Cài tốc độ động cơ bước qua chân A0
  move_stepper(disMax,1);
  attachInterrupt(btStops,handlebtStops,FALLING);
  Serial.println("----------------------Init-------------------");
}

void loop() {
  if(wifiMode==1){
    Blynk.run();
  }
  webServer.handleClient();
  webSocket.loop();
  checkButtonReset();

  if(runState==1){
    if(millis()-timesReadA0>500){
      int adc = analogRead(A0);
      speeds = map(adc,0,1023,1,speedsMax);//Lấy tốc độ dựa vào chiếc ap tại A0
      timesReadA0=millis();
    }
    stepper.setSpeed(numStep*speeds);
    stepper.runSpeedToPosition();
    if(stepper.currentPosition()==pos){
      Serial.println();
      Serial.println("Posision:"+ String(stepper.currentPosition()));
      stop_stepper();
    }else{
      if(dirStep==1){  //Nếu đang đóng
        if(digitalRead(swStop2)==LOW){ //Kiểm tra công tắc hành trình 2
          if(runState==1){
            stop_stepper();
          }
        }
      }else{           //Nếu đang mở
        if(digitalRead(swStop1)==LOW){//Kiểm tra công tắc hành trình 1
          if(runState==1){
            stop_stepper();
          }
        }
      }
    }
  }else{
    //Cập nhật trạng thái cửa và ip lên blynk
    if(wifiMode==1){
      if(millis()-timesUpdate>1000){
        if (LEDCONNECT.getValue()) {
          LEDCONNECT.off();
        } else {
          LEDCONNECT.on();
        }
        String state;
        if(digitalRead(swStop2)==LOW){
          state="CỬA ĐANG ĐÓNG!";
        }else if(digitalRead(swStop2)==HIGH){
          state="CỬA ĐANG MỞ!";
        }
        Blynk.virtualWrite(DOORSTATE,state);
        Blynk.virtualWrite(IPCONNECT,WiFi.localIP().toString());
        timesUpdate=millis();
      }
    }
    //Chờ đọc thẻ RFID
    readUID();
  }
  checkButton();
}
void beep(int t){
  digitalWrite(buzzer,HIGH);
  delay(t);
  digitalWrite(buzzer,LOW);
}
void checkButtonReset(){
  if(digitalRead(btSetup)==LOW){
    if(btSetupState==HIGH){
      timesConfig=millis();
      Serial.println("Ấn giữ 15 giây để reset về mặc định!");
      btSetupState=LOW;
    }else{
      digitalWrite(ledSignal,!digitalRead(ledSignal));
      delay(500);
    }
    if(millis()-timesConfig>15000){
      for(int i=0;i<20;++i){
        digitalWrite(ledSignal,!digitalRead(ledSignal));
        delay(100);
      }
      DynamicJsonDocument doc(512);
      doc["ssid_ap"]="SMART DOOR";
      doc["pass_ap"]="12345678";
      doc["user_login"]="admin";
      doc["pass_login"]="admin";
      doc["ssid_sta"]="";
      doc["pass_sta"]="";
      doc["auth_token"]="";
      String str="";
      serializeJson(doc, str);
      writeString(0,512,str);
      EEPROM.commit();
      Serial.println("Đã reset về trạng thái mặc định!");
      delay(2000);
      ESP.restart();
    }
  }else{
    btSetupState=HIGH;
    if(wifiMode==0){
      if(millis()-timesConfig>200){
        digitalWrite(ledSignal,!digitalRead(ledSignal));
        timesConfig=millis();
      }
    }else if(wifiMode==1){
      if(millis()-timesConfig>2000){
        digitalWrite(ledSignal,!digitalRead(ledSignal));
        timesConfig=millis();
      }
    }
  }
}
void readConfig(){
  Serial.println();
  Serial.println("=============================================");
  Serial.println("*          Đọc thông tin cấu hình!          *");
  Serial.println("=============================================");
  String str=readString(0,512); //max343
  DynamicJsonDocument doc(512);
  deserializeJson(doc, str);
  //serializeJsonPretty(doc, Serial);//serializeJson(doc, Serial);
  JsonObject obj = doc.as<JsonObject>();
  ssid_ap = obj["ssid_ap"].as<String>();
  pass_ap = obj["pass_ap"].as<String>();
  user_login = obj["user_login"].as<String>();
  pass_login = obj["pass_login"].as<String>();
  ssid_sta = obj["ssid_sta"].as<String>();
  pass_sta = obj["pass_sta"].as<String>();
  auth_token = obj["auth_token"].as<String>();
  Serial.println("----------------Chế độ phát wifi-------------");
  Serial.println("Tên wifi: " + ssid_ap);
  Serial.println("Mật khẩu: " + pass_ap);
  Serial.println("--------------Chế độ kết nối wifi------------");
  Serial.println("Tên wifi: " + ssid_sta);
  Serial.println("Mật khẩu: " + pass_sta);
  Serial.println("Mã blynk: " + auth_token);
  Serial.println("--------------Thông tin webserver------------");
  Serial.println("Tên đăng nhập: " + user_login);
  Serial.println("Mật khẩu: " + pass_login);
}
void writeConfig(){
  DynamicJsonDocument doc(512);
  doc["ssid_ap"]=ssid_ap;
  doc["pass_ap"]=pass_ap;
  doc["user_login"]=user_login;
  doc["pass_login"]=pass_login;
  doc["ssid_sta"]=ssid_sta;
  doc["pass_sta"]=pass_sta;
  doc["auth_token"]=auth_token;
  String str="";
  serializeJson(doc, str);
  writeString(0,512,str);
  EEPROM.commit();
  Serial.println("Đã lưu cấu hình thành công!");
}
void setupWifi(){
  Serial.println("=============================================");
  Serial.println("*          Thiết lập chế độ wifi!           *");
  Serial.println("=============================================");
  if(ssid_sta!=""){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_sta.c_str(),pass_sta.c_str());
    Serial.println("Đang kết nối wifi: " + ssid_sta);
    int n=0;
    while(n<20){
      if(WiFi.status()==WL_CONNECTED){
        Serial.println("\nĐã kết nối đến wifi!");
        Serial.print("Ip kết nối: ");Serial.println(WiFi.localIP());
        wifiMode=1;
        break;
      }
      n++;
      digitalWrite(ledSignal,!digitalRead(ledSignal));
      delay(500);
      Serial.print(".");
    }
  }
  if(wifiMode==0){
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid_ap.c_str(),pass_ap.c_str(), 1, false,4);
    Serial.println("\nĐã phát ra wifi mới!");
    Serial.println("Tên wifi: " + String(ssid_ap) +"\nMật khẩu:" + pass_ap);
    Serial.println("Ip webserver: " + WiFi.softAPIP().toString());
  }
}
void setupWebServer(){
  webServer.on("/",[]{
    setupMode=0;
    String s = FPSTR(homePage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/home",[]{
    setupMode=0;
    String s = FPSTR(homePage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/login",[]{
    String s = FPSTR(loginPage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/setup",[]{
    String s = FPSTR(setupPage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/data",[]{
    String s = FPSTR(dataPage);
    webServer.send(200,"text/html",s);
  });
  webServer.on("/checkLogin",checkLogin);
  webServer.on("/saveLogin",saveLogin);
  webServer.on("/getConfig",getConfig);
  webServer.on("/getWifiList",getWifiList);
  webServer.on("/saveConfig",saveConfig);
  webServer.on("/getDataStore",getDataStore);
  webServer.on("/addDataStore",addDataStore);
  webServer.on("/deleteDataStore",deleteDataStore);
  webServer.on("/reStart",[]{ESP.restart();});
  webServer.onNotFound([]{
    String s = FPSTR(homePage);
    webServer.send(200,"text/html",s);
  });
  webServer.begin();
  Serial.println("---------------------------------------------");
  Serial.println("Đã khởi tạo Web server!");
}
void checkLogin(){
  String user_lg = webServer.arg("user_login");
  String pass_lg = webServer.arg("pass_login");
  String s;
  if(user_lg==user_login && pass_lg==pass_login){
    s="OK";
  }else{
    s="Tên đăng nhập hoặc mật khẩu không đúng!";
  }
  webServer.send(200,"text/plain",s);
}
void saveLogin(){
  user_login = webServer.arg("user_login");
  pass_login = webServer.arg("pass_login");
  writeConfig();
  String s = "Đã lưu thông tin đăng nhập!";
  webServer.send(200,"text/plain",s);
}
void getConfig(){
  String str = "";
  str = readString(0,512);
  webServer.send(200,"application/json",str);  
  //Serial.println(str);
}
void getWifiList(){
  int n = WiFi.scanNetworks();
  delay(100);
  String ssidList = "[\""; 
  for(int i=0;i<n;++i){
    ssidList+= WiFi.SSID(i)+ "\""; //{"keyword":["wifi1","wifi2",...]}
    if(i<(n-1)){
      ssidList += ",\"";
    }
  }
  ssidList += "]";
  String s = "{\"ssidList\": "+ ssidList + "}";
  webServer.send(200,"application/json",s);
  //Serial.println(s);
}
void saveConfig(){
  String str = webServer.arg("plain");
  //Serial.println(str);
  writeString(0,512, str);
  EEPROM.commit();
  String s = "Cấu hình đã được lưu thành công. Vui lòng khởi động lại thiết bị!";
  webServer.send(200,"text/html",s);
}
void reStart(){
  ESP.restart();
}
void writeString(int n, int m, String str){
  if(str.length()<=(m-n)){
    for(int i=n; i<m;++i){
      EEPROM.write(i,0);
      delay(1);
    }
    for(int i=0; i<str.length();++i){
      EEPROM.write(n+i,str[i]);
    }
  }else{
    Serial.println("Dữ liệu vược quá vùng nhớ thiết lập!");
  }
}
String readString(int n, int m){
  String str="";
  for(int i=n;i<m;++i){
    str += char(EEPROM.read(i));
    delay(1);
  }
  str = str.c_str();
  return str;
}
void getDataStore(){
  //Serial.println("Get data store!");
  setupMode=1;
  String str = readFile(SPIFFS,"/dataStore.json");
  webServer.send(200,"application/json",str);  
  //Serial.println(str);
}
void addDataStore(){
  //Serial.println("Add data store!");
  String idcard = webServer.arg("idcard");
  String namecard = webServer.arg("namecard");
  String str = readFile(SPIFFS,"/dataStore.json");
  //Serial.println("Data old: " +str);
  DynamicJsonDocument doc(2024);
  deserializeJson(doc, str);
  doc[idcard]=namecard;
  str="";
  serializeJson(doc, str);
  //Serial.println("Data new: " +str);
  writeFile(SPIFFS,"/dataStore.json",str);
  String s="Đã thêm dữ liệu mới!";
  webServer.send(200,"text/html",s);
  //Serial.println(str);
}
void deleteDataStore(){
  Serial.println("Delete data store!");
  String idcard = webServer.arg("idcard");
  String str = readFile(SPIFFS,"/dataStore.json");
  DynamicJsonDocument doc(2024);
  deserializeJson(doc, str);
  doc.remove(idcard);
  str="";
  serializeJson(doc, str);
  writeFile(SPIFFS,"/dataStore.json",str);
  String s="Đã xóa dữ liệu!";
  webServer.send(200,"text/html",s);
  //Serial.println(str);
}
void webSocketEvent(uint8_t num, WStype_t type,uint8_t * payload,size_t length){
  String payloadString = (const char *)payload;
  Serial.print("payloadString= ");
  Serial.println(payloadString);
  if(payloadString=="setupStoreOn"){
    setupMode=1;
  }else if(payloadString=="setupStoreOff"){
    setupMode=0;
  }
  if(payloadString=="open"){
    Serial.println("Mở cửa!");
    move_stepper(disMax, -1);
  }else if(payloadString=="close"){
    Serial.println("Đóng cửa!");
    move_stepper(disMax, 1);
  }else if(payloadString=="stop"){
    Serial.println("Dừng cửa!");
    runState=!runState;
    Serial.println("Run state: "+String(runState));
  }
  Serial.print("Setup Mode: ");Serial.println(setupMode);
}
void sendidcard(){
  String JSONtxt = "{\"idcard\": \""+ idcard +"\"," +
                    "\"namecard\": \""+ namecard +"\"}";
  webSocket.broadcastTXT(JSONtxt);
  //Serial.println(JSONtxt);
}
String readFile(fs::FS &fs, const char * path){
  File file = fs.open(path);
  if(!file|| file.isDirectory()){
    Serial.println("File not found!");
    return "null";
  }
  String buf;
  while(file.available()){
    buf += (char)file.read();
  }
  file.close();
  return buf;
}
boolean writeFile(fs::FS &fs, const char * path, String json){
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return false;
  }
  if (file.print(json)) {
    //Serial.println("File written");
    delay(100);
    file.close();
    return true;
  } else {
    Serial.println("Write failed");
    return false;
  }
}
void readUID(){
  unsigned long uidDec, uidDecTemp;  
  // Tim the moi
  if ( ! mfrc522.PICC_IsNewCardPresent()) return;
  // Doc the
  if ( ! mfrc522.PICC_ReadCardSerial()) return;
  uidDec = 0;
  // Hien thi so UID cua the
  for (byte i = 0; i < mfrc522.uid.size; i++){
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec*256+uidDecTemp;
  } 
  Serial.println("Card UID: " + String(uidDec));
  //mfrc522.PICC_HaltA(); 
  idcard=String(uidDec);
  String str = readFile(SPIFFS,"/dataStore.json");
  DynamicJsonDocument doc(2024);
  deserializeJson(doc, str);
  JsonObject obj = doc.as<JsonObject>();
  namecard = obj[idcard].as<String>();
  if(namecard=="null") namecard="";
  Serial.println("Name card: " + namecard);
  sendidcard();
  if(setupMode==1){
    if(namecard==""){
      Serial.println("Thẻ chưa được lưu!");
      beep(500);
      delay(200);
      beep(500);
    }else{
      beep(200);
    }
  }else{
    if(namecard==""){
      Serial.println("Thẻ chưa được lưu!");
      beep(500);
      delay(200);
      beep(500);
    }else{
      beep(200);
      if(dirStep==1)move_stepper(disMax, -1);
      else move_stepper(disMax, 1);
      
    }
  }
}
//--------------------STEPPER-------------------
void checkButton(){
  if(digitalRead(btOpen)==LOW){
    if(btOpenState==HIGH){
      move_stepper(disMax, -1);
      btOpenState=LOW;
      delay(200);//Delay chống dội
    }
  }else{
    btOpenState=HIGH;
  }
  if(digitalRead(btClose)==LOW){
    if(btCloseState==HIGH){
      move_stepper(disMax, 1);
      btCloseState=LOW;
      delay(200);//Delay chống dội
    }
  }else{
    btCloseState=HIGH;
  }
}
void move_stepper(int dis, int dir){
  dirStep=dir;
  pos=(long)dirStep*(long)numStep*(long)dis;
  Serial.println("Move pos: " + String(pos));
  stepper.moveTo(pos);  //Xoay vòng đến vị trí thiết lập
  stepper.enableOutputs();
  digitalWrite(en,LOW);
  runState=1;
}
void stop_stepper(){
  Serial.println();
  Serial.println("Posision:"+ String(stepper.currentPosition()));
  stepper.disableOutputs();
  runState=0;
  digitalWrite(en,HIGH);
  stepper.setCurrentPosition(0);
  Serial.println();
  Serial.println("Posision:"+ String(stepper.currentPosition()));
}
ICACHE_RAM_ATTR void handlebtStops(){
  if(millis()-timesDelay>500){
    runState=!runState;
    Serial.println("Run State: " + String(runState));
    timesDelay=millis();
  }
}
BLYNK_WRITE(BTOPEN){
  int p=param.asInt();
  if(p==1){
    move_stepper(disMax, -1);
  }
}
BLYNK_WRITE(BTCLOSE){
  int p=param.asInt();
  if(p==1){
    move_stepper(disMax, 1);
  }
}
BLYNK_WRITE(BTSTOP){
  int p=param.asInt();
  if(p==1){
    runState=!runState;
    Serial.println("Run state: "+String(runState));
  }
}
