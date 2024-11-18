const char homePage[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html>
<head>
  <title>HOME</title>
  <style type="text/css">
    body{
      background: white;
    }
    #main{
      margin: 5px auto;
      width: 728px;
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
    .label{
      width: 100px;
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
      color: blue;
      background: white;
      border-radius: 5px;
      border-color: blue;
    }
    input[type="text"]{
      width: 270px;
      float: left;
      margin: 5px 0px;
    }
    .row input[type="button"]{
      width: 120px;
      height: 100px;
      float: left;
      margin: 3px 4px;
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
      <div class="div1" style="margin-left: 80px;">
        <div>
          <h3 style="color: blue">THẺ RFID</h3>
          <p><span class="label">Mã thẻ:</span>
              <input id="idcard" type="text"></p>
          <p><span class="label">Tên thẻ:</span>
              <input id="namecard" type="text"></p>
        </div>
      </div>
      <div class="div1" style="margin-left: 80px;">
        <div class="row">
          <input id="open" onclick="sendControl('open')" type="button" value="MỞ CỬA">
          <input id="close" onclick="sendControl('close')" type="button" value="ĐÓNG CỬA">
          <input id="stop" onclick="sendControl('stop')" type="button" value="DỪNG">
        </div> 
      </div>
      <div class="div1" style="margin-left: 100px;">
        <input style="background: green; color: white;border-color: orange;" type="button" onclick="window.location.href='/setup'" value="Thiết lập wifi">
        <input style="background: yellow; color: green;border-color: orange;" type="button" onclick="window.location.href='/data'" value="Thiết lập thẻ RFID">
        <input style="background: red; color: white;border-color: orange;" type="button" onclick="exit()" value="Thoát">
      </div>
  </div>
  <script type="text/javascript">
    window.onload = function(){
       if(sessionStorage.getItem("LOGIN_STATE") != "1"){
         window.location.assign('/login');
       }else{
         init();
       }
    }
    var Socket;      //Khai báo biến Socket
    var temp=0;
    function init(){
       //Khởi tạo websocket
       Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
       //Nhận broadcase từ server
       Socket.onmessage = function(event){   
         JSONobj = JSON.parse(event.data);   //Tách dữ liệu json
         //alert(event.data);
         idcard = JSONobj.idcard;
         namecard = JSONobj.namecard;
         document.getElementById("idcard").value = idcard;
         document.getElementById("namecard").value = namecard;
       }
    }

    function sendControl(ctr){
      Socket.send(ctr);
    }


    function exit(){
      sessionStorage.setItem("LOGIN_STATE","0");
      window.location.assign('/login');
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
