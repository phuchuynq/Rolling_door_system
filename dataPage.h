const char dataPage[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html>
<head>
  <title>SETUP RFID</title>
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
    #dataStore{
      height: 200px;
      width: 380px;
      overflow-y: scroll;
      border: solid 1px blue;
    }
    table{
      width: 380px;
      height: 50px;
      float: left;
    }
    table, td 
    {
        border: solid 1px #DDD;
        border-collapse: collapse;
        padding: 2px 3px;
        text-align: left;
        font:14px Verdana;
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
    input[type="text"]{
      width: 270px;
      float: left;
      margin: 5px 0px;
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
      <div class="div1" style="width: 500px;margin-left: 80px;">
        <h3>THÔNG TIN THẺ</h3>
          <p><span class="label">Mã thẻ:</span>
              <input id="idcard" type="text"></p><br><br>
          <p><span class="label">Tên thẻ:</span>
              <input id="namecard" type="text"></p>
      </div>
      <div class="div1" style="margin-left: 80px;">
        <input style="background: green; color: white;border-color: orange;" type="button" onclick="addcard()" value="Thêm">
        <input style="background: yellow; color: green;border-color: orange;" type="button" onclick="deletecard()" value="Xóa">
        <input style="background: red; color: white;border-color: orange;" type="button" onclick="exit()" value="Thoát">
      </div>
      <div class="div1" style="margin-left: 80px;">
        <h3 style="margin-top: 10px;">DANH SÁCH THẺ RFID</h3>
        <div id="dataStore" style="float: left;margin-left: 0px;">
        </div>
      </div>
  </div>
  <script type="text/javascript">
    window.onload = function(){
       if(sessionStorage.getItem("LOGIN_STATE") != "1"){
         window.location.assign('/login');
       }else{
         getDataStore();
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

    var xhttp = new XMLHttpRequest();
    function getDataStore(){
      xhttp.onreadystatechange = function(){
        if(xhttp.readyState==4 && xhttp.status==200){
          console.log("Data response:" ,xhttp.responseText);
          obj = JSON.parse(xhttp.responseText);
          window.data=obj;
          num=1;
          var table = "<table id='tableId'>";
          table += `<tr><td>STT</td><td>MÃ THẺ</td><td>TÊN THẺ</td></tr>`;
          for (let key in obj) {
            table += `<tr><td>${num}</td><td>${key}</td><td>${obj[key]}</td></tr>`;
            num++;
          }
          table += "</table>";
          document.getElementById("dataStore").innerHTML = table;

          var rows = document.getElementById("tableId").rows;
          for (i = 0; i < rows.length; i++) {
              rows[i].onclick = function(){ return function(){
                     var id = this.cells[1].innerHTML;
                     var name= this.cells[2].innerHTML;
                     document.getElementById("idcard").value=id;
                     document.getElementById("namecard").value=name;
              };}(rows[i]);
          }
        }
      }
      xhttp.open("GET","/getDataStore",true);
      xhttp.send();
    }
    function deletecard(){
      var id = document.getElementById("idcard").value;
      if(Empty(id,"Vui lòng nhập mã thẻ!")){
        if(confirm("Bạn muốn xóa thẻ này?")){
          xhttp.onreadystatechange = function(){
              if(xhttp.readyState==4 && xhttp.status==200){
              //alert(xhttp.responseText);
                if(xhttp.responseText=="Đã xóa dữ liệu!"){
                  document.getElementById("idcard").value="";
                  document.getElementById("namecard").value="";
                  alert("Đã xóa thẻ!");
                  getDataStore();
                }
              }
            }
            xhttp.open("GET","/deleteDataStore?idcard="+id,true);
            xhttp.send();
        }
      }
    }
    
    function addcard(){
        var id = document.getElementById("idcard").value;
        var name = document.getElementById("namecard").value;
        if(Empty(id,"Vui lòng nhập mã thẻ!")&&Empty(name,"Vui lòng nhập vị trí!")){
          xhttp.onreadystatechange = function(){
            if(xhttp.readyState==4 && xhttp.status==200){
              //alert(xhttp.responseText);
              if(xhttp.responseText=="Đã thêm dữ liệu mới!"){
                document.getElementById("idcard").value="";
                document.getElementById("namecard").value="";
                getDataStore();
              }
            }
          }
          xhttp.open("GET","/addDataStore?idcard="+id+"&&namecard="+name,true);
          xhttp.send();
      }

    }

    function exit(){
      //sessionStorage.setItem("LOGIN_STATE","0");
      window.location.assign('/home');
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
