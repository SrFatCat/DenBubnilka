/*
 Name:		DenBubnilka.ino
 Created:	28.05.2019 15:08:49
 Author:	Alex
*/
#define _DEBUG

#include "KR145IK1901.h"
#include "MyArduino.h"
#include <ESP8266WiFi.h>
extern "C" {
#include <user_interface.h>
}
#include "AudioFileSourceICYStream.h"//ESP8266Audio-master_mod_espmini.zip (����������� � �������)
#include "AudioFileSourceBuffer.h" //ESP8266Audio-master_mod_espmini.zip (����������� � �������)
#include "AudioGeneratorMP3.h" //ESP8266Audio-master_mod_espmini.zip (����������� � �������)
//#include "AudioGeneratorWAV.h"
//#include "AudioFileSourceSPIFFS.h"
#include "AudioOutputI2SNoDAC.h" //ESP8266Audio-master_mod_espmini.zip // исправлено https://github.com/Gianbacchio/ESP8266_Spiram
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <FS.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include <DNSServer.h>
#include "html.h" 
#include "logo.h"

#include "UniversalKeyboardDriver.h"
enum EKeyCodes { KEY_UP = 5, KEY_DOWN = 0, KEY_SLEEP = 4};
const int16_t keyCodeArray_d[] = { KEY_UP,  KEY_DOWN, KEY_SLEEP };
CKeyboardDriver keyDriver(1);

CKR145IK1901 cClock;

#define Debug Serial

int ver = 37;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

int mute = 16;
int vol; //������� 
bool temp_write; //���� ��������� �������

int ch;
bool stream_ok; //������ ������
bool apstart = 0; //������ ������ ������ wifi
bool power_ok = 1;
bool isSleep;

String ssid, wifipsw; //ssid �  wifi pasword
String jsonConfig = "{}"; //json
String url0, url1, url2, url3, url4, url5, url6, url7, url8, url9; //url �������
//unsigned int ir_power, ir_mode, ir_ch_up, ir_ch_dwn, ir_vol_up, ir_vol_dwn, rxir, ir_ch_0, ir_ch_1, ir_ch_2, ir_ch_3, ir_ch_4, ir_ch_5, ir_ch_6, ir_ch_7, ir_ch_8, ir_ch_9;
String url_tmp;
String state_uart;
String indicator;
String radio_name, radio_br;
int  posstart, posstop;
bool revers;

int lastms = 0, refresh = 0; //������� ������� (���������)
int timer_read;

//IRrecv irrecv(kRecvPin);
//decode_results results;

ESP8266WebServer server(80); //������ ��� �������

//AudioGeneratorWAV* wav;
//AudioFileSourceSPIFFS *file;

AudioGeneratorMP3* mp3;
AudioFileSourceICYStream* file;
AudioFileSourceBuffer* buff;
AudioOutputI2SNoDAC* out;

//wav = new AudioGeneratorWAV();
//wav->begin(file, out);
//  file = new AudioFileSourceSPIFFS("/pno-cs.mp3");
// mp3 = new AudioGeneratorMP3();
// mp3->begin(id3, out);


bool savetemp() // ��������� ����������� � ����
{
	DynamicJsonDocument root(1024);
	root["ch"] = String(ch);
	root["vol"] = String(vol);
	root["ap"] = String(apstart);
	root["sleep"] = String(isSleep);
	String json;
	serializeJson(root, json);
	Debug.println(json);

	File configFile = SPIFFS.open("/temp.json", "w");// ��������� ���� ��� ������
	if (!configFile) {
		Debug.println("Error open temp file for write!");
		return false;
	}

	if (stream_ok == 1) mp3->loop();//������� ���������� ������� �������� "��������"

	configFile.print(json);// ���������� ������ json � ����

	if (stream_ok == 1) mp3->loop();

	configFile.close();//��������� ����

	//json = String();

	return true;
}

