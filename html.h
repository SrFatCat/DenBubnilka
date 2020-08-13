const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title></title>
<style>
body { margin:0; font-size:0.8em; color:#2b2b2b;  line-height:16px; background-color:#e6e6e6; font-family: "Trebuchet MS", "Myriad Pro", Arial;}
a { color:#2b2b2b;}
input { color:#2b2b2b; }
form { color:#2b2b2b;}
select { color:#2b2b2b;}
button { color:#2b2b2b;}
a:hover { text-decoration:none;}
#content p {color:#2b2b2b; padding-bottom:0px; margin-bottom:0; }
#container { background: #e6e6e6; }
#header { background: #e6e6e6;  repeat-x #193441;  }
#logo { padding:25px 0 0 0; text-align:center; }
#logo a{color:#666; text-decoration:none; font-weight:bold; height:36px; font-size:30px;}
#logo a:hover { }
#menu { background:#7D8EE2; border-radius: 10px; width:780px; margin:0 auto; height:32px; margin-top:30px;}
#menu ul { margin:0; padding:0;}
#menu ul li{ display:inline-block; float:left; width:100px; list-style:none; text-align:center; height:32px;}
#menu a { border-radius: 10px; display:block; float:left; width:100px; color:#FFFFFF; font-size:14px; text-transform:uppercase; font-weight:bold; text-decoration:none; padding-top:8px;}
#menu a:hover{ background: #8A9DFA; padding-top:8px; height:24px; color:#fff}
#main { width:100%;}
#content { margin:0 auto;  width:760px; padding:10px 10px 20px 10px; }
h1 {text-align: center; margin:0; border-radius: 10px; font-size:1em; color:#fff; background:#808080; width:750px; padding:4px; margin-top:20px;}
#footer {background:#e6e6e6; repeat-x #193340 0 2px; height:100px}
#left_footer { float:left; padding:40px 0 0 30px;  color:#233D2E; font-weight:bold; font-size:11px;}
#left_footer a { color:#666;}
#left_footer a:hover { text-decoration:none;}
</style>
</head>
<body>
<div id="container">
	<!-- header -->
    <div id="header">
    	<div id="logo"><a href="http://esp-radio.ru/" target="_blank">ESPmini</a> </div>
        <div id="menu">
        	<ul>
	    <li><a onclick="_click(1); return false;" href="#" >Станции</a></li>
       <li><a onclick="_click(2); return false;" href="#" >Система</a></li>
       <li><a onclick="_click(3); return false;" href="#" >Обновление</a></li>
         </ul>
      </div>
  </div>
    <!--end header -->
    <!-- main -->
    <div id="main">
    	<div id="content">
		<div style=" display:none" id="item1">
				<h1>Плейлист</h1>
				<center>
				<br>
<p>
00. URL: <input type="text"  id="url0"  size="50"> <input type="submit"  value="Сохранить" onclick="set_url(0);"><br>
01. URL: <input type="text"  id="url1"  size="50">  <input type="submit"  value="Сохранить" onclick="set_url(1);"><br>
02. URL: <input type="text"  id="url2"  size="50"> <input type="submit"  value="Сохранить" onclick="set_url(2);"><br>
03. URL: <input type="text"  id="url3"  size="50">  <input type="submit"  value="Сохранить" onclick="set_url(3);"><br>
04. URL: <input type="text"  id="url4"  size="50">  <input type="submit"  value="Сохранить" onclick="set_url(4);"><br>
05. URL: <input type="text"  id="url5"  size="50"> <input type="submit"  value="Сохранить" onclick="set_url(5);"><br>
06. URL: <input type="text"  id="url6"  size="50"> <input type="submit"  value="Сохранить" onclick="set_url(6);"><br>
07. URL: <input type="text"  id="url7"  size="50">  <input type="submit"  value="Сохранить" onclick="set_url(7);"><br>
08. URL: <input type="text"  id="url8"  size="50">  <input type="submit"  value="Сохранить" onclick="set_url(8);"><br>
09. URL: <input type="text"  id="url9"  size="50">  <input type="submit"  value="Сохранить" onclick="set_url(9);"><br>
</p>
</center>
   </div>
	<div style=" display:none" id="item2">
<h1>Информация</h1>    
<br> 
<center>
Версия: <span id="ver">-</span><br>
</center>
<h1>Настройка пульта</h1> 
  <br>
 <center>
 Принятый код пульта: <strong><span id="rxir">-</span></strong><br>
   <br>
 <table >
 <tr valign="middle" align="right ">
 <td >
  Включить-выключить: 
 </td>  
 <td >
 <input type="text"  id="power"  size="8"><br>
 </td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('power').value=document.getElementById('rxir').innerHTML;">
 </td>
 </tr>

<tr valign="middle" align="right ">
 <td >
  Приемник-Точка доступа: 
 </td>  
 <td >
 <input type="text"  id="mode"  size="8"><br>
 </td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('mode').value=document.getElementById('rxir').innerHTML;">
 </td>
 </tr>
 
 <tr valign="middle" align="right ">
 <td >
Канал +: 
</td >
<td >
<input type="text"  id="ch_up"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_up').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
  <tr valign="middle" align="right ">
 <td >
Канал -: 
</td >
<td >
<input type="text"  id="ch_dwn"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_dwn').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>

<tr valign="middle" align="right ">
  <td >
Звук +:
 </td >
  <td >
  <input type="text"  id="vol_up"  size="8"><br>
   </td >
    <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('vol_up').value=document.getElementById('rxir').innerHTML;">
 </td>
   </tr>
  <tr valign="middle" align="right ">
  <td > 
Звук -: 
</td >
  <td > 
<input type="text" id="vol_dwn"  size="8"><br>  
  </td > 
   <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('vol_dwn').value=document.getElementById('rxir').innerHTML;">
 </td>
  </tr>

 <tr valign="middle" align="right ">
 <td >
Кнопка 0: 
</td >
<td >
<input type="text"  id="ch_0"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_0').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
 <tr valign="middle" align="right ">
 <td >
Кнопка 1: 
</td >
<td >
<input type="text"  id="ch_1"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_1').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
 <tr valign="middle" align="right ">
 <td >
Кнопка 2: 
</td >
<td >
<input type="text"  id="ch_2"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_2').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
 <tr valign="middle" align="right ">
 <td >
Кнопка 3: 
</td >
<td >
<input type="text"  id="ch_3"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_3').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
 <tr valign="middle" align="right ">
 <td >
Кнопка 4: 
</td >
<td >
<input type="text"  id="ch_4"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_4').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
 <tr valign="middle" align="right ">
 <td >
Кнопка 5: 
</td >
<td >
<input type="text"  id="ch_5"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_5').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
 <tr valign="middle" align="right ">
 <td >
Кнопка 6: 
</td >
<td >
<input type="text"  id="ch_6"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_6').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
 <tr valign="middle" align="right ">
 <td >
Кнопка 7: 
</td >
<td >
<input type="text"  id="ch_7"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_7').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
 <tr valign="middle" align="right ">
 <td >
Кнопка 8: 
</td >
<td >
<input type="text"  id="ch_8"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_8').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
 <tr valign="middle" align="right ">
 <td >
Кнопка 9: 
</td >
<td >
<input type="text"  id="ch_9"  size="8"><br>
</td >
 <td>
 <input type="submit"  value="Установить" onclick="document.getElementById('ch_9').value=document.getElementById('rxir').innerHTML;">
 </td>
</tr>
 
  <tr valign="middle" align="center">
  <td colspan="3">
  <br>
<input type="submit"  value="Сохранить" onclick="set_ir();"><br>
</td >
</tr>
</table >
</center>
<h1>Настройка сети</h1>    
<br> 
<center>
<table >
 <tr valign="middle" align="right ">
<td >
Имя сети (SSID):
</td>  
<td>
 <input type="text" placeholder="Имя Wi-Fi сети" id="ssid" size="22"> <br>
 </td>
 </tr>
  <tr valign="middle" align="right ">
 <td >
Ключ (PASW):
</td>
<td>
<input type="password" id="wifipsw" size="22">  <br>
</td>
<td>
<input type="submit" value="Сохранить" onclick="set_lan();">
<span id="lanok"></span>
</td>
 </tr>
  </table >
</center>
<h1>Системные команды</h1> 
<br>
<center>
Тип индикатора : 
 <select id="indicator" >
<option>Led</option>
<option>Display</option>
</select>
<input type="submit" value="Сохранить" onclick="set_indicator();"><br>
<br> 
Данные для отладки в Uart: 
 <select id="uart" >
<option>On</option>
<option>Off</option>
</select>
<input type="submit" value="Сохранить" onclick="set_uart();"><br>
</center>

<br> 
<center><input type="submit" value="Перезагрузить и выйти из режима точки доступа" onclick="reset();"></center><br>
<br> 
<center><input type="submit" value="Сброс к заводским настройкам" onclick="freset();"></center><br>
</div>	
<div style=" display:none" id="item3">
<h1>Обновление ПО приемника</h1>    
<br>
<center>
<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' multiple name='update' ><input type='submit' onclick="stoptimer();"  value='Обновить прошивку'></form>
</center>
	</div>		
		</div>
    </div>
<div id="footer">
    <div id="left_footer">&copy; ESPradio Copyright 2018 Bubnov Evgeny
    </div>
    </div>
</div>

<script type="text/javascript">
var _click = function () {
        var b = 1;
        return function (c) {
            var a = document.getElementById("item" + b);
            c == b && (a.style.display = "none" == a.style.display ? "" : "");
            c != b && (a.style.display = "none", a = document.getElementById("item" + c), a.style.display = "", b = c)
        }
    }();
window.onload = function() {
    _click(1)
    refresh();
	  ir_refresh();
 }
</script>
</body>

 <SCRIPT>
   var xmlHttp=createXmlHttpObject();
 var timerId

function createXmlHttpObject(){
try {
    xmlHttp = new ActiveXObject("Msxml2.XMLHTTP");
  } catch (e) {
    try {
      xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
    } catch (E) {
      xmlHttp = false;
    }
  }
  if (!xmlHttp && typeof XMLHttpRequest!='undefined') {
    xmlHttp = new XMLHttpRequest();
  }
  return xmlHttp;
}

function stoptimer(){
  clearInterval(timerId);
}

function ir_refresh(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
   xmlHttp.open('GET','rc.conf?r='+Math.random(),true);
   xmlHttp.onreadystatechange=ir_handleServerResponse;
   xmlHttp.send(null);
 }
 setTimeout('ir_refresh()',1000); 
}
function ir_handleServerResponse(){
 if(xmlHttp.readyState==4 && xmlHttp.status==200){
   xmlResponse=xmlHttp.responseText;
  var JsonObject= JSON.parse(xmlResponse);
   document.getElementById('rxir').innerHTML=JsonObject.rxir;
}
}

function refresh(){
   if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
   xmlHttp.open('GET','json.conf?r='+Math.random(),true);
   xmlHttp.onreadystatechange=handleServerResponse;
   xmlHttp.send(null);
 }
}

function handleServerResponse(){
 if(xmlHttp.readyState==4 && xmlHttp.status==200){
   xmlResponse=xmlHttp.responseText;
 var JsonObject= JSON.parse(xmlResponse);
   document.getElementById('ver').innerHTML=JsonObject.ver;
   document.getElementById('ssid').value=JsonObject.ssid;
  document.getElementById('wifipsw').value=JsonObject.wifipsw; 
  document.getElementById('uart').value=JsonObject.state_uart;
  document.getElementById('indicator').value=JsonObject.indicator;
  document.getElementById('mode').value=JsonObject.mode;
  document.getElementById('url0').value=JsonObject.url0;
  document.getElementById('url1').value=JsonObject.url1;
  document.getElementById('url2').value=JsonObject.url2;
  document.getElementById('url3').value=JsonObject.url3;
  document.getElementById('url4').value=JsonObject.url4;
  document.getElementById('url5').value=JsonObject.url5;
  document.getElementById('url6').value=JsonObject.url6;
  document.getElementById('url7').value=JsonObject.url7;
  document.getElementById('url8').value=JsonObject.url8;
  document.getElementById('url9').value=JsonObject.url9;
  document.getElementById('power').value=JsonObject.power;
  document.getElementById('ch_up').value=JsonObject.ch_up;
  document.getElementById('ch_dwn').value=JsonObject.ch_dwn;
  document.getElementById('vol_up').value=JsonObject.vol_up;
  document.getElementById('vol_dwn').value=JsonObject.vol_dwn;
  document.getElementById('ch_0').value=JsonObject.ch_0;
  document.getElementById('ch_1').value=JsonObject.ch_1;
  document.getElementById('ch_2').value=JsonObject.ch_2;
  document.getElementById('ch_3').value=JsonObject.ch_3;
  document.getElementById('ch_4').value=JsonObject.ch_4;
  document.getElementById('ch_5').value=JsonObject.ch_5;
  document.getElementById('ch_6').value=JsonObject.ch_6;
  document.getElementById('ch_7').value=JsonObject.ch_7;
  document.getElementById('ch_8').value=JsonObject.ch_8;
  document.getElementById('ch_9').value=JsonObject.ch_9;
 }
}
function set_lan(){
ssid = document.getElementById('ssid').value;
psw= document.getElementById('wifipsw').value;
server = "/set?ssid="+encodeURIComponent(ssid)+"&wifipsw="+encodeURIComponent(psw);
request = new XMLHttpRequest();
request.open("GET", server, true);
request.send();
document.getElementById('lanok').innerHTML='OK';
setTimeout("document.getElementById('lanok').innerHTML=''",2000);  
}

function reset(){
server = "/reboot";
request = new XMLHttpRequest();
request.open("GET", server, true);
request.send();
}

function freset(){
server = "/freset";
request = new XMLHttpRequest();
request.open("GET", server, true);
request.send();
}

function set_url(urlnum){
  //http://192.168.0.101/url0?url=http://dfsdfd
urlx = document.getElementById('url'+urlnum).value;
server = "/url"+urlnum+"?url="+encodeURIComponent(urlx);
request = new XMLHttpRequest();
request.open("GET", server, true);
request.send();
}

function set_uart(){
  //http://192.168.0.101/url0?url=http://dfsdfd
server = "/uart?state="+encodeURIComponent(document.getElementById('uart').value);
request = new XMLHttpRequest();
request.open("GET", server, true);
request.send();
}

function set_indicator(){
  //http://192.168.0.101/url0?url=http://dfsdfd
server = "/indicator?state="+encodeURIComponent(document.getElementById('indicator').value);
request = new XMLHttpRequest();
request.open("GET", server, true);
request.send();
}

function set_ir(){
  power = document.getElementById('power').value;
  ch_up = document.getElementById('ch_up').value;
  ch_dwn = document.getElementById('ch_dwn').value;
  vol_up = document.getElementById('vol_up').value;
  vol_dwn = document.getElementById('vol_dwn').value;
  ch_0 = document.getElementById('ch_0').value;
  ch_1 = document.getElementById('ch_1').value;
 ch_2 = document.getElementById('ch_2').value;
 ch_3 = document.getElementById('ch_3').value;
 ch_4 = document.getElementById('ch_4').value;
 ch_5 = document.getElementById('ch_5').value;
 ch_6 = document.getElementById('ch_6').value;
 ch_7 = document.getElementById('ch_7').value;
 ch_8 = document.getElementById('ch_8').value;
 ch_9 = document.getElementById('ch_9').value;
 mode=document.getElementById('mode').value;
   server = "/ir?power="+encodeURIComponent(power)+"&mode="+encodeURIComponent(mode)+"&ch_up="+encodeURIComponent(ch_up)+"&ch_dwn="+encodeURIComponent(ch_dwn)+"&vol_up="+encodeURIComponent(vol_up)+"&vol_dwn="+encodeURIComponent(vol_dwn)+"&ch_0="+encodeURIComponent(ch_0)+"&ch_1="+encodeURIComponent(ch_1)+"&ch_2="+encodeURIComponent(ch_2)+"&ch_3="+encodeURIComponent(ch_3)+"&ch_4="+encodeURIComponent(ch_4)+"&ch_5="+encodeURIComponent(ch_5)+"&ch_6="+encodeURIComponent(ch_6)+"&ch_7="+encodeURIComponent(ch_7)+"&ch_8="+encodeURIComponent(ch_8)+"&ch_9="+encodeURIComponent(ch_9);
request = new XMLHttpRequest();
request.open("GET", server, true);
request.send();
}

</SCRIPT>
</html>
)=====";
