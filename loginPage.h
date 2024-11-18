const char* loginPage PROGMEM = R"html(
<!DOCTYPE html>
<html>
<head>
  <title>LOGIN</title>
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
      width: 160px;
      height: 50px;
      float: left;
      margin: 10px 8px;
      outline: none;
      text-align: center; 
          white-space: normal;
          font-weight: bold;
    }
  </style>
  <meta name="viewport" content="width=device-width,user-scalable=0"  charset="utf-8">
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
      <div style="margin-left: 80px;">
        <h3>ĐĂNG NHẬP HỆ THỐNG</h3>
      </div>
      <div class="div1" style="width: 500px;margin-left: 80px;">
          <p>Tên đăng nhập:</p>
          <input id="user_login" class="input" type="text"><br>
          <p>Mật khẩu:</p>
          <input id="pass_login" class="input" type="password">
          <input onclick="showpass('pass_login')" type="checkbox">hiện
      </div>
      
      <div id="btlogin" class="div1" style="margin-left: 70px;">
        <input style="background: green; color: white;border-color: orange;" onclick="checkLogin('1')" type="button" value="Đăng nhập">
        <input style="background: yellow; color: green;border-color: orange;" onclick="checkLogin('2')" type="button" value="Đổi mật khẩu">
      </div>

      <div id="btupdate" class="div1" style="margin-left: 70px;">
        <input style="background: yellow; color: green;border-color: orange;" onclick="saveLogin()" type="button" value="Lưu">
        <input style="background: red; color: white;border-color: orange;" onclick="window.location.href='/'" type="button" value="Thoát">
      </div>
    </div>    
  </div>

  <script type="text/javascript">
    window.onload=function(){
      document.getElementById("btlogin").style.display="block";
      document.getElementById("btupdate").style.display="none";
    }

    var xhttp = new XMLHttpRequest();
    function checkLogin(m){
      user_login = document.getElementById("user_login").value;
      pass_login = document.getElementById("pass_login").value;
      xhttp.onreadystatechange = function(){
        if(xhttp.readyState==4&&xhttp.status==200){
          var loginState = xhttp.responseText;
          if(loginState=="OK"){
            sessionStorage.setItem("LOGIN_STATE","1");
            if(m=='1'){
              window.location.assign('/');
            }else{
              document.getElementById("btlogin").style.display="none";
              document.getElementById("btupdate").style.display="block";
            }
          }else{
            sessionStorage.setItem("LOGIN_STATE","0");
            alert(loginState);
          }
        }
      }
      xhttp.open("GET","/checkLogin?user_login="+user_login + "&pass_login=" + pass_login,true);
          xhttp.send();
    }

    function saveLogin(){
      user_login = document.getElementById("user_login").value;
      pass_login = document.getElementById("pass_login").value;
      xhttp.onreadystatechange = function(){
        if(xhttp.readyState==4&&xhttp.status==200){
          alert(xhttp.responseText);
        }
      }
      xhttp.open("GET","/saveLogin?user_login="+user_login + "&pass_login=" + pass_login,true);
          xhttp.send();
    }

    function showpass(m){
          var temp = document.getElementById(m);
          if(temp.type=="password"){
            temp.type = "text";
          }else{
            temp.type="password";
         }
      }
  </script>
</body>
</html>
)html";