bool saveConfig() // ��������� ����������� � ����
{
	if (ssid == "") ssid = "ssid";
	if (wifipsw == "") wifipsw = "none";
	if (url0 == "") url0 = "http://";
	if (url1 == "") url1 = "http://";
	if (url2 == "") url2 = "http://";
	if (url3 == "") url3 = "http://";
	if (url4 == "") url4 = "http://";
	if (url5 == "") url5 = "http://";
	if (url6 == "") url6 = "http://";
	if (url7 == "") url7 = "http://";
	if (url8 == "") url8 = "http://";
	if (url9 == "") url9 = "http://";

	String json = "{";
	json += "\"ssid\":";
	json += "\"" + ssid + "\",";
	json += "\"wifipsw\":";
	json += "\"" + wifipsw + "\",";

	json += "\"state_uart\":";
	json += "\"" + state_uart + "\",";

	json += "\"indicator\":";
	json += "\"" + indicator + "\",";

	json += "\"url0\":";
	json += "\"" + url0 + "\",";
	json += "\"url1\":";
	json += "\"" + url1 + "\",";
	json += "\"url2\":";
	json += "\"" + url2 + "\",";
	json += "\"url3\":";
	json += "\"" + url3 + "\",";
	json += "\"url4\":";
	json += "\"" + url4 + "\",";
	json += "\"url5\":";
	json += "\"" + url5 + "\",";
	json += "\"url6\":";
	json += "\"" + url6 + "\",";
	json += "\"url7\":";
	json += "\"" + url7 + "\",";
	json += "\"url8\":";
	json += "\"" + url8 + "\",";
	json += "\"url9\":";
	json += "\"" + url9 + "\"";
	//json += "\"" + url9 + "\",";

	//json += "\"power\":";
	//json += "\"" + String(1) + "\",";
	//json += "\"mode\":";
	//json += "\"" + String(ir_mode) + "\",";
	//json += "\"ch_up\":";
	//json += "\"" + String(ir_ch_up) + "\",";
	//json += "\"ch_dwn\":";
	//json += "\"" + String(ir_ch_dwn) + "\",";
	//json += "\"vol_up\":";
	//json += "\"" + String(ir_vol_up) + "\",";
	//json += "\"vol_dwn\":";
	//json += "\"" + String(ir_vol_dwn) + "\",";
	//json += "\"ch_0\":";
	//json += "\"" + String(ir_ch_0) + "\",";
	//json += "\"ch_1\":";
	//json += "\"" + String(ir_ch_1) + "\",";
	//json += "\"ch_2\":";
	//json += "\"" + String(ir_ch_2) + "\",";
	//json += "\"ch_3\":";
	//json += "\"" + String(ir_ch_3) + "\",";
	//json += "\"ch_4\":";
	//json += "\"" + String(ir_ch_4) + "\",";
	//json += "\"ch_5\":";
	//json += "\"" + String(ir_ch_5) + "\",";
	//json += "\"ch_6\":";
	//json += "\"" + String(ir_ch_6) + "\",";
	//json += "\"ch_7\":";
	//json += "\"" + String(ir_ch_7) + "\",";
	//json += "\"ch_8\":";
	//json += "\"" + String(ir_ch_8) + "\",";
	//json += "\"ch_9\":";
	//json += "\"" + String(ir_ch_9) + "\"";

	json += "}";

	//Debug.println(json);

	File configFile = SPIFFS.open("/config.json", "w");// ��������� ���� ��� ������
	if (!configFile) {
		Debug.println("Error open config file for write!");
		return false;
	}

	configFile.print(json);// ���������� ������ json � ����

	configFile.close();//��������� ����

	json = String();

	return true;
}

void set()
{
	ssid = server.arg("ssid");
	wifipsw = server.arg("wifipsw");
	saveConfig();
	server.send(200, "text/html", MAIN_page);
}

