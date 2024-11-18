const char setupPage[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html>
<head>
  <title>SETUP WIFI</title>
  <style type="text/css">
    body{
      background: white;
    }
    #main{
      margin: 10px auto;
      width: 728px;
      color: white;
    }
    #content{
      width: 728px;
      float: left;
      border-radius: 5px;
      color: blue;
      margin-left: 80px;
    }
    h3{
      margin-top: 0px;
      margin-bottom: 0px;
    }
    p{
      margin-top: 10px;
      margin-bottom: 10px;
    }
    .div1{
      margin-left: 20px;
      float: left;
      margin-bottom: 10px;
    }
    .div1 div{
      margin-left: 20px;
    }
    .input{
      width: 300px;
      float: left;
    }
    input[type="checkbox"]{
      width: 20px;
      height: 20px;
      margin-top: 0px;
      float: left;
    }
    input[type="button"]{
      width: 120px;
      height: 50px;
      float: left;
      margin: 10px 5px;
      outline: none;
      text-align: center; 
      white-space: normal;
      font-weight: bold;
    }
  </style>
  <meta name="viewport" content="width=device-width,user-scalable=0" charset="utf-8">
</head>
<body>
  <div id="main">
    <div id="header" style="text-align: center;">
      <h2 style="color: blue">TRƯỜNG ĐH BÁCH KHOA ĐÀ NẴNG<br>
      LẬP TRÌNH ESP32</h2>
      <h1 style="color: red">PBL3</h1>
      <h3 style="color: orange">ĐỀ TÀI: CỬA CUỐN THÔNG MINH</h3>
      <p style="color: green; font-weight: bold;">GVHD: Hồ Viết Việt<br>
        SVTH: Nguyễn Quang Phúc Huy</p>
    </div>
     <hr  width="100%" align="center" />
    <div id="content">
      <div class="div1" style="width: 500px;margin-left: 85px;">
        <h3>CHẾ ĐỘ PHÁT WIFI</h3>
          <p>Tên wifi phát:</p>
          <input id="ssid_ap" readonly="true" class="input" type="text"><br>
          <p>Mật khẩu:</p>
          <input id="pass_ap" readonly="true" class="input" type="password">
          <input onclick="showpass('pass_ap')" type="checkbox">hiện
      </div>
      <div class="div1" style="width: 500px;margin-left: 85px;">
        <h3>CHẾ ĐỘ KẾT NỐI WIFI VÀ BLYNK</h3>
          <p>Tên wifi kết nối:</p>
          <input id="ssid_sta" readonly="true" class="input" type="text"><br><br>
          <select id="ssidList" readonly="true" onchange="updatessid_sta()">
            <option>Chọn mạng wifi</option>
          </select>
          <p>Mật khẩu:</p>
          <input id="pass_sta" readonly="true" class="input" type="password">
          <input onclick="showpass('pass_sta')" type="checkbox">Hiện<br>
          <p>Mã auth token blynk:</p>
          <input id="auth_token" readonly="true" class="input" type="text">
      </div>
      <div class="div1" style="margin-left: 80px;">
        <input style="background: green; color: white;border-color: orange;" id="edit" type="button" onclick="getWifiList()" value="Sửa">
        <input style="background: green; color: white;border-color: orange;display: none;" id="save" type="button" onclick="saveConfig()" value="Lưu">
        <input style="background: yellow; color: green;border-color: orange;" type="button" onclick="window.location.href='/home'" value="Thoát">
        <input style="background: red; color: white;border-color: orange;" type="button" onclick="reStart()" value="Restart">
      </div>
  </div>
  <script type="text/javascript">
    window.onload = function(){
      if(sessionStorage.getItem("LOGIN_STATE") != "1"){
        window.location.assign('/login');
      }else{
        getConfig();
      }
    }
    var xhttp = new XMLHttpRequest();
    var ssid_ap, pass_ap, user_login, pass_login;
    function getConfig(){
      xhttp.onreadystatechange = function(){
        if(xhttp.readyState==4 && xhttp.status==200){
          //alert(xhttp.responseText);
          var obj = JSON.parse(xhttp.responseText);
          ssid_ap = obj.ssid_ap;
          pass_ap = obj.pass_ap;
          user_login = obj.user_login;
          pass_login = obj.pass_login;
          ssid_sta = obj.ssid_sta;
          pass_sta = obj.pass_sta;
          auth_token=obj.auth_token;
          document.getElementById("ssid_ap").value=ssid_ap;
          document.getElementById("pass_ap").value=pass_ap;
          if(ssid_sta!=null){
            document.getElementById("ssid_sta").value = ssid_sta;
            document.getElementById("pass_sta").value = pass_sta;
            document.getElementById("auth_token").value=auth_token;
          }
        }
      }
      xhttp.open("GET","/getConfig",true);
      xhttp.send();
    }

    function getWifiList(){
      document.getElementById("ssid_ap").readOnly=false;
      document.getElementById("pass_ap").readOnly=false;
      document.getElementById("ssid_sta").readOnly=false;
      document.getElementById("pass_sta").readOnly=false;
      document.getElementById("auth_token").readOnly=false;
      document.getElementById("save").style.display="block";
      document.getElementById("edit").style.display="none";
      xhttp.onreadystatechange = function(){
        if(xhttp.readyState==4 && xhttp.status==200){
          //alert(xhttp.responseText);
          var obj = JSON.parse(xhttp.responseText);
          ssidList = obj.ssidList;
          var select = document.getElementById("ssidList");
          for(var i=0; i<ssidList.length;++i){
            select[select.length] = new Option(ssidList[i],ssidList[i]);
          }
        }
      }
      xhttp.open("GET","/getWifiList",true);
      xhttp.send();
    }
    function updatessid_sta(){
      document.getElementById("ssid_sta").value = document.getElementById("ssidList").value;
    }
    function saveConfig(){
        const json = {
          ssid_ap: document.getElementById("ssid_ap").value,
          pass_ap: document.getElementById("pass_ap").value,
          user_login: user_login,
          pass_login: pass_login,
          ssid_sta: document.getElementById("ssid_sta").value,
          pass_sta: document.getElementById("pass_sta").value,
          auth_token: document.getElementById("auth_token").value
        };
        const jsonString = JSON.stringify(json);
        xhttp.onreadystatechange = function(){
          if(xhttp.readyState==4 && xhttp.status==200){
            alert(xhttp.responseText);
            document.getElementById("ssid_ap").readOnly=true;
            document.getElementById("pass_ap").readOnly=true;
            document.getElementById("ssid_sta").readOnly=true;
            document.getElementById("pass_sta").readOnly=true;
            document.getElementById("auth_token").readOnly=true;
            document.getElementById("save").style.display="none";
            document.getElementById("edit").style.display="block";
          }
        }
        xhttp.open("POST","/saveConfig",true);
        xhttp.setRequestHeader("Content-type","application/json");
        xhttp.send(jsonString);
    }
    function reStart(){
      if(confirm("Bạn có muốn khởi động lại thiết bị hay không?")){
        alert("Hệ thống đang khởi động lại vui lòng đợi trong giây lát!")
        xhttp.open("GET","/reStart",true);
        xhttp.send();
      }
    }
    function showpass(m){
      var temp = document.getElementById(m);
      if(temp.type=="password"){
        temp.type = "text";
      }else{
        temp.type="password";
      }
    }
    function Empty(element,message){
      if(element.trim()==""){
        alert(message);
        element.focus();
        return false;
      }else{
        return true;
      }
    }
  </script>
</body>
</html>
)=====";