void webinit() {
	DEBUG_PRINT("Start web");

	server.onNotFound([]() {
		server.send(200, "text/html", MAIN_page);
		});

	server.on("/freset", HTTP_GET, []() {
		SPIFFS.format();
		server.send(200, "text/html", "OK");
		ESP.restart();
		});

	server.on("/reboot", HTTP_GET, []() {
		server.send(200, "text/html", "OK");
		apstart = 0;
		savetemp();
		ESP.restart();
		});
	server.on("/update", HTTP_POST, []() {
		server.sendHeader("Connection", "close");
		server.send(200, "text/html", (Update.hasError()) ? "<center>FAIL Update<br> <a href=""/"" >Back</a></center>" : "<center>Update firmware OK. Reboot... <br> <a href=""/"" >Back</a></center>");
		if (Update.hasError() != 1) {
			delay(500);
			ESP.restart();
		}
		}, []() {
			HTTPUpload& upload = server.upload();
			if (upload.status == UPLOAD_FILE_START) {
				//Debug.setDebugOutput(true);
				Debug.printf("Update: %s\n", upload.filename.c_str());
				uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
				if (!Update.begin(maxSketchSpace)) {//start with max available size
					Update.printError(Debug);
				}
			}
			else if (upload.status == UPLOAD_FILE_WRITE) {
				if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
					Update.printError(Debug);
				}
			}
			else if (upload.status == UPLOAD_FILE_END) {
				if (Update.end(true)) { //true to set the size to the current progress
					Debug.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
				}
				else {
					Update.printError(Debug);
				}
				//Debug.setDebugOutput(false);
			}
			yield();
		});

	server.on("/json.conf", HTTP_GET, []() {//��������� ������� ������������ � ���������
		String json = "{";
		json += "\"ver\":";
		json += "\"" + String(ver) + "\",";
		json += "\"ssid\":";
		json += "\"" + ssid + "\",";
		json += "\"wifipsw\":";
		json += "\"" + wifipsw + "\",";

		json += "\"state_uart\":";
		json += "\"" + state_uart + "\",";

		json += "\"indicator\":";
		json += "\"" + indicator + "\",";

		json += "\"url0\":";
		json += "\"" + url0 + "\",";
		json += "\"url1\":";
		json += "\"" + url1 + "\",";
		json += "\"url2\":";
		json += "\"" + url2 + "\",";
		json += "\"url3\":";
		json += "\"" + url3 + "\",";
		json += "\"url4\":";
		json += "\"" + url4 + "\",";
		json += "\"url5\":";
		json += "\"" + url5 + "\",";
		json += "\"url6\":";
		json += "\"" + url6 + "\",";
		json += "\"url7\":";
		json += "\"" + url7 + "\",";
		json += "\"url8\":";
		json += "\"" + url8 + "\",";
		json += "\"url9\":";
		json += "\"" + url9 + "\"";
		//json += "\"" + url9 + "\",";

		//json += "\"power\":";
		//json += "\"" + String(ir_power) + "\",";
		//json += "\"mode\":";
		//json += "\"" + String(ir_mode) + "\",";
		//json += "\"ch_up\":";
		//json += "\"" + String(ir_ch_up) + "\",";
		//json += "\"ch_dwn\":";
		//json += "\"" + String(ir_ch_dwn) + "\",";
		//json += "\"vol_up\":";
		//json += "\"" + String(ir_vol_up) + "\",";
		//json += "\"vol_dwn\":";
		//json += "\"" + String(ir_vol_dwn) + "\",";

		//json += "\"ch_0\":";
		//json += "\"" + String(ir_ch_0) + "\",";
		//json += "\"ch_1\":";
		//json += "\"" + String(ir_ch_1) + "\",";
		//json += "\"ch_2\":";
		//json += "\"" + String(ir_ch_2) + "\",";
		//json += "\"ch_3\":";
		//json += "\"" + String(ir_ch_3) + "\",";
		//json += "\"ch_4\":";
		//json += "\"" + String(ir_ch_4) + "\",";
		//json += "\"ch_5\":";
		//json += "\"" + String(ir_ch_5) + "\",";
		//json += "\"ch_6\":";
		//json += "\"" + String(ir_ch_6) + "\",";
		//json += "\"ch_7\":";
		//json += "\"" + String(ir_ch_7) + "\",";
		//json += "\"ch_8\":";
		//json += "\"" + String(ir_ch_8) + "\",";
		//json += "\"ch_9\":";
		//json += "\"" + String(ir_ch_9) + "\"";

		json += "}";

		server.send(200, "text/javascript", json);
		json = String();
		});
		
	server.on("/uart", HTTP_GET, []() {
		state_uart = server.arg("state");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});

	server.on("/indicator", HTTP_GET, []() {
		indicator = server.arg("state");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});

	//http://192.168.0.101/url0?url=http://dfsdfd
	server.on("/url0", HTTP_GET, []() {//��� ������ get ��������� url
		url0 = server.arg("url");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});
	server.on("/url1", HTTP_GET, []() {
		url1 = server.arg("url");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});
	server.on("/url2", HTTP_GET, []() {
		url2 = server.arg("url");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});
	server.on("/url3", HTTP_GET, []() {
		url3 = server.arg("url");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});
	server.on("/url4", HTTP_GET, []() {
		url4 = server.arg("url");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});
	server.on("/url5", HTTP_GET, []() {
		url5 = server.arg("url");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});
	server.on("/url6", HTTP_GET, []() {
		url6 = server.arg("url");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});
	server.on("/url7", HTTP_GET, []() {
		url7 = server.arg("url");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});
	server.on("/url8", HTTP_GET, []() {
		url8 = server.arg("url");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});
	server.on("/url9", HTTP_GET, []() {
		url9 = server.arg("url");
		saveConfig();
		server.send(200, "text/html", MAIN_page);
		});

	server.on("/set", set);//��� ������ get ��������� ssid � psw

	server.begin();//��������� ��� ������

	apstart = 1; //������ ���� �������� ����� �������

}

void ap_start(){
	DEBUG_PRINT("\nStart ap\n");
	WiFi.mode(WIFI_AP);
	DEBUG_PRINT("Configuring access point...\n");
	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	WiFi.softAP("ESPmini");
	IPAddress myIP = WiFi.softAPIP();
	DEBUG_PRINT("SSID: ESPmini. AP IP address: %s\n", myIP.toString().c_str());
	dnsServer.start(DNS_PORT, "*", apIP);
	webinit();
}

void fail_save() {//���� ������� ��������� ������ �� ���������
	Debug.println("Fail save settings write!");
	ssid = "ssid";
	wifipsw = "none";
	state_uart = "On";
	indicator = "Led";
	url0 = "http://ep128.hostingradio.ru:8030/ep128";
	url1 = "http://94.19.12.205:9500/relax";
	url2 = "http://ic3.101.ru:8000/c19_2";
	url3 = "http://jfm1.hostingradio.ru:14536/jlstream.mp3";
	url4 = "http://16bitfm.com:8000/cafe_mp3_128"; // ****************
	url5 = "http://91.207.59.26:8000/c10_5";
	url6 = "http://ic3.101.ru:8000/c1_2";
	url7 = "";
	url8 = "";
	url9 = "";
	//ir_power = 1032765;
	//ir_mode = 1020015;
	//ir_ch_up = 1040925;
	//ir_ch_dwn = 1024605;
	//ir_vol_up = 1026135;
	//ir_vol_dwn = 1040415;
	//ir_ch_0 = 1009815;
	//ir_ch_1 = 995535;
	//ir_ch_2 = 989415;
	//ir_ch_3 = 1014405;
	//ir_ch_4 = 987375;
	//ir_ch_5 = 997575;
	//ir_ch_6 = 1006245;
	//ir_ch_7 = 1000125;
	//ir_ch_8 = 1002165;
	//ir_ch_9 = 1004205;
	saveConfig();//������� ���� ������� � ���� ����� �� ���������
	vol = 100;
	ch = 0;
	apstart = 0;
	savetemp();
	//return false;
}

bool loadConfig() // p������� ������ ����������� �� �����
{
	File configFile = SPIFFS.open("/config.json", "r");// ��������� ���� ��� ������
	if (!configFile) {// ���� ���� �� ������ �������� � ����� ������ �� ���������
		fail_save();
		return false;
	}

	size_t size = configFile.size();

	if (size > 1024) {// ��������� ������ �����
		Debug.println("Size error!");
		return false;
	}


	jsonConfig = configFile.readString();// ��������� ���� ������������ 

	DynamicJsonDocument root(1024);


	//DynamicJsonBuffer jsonBuffer;// ����������� ����� ��� json  
	//**********StaticJsonBuffer<900> jsonBuffer;
	//JsonObject& root = jsonBuffer.parseObject(jsonConfig);//  ������ ������� �� ����������
	DeserializationError error = deserializeJson(root, jsonConfig);
	if (error) {// ��������� ������ �����
		Debug.println("deserializeJson error!");
		return false;
	}
	ssid = root["ssid"].as<String>();
	wifipsw = root["wifipsw"].as<String>();

	state_uart = root["state_uart"].as<String>();

	indicator = root["indicator"].as<String>();

	url0 = root["url0"].as<String>();
	url1 = root["url1"].as<String>();
	url2 = root["url2"].as<String>();
	url3 = root["url3"].as<String>();
	url4 = root["url4"].as<String>();
	url5 = root["url5"].as<String>();
	url6 = root["url6"].as<String>();
	url7 = root["url7"].as<String>();
	url8 = root["url8"].as<String>();
	url9 = root["url9"].as<String>();

	//ir_power = root["power"];
	//ir_ch_up = root["ch_up"];
	//ir_ch_dwn = root["ch_dwn"];
	//ir_vol_up = root["vol_up"];
	//ir_vol_dwn = root["vol_dwn"];
	//ir_mode = root["mode"];
	//ir_ch_0 = root["ch_0"];
	//ir_ch_1 = root["ch_1"];
	//ir_ch_2 = root["ch_2"];
	//ir_ch_3 = root["ch_3"];
	//ir_ch_4 = root["ch_4"];
	//ir_ch_5 = root["ch_5"];
	//ir_ch_6 = root["ch_6"];
	//ir_ch_7 = root["ch_7"];
	//ir_ch_8 = root["ch_8"];
	//ir_ch_9 = root["ch_9"];

	configFile.close();//��������� ����

	if (url0 == "" && url1 == "" && url2 == "") fail_save();

	return true;
}

bool loadtemp() // p������� ������ ����������� �� �����
{
	File configFile = SPIFFS.open("/temp.json", "r");// ��������� ���� ��� ������
	if (!configFile) {// ���� ���� �� ������ �������� � ����� ������ �� ���������
		vol = 100;
		ch = 0;
		savetemp();
		return false;
	}

	size_t size = configFile.size();

	if (size > 1024) {// ��������� ������ �����
		Debug.println("Size error temp!");
		return false;
	}


	jsonConfig = configFile.readString();// ��������� ���� ������������ 

	DynamicJsonDocument root(1024);
	DeserializationError error = deserializeJson(root, jsonConfig);
	if (error) {// ��������� ������ �����
		Debug.println("deserializeJson error temp!");
		return false;
	}

	ch = root["ch"];
	vol = root["vol"];
	apstart = root["ap"];
	if (root.containsKey("sleep")) isSleep = root["sleep"]; else isSleep = false;
	configFile.close();//��������� ����

	return true;
}

void StopPlaying(){ //������������ ��������� ������ 

	digitalWrite(mute, LOW);
	DEBUG_PRINT("mute LOW\n");
	radio_br = "";
	radio_name = "";

	if (mp3)
	{
		mp3->stop();
		delete mp3;
		mp3 = NULL;
	}
	if (buff)
	{
		buff->close();
		delete buff;
		buff = NULL;
	}
	if (file)
	{
		file->close();
		delete file;
		file = NULL;
	}
}

void MDCallback(void* cbData, const char* type, bool isUnicode, const char* string)
{
	const char* ptr = reinterpret_cast<const char*>(cbData);
	(void)isUnicode;
	char s1[32], s2[64];
	strncpy_P(s1, type, sizeof(s1));
	s1[sizeof(s1) - 1] = 0;
	strncpy_P(s2, string, sizeof(s2));
	s2[sizeof(s2) - 1] = 0;
	Serial.printf("METADATA(%s) '%s' = '%s'\n", ptr, s1, s2);
	Serial.flush();
	//if (String(s1) == "StreamTitle") {
	//	id3 = s2;
	//}
}

bool play(String stream)//������������ ������� ������
{
	if (stream == "") {
		Debug.println("restart stream");
	}
	else {
		url_tmp = stream;
		//save_url=url_tmp;
	}

	StopPlaying();//������������� ���������������

	file = new AudioFileSourceICYStream();
	//file = new AudioFileSourceHTTPStream();

	file->format_file("audio/mpeg");//�������� ��������� ������, ���������� ������ mpeg ������

	//file->read_name();

	if (file->open(url_tmp.c_str())) {//��������� �����, ���� ��� �� �� ���������

		//if (indicator == "Display") file->RegisterMetadataCB(MDCallback, (void*)"ICY");
		Debug.println("Open Stream " + url_tmp + " OK");
		Debug.println("Radio name: " + file->read_name());
		Debug.println("Radio bitrate: " + file->read_br());
		radio_name = file->read_name();
		radio_br = file->read_br();
		buff = new AudioFileSourceBuffer(file, 4096);//������������� �����
		mp3 = new AudioGeneratorMP3();//��������� ������� ������
		mp3->begin(buff, out);//��������� �����

		digitalWrite(mute, HIGH);
		DEBUG_PRINT("mute HIGH\n");
		return true;//���������� 1
	}
	else {//���� �� �� 
		Debug.println("Open Stream " + url_tmp + " ERR");//����� � �������� ������

		//if (indicator == "Led") digitalWrite(status_green, LOW), digitalWrite(status_red, HIGH);//������� ��������

		//status_string = "ERROR";
		//oled();
		delay(500);

		digitalWrite(mute, LOW);
		DEBUG_PRINT("mute LOW\n");
		return false;//���������� 0
	}
}

void sel_ch(bool isDec = false)//������������ ������ ������ 
{
	String url_ = "http://";
	if (ch > 9) ch = 0, url_ = url0;
	if (ch < 0) ch = 9, url_ = url9;
	if (ch == 0) url_ = url0;
	if (ch == 1) url_ = url1;
	if (ch == 2) url_ = url2;
	if (ch == 3) url_ = url3;
	if (ch == 4) url_ = url4;
	if (ch == 5) url_ = url5;
	if (ch == 6) url_ = url6;
	if (ch == 7) url_ = url7;
	if (ch == 8) url_ = url8;
	if (ch == 9) url_ = url9;
	if (url_ == "http://") {
		if (isDec) ch++; else ch--;
		sel_ch();
	}
	else stream_ok = play(url_);
}

void setup() {
	system_update_cpu_freq(SYS_CPU_160MHZ); //n�������� 160���

	Serial.begin(115200); 
	Serial.setDebugOutput(false);
	
	SPIFFS.begin(); 

	loadConfig(); 

	DEBUG_PRINT("Init hardware\n");

	keyDriver.addDevice((CKeybdDevice*)new CKeybdDigital((int16_t*)keyCodeArray_d, ARRAY_SIZE(keyCodeArray_d)));

	pinMode(mute, OUTPUT);
	digitalWrite(mute, LOW);
	DEBUG_PRINT("mute LOW\n");
	DEBUG_PRINT("Load settings\n");
	loadtemp();

	DEBUG_PRINT("\nStart ESP-12\n");
	DEBUG_PRINT("Set indicator: %s", indicator.c_str());

	if (!digitalRead(KEY_SLEEP) || apstart == 1) { 
		apstart = 0;
		savetemp();
		delay(1000);
		ap_start();

	}
	else {

		DEBUG_PRINT("Connect to %s", ssid.c_str());
		Debug.println("Connecting to " + ssid);
		WiFi.disconnect();
		WiFi.softAPdisconnect(true);
		WiFi.mode(WIFI_STA);// ����� �������
		//WiFi.setPhyMode(WIFI_PHY_MODE_11B);//�������� ������������� ����� B
		WiFi.begin(ssid.c_str(), wifipsw.c_str());

		byte tries = 10;
		while (--tries && WiFi.status() != WL_CONNECTED) //10 ��� �������� ������������
		{
			//if (indicator == "Led") digitalWrite(status_green, !digitalRead(status_green));
			Debug.print(".");
			delay(3000);
		}

		if (WiFi.status() != WL_CONNECTED) {
			DEBUG_PRINT("\nNo connecting...\n");
			delay(2000);
			ap_start(); //���� �� ���������� ������������ ��������� ����� ������� � ��� ������
		}
		Debug.println("");
		WiFi.printDiag(Debug);

		if (apstart == 0 ) {
			out = new AudioOutputI2SNoDAC();//��������� ����� ����� ����� ���
			out->SetGain(((float)vol) / 100.0);//������������� ������� �����
			if (!isSleep) {
				DEBUG_PRINT("\nStart radio\n");
				delay(1000);

				sel_ch();//��������� ��������������� ������������ ������ � ��������� ������
			}
			else DEBUG_PRINT("\nStart radio in sleep mode\n");
		}
	}
	cClock.init();
}

void loop() {
	if (apstart == 1) {

		dnsServer.processNextRequest();
		server.handleClient();

	}
	else {
		keyDriver.listenKeys(); 
		if (keyDriver.isKeyPressed()) {
			int currKey = keyDriver.getKeyCode();
			DEBUG_PRINT("KeyCode = %i\n", currKey);
			if (isSleep) {
				stream_ok = play("");
				isSleep = false;
				DEBUG_PRINT("Sleep mode OFF\n");
				temp_write = 1;
				currKey = KEY_NOCODE;
			}
			if (currKey == KEY_UP) {
				ch++;
				sel_ch(true);
				temp_write = 1;
			}
			else if (currKey == KEY_DOWN) {
				ch--;
				sel_ch();//������������ ��������� �����
				temp_write = 1;
			}
			else if (currKey == KEY_SLEEP) {
				isSleep = true;
				StopPlaying();
				stream_ok = false;
				DEBUG_PRINT("Sleep mode ON\n");
			}
		}
		if (!isSleep) {
			if (stream_ok) {//���� ����� ���������� ��
				if (mp3->isRunning()) {//��������� ��������������� ������
					if (millis() - lastms > 3000) {//��� � 3 ���
						lastms = millis();
						if (temp_write == 1) temp_write = 0, Debug.println("Save"), savetemp();//��������� ���������� �� �������� � ��������� 
						Debug.printf("Running for %d ms. Heap: %d b\n", lastms, ESP.getFreeHeap());//���������� ��������������� �� �����
						Debug.flush();
					}
					if (!mp3->loop()) mp3->stop();//���� "�������� ������" ������������� ��������������� 
					digitalWrite(mute, HIGH);
					//DEBUG_PRINT("mute HIGH\n");
				}
				else {
					stream_ok = play("");//���� ��������������� ������ ������������, �������������
				}

			}

			if (stream_ok) mp3->loop();
			//else {
			//	if (!stream_ok) stream_ok = play("");
			//	if (!stream_ok) stream_ok = play("");
			//	if (!stream_ok) {
			//		ch--;
			//		sel_ch();
			//		temp_write = 1;
			//	}
			//}
		}
	}
	cClock.run();
}
