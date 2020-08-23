/**********************************************************************************
 * When in the IDE please select the following options on the ESP8266:
 *
 * 
 * Name:		DenBubnilka.ino
 * Created:	28.05.2019 15:08:49
 * Author:	Alex
 *
 * Tools->lwIP Variant->v1.4 Open Source, or V2 Higher Bandwidth
 * Tools->CPU Frequency->160MHz
**********************************************************************************/

#include "AudioFileSourceICYStream.h"//ESP8266Audio-master_mod_espmini.zip 
#include "AudioFileSourceBuffer.h" //ESP8266Audio-master_mod_espmini.zip 
#include "AudioFileSourceSPIFFS.h" //ESP8266Audio-master_mod_espmini.zip 
#include "AudioFileSourceID3.h" 
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"
extern "C" {
#include <user_interface.h>
}

#define _DEBUG
#define PROJECT "DenBubnilka"
#define VERSION "1.5"

#include "MyArduino.h"
#include "MyWiFi.h"
#include "KR145IK1901.h"
#include "UniversalKeyboardDriver.h"

enum EKeyCodes { KEY_UP = 5, KEY_DOWN = 0, KEY_SLEEP = 4};
const int16_t keyCodeArray_d[] = { KEY_UP,  KEY_DOWN, KEY_SLEEP };
CKeyboardDriver keyDriver(1);

#define PIN_MUTE D6 //GPIO12
#define DO_MUTE digitalWrite(PIN_MUTE, LOW)
#define DO_UNMUTE digitalWrite(PIN_MUTE, HIGH)

AudioGeneratorMP3* mp3 = nullptr;
AudioFileSource* file = nullptr;
AudioFileSourceBuffer* buff = nullptr;
AudioOutputI2SNoDAC *out = nullptr;
AudioFileSourceID3 *id3 = nullptr;

String deviceID="NodeMCU";

void kuku();
CKR145IK1901 cClock(kuku);

#define TIME_DELAY_SAVE 5000
uint32_t timeToSave = 0;
uint32_t timeToRestart = 0;
bool isAlreadyRestart = false;
StaticJsonDocument<1024> config;
bool isErrConfigLoad;
bool isSleep = false;
uint8_t stationID;
uint8_t stationCount;
bool isUpshift = true;

bool goOTA(String httpAnswer){
	playFile("/startota.mp3");
	DynamicJsonDocument configOTA(2048);
	DeserializationError error = deserializeJson(configOTA, httpAnswer);
	if (error) {
		DEBUG_PRINT("DeserializeJson HTTP OTA request failed:%s!\n", error.c_str());
		return false;
	}
	if (configOTA.containsKey("SPIIFS")){
		playFile("/otaspiffs.mp3");
		if (configOTA["SPIFFS"]["format"]) SPIFFS.format();
		JsonArray arr = configOTA["SPIIFS"]["files"].as<JsonArray>();
		for (int i=0; i<arr.size(); i++) {
			HTTPClient client;
			String fileUrl = configOTA["SPIIFS"]["url"].as<String>() + arr[i].as<String>();
			String fileName = "/" + arr[i].as<String>();
			DEBUG_PRINT("Download \"%s\" from \"%s\"\n", fileName.c_str(), fileUrl.c_str());
			File f = SPIFFS.open(fileName, "w");
			if (f) {
				client.begin(fileUrl);
				int httpCode = client.GET();
				if (httpCode == HTTP_CODE_OK){
					client.writeToStream(&f);
				}
				else{				
					DEBUG_PRINT("GET... OTA SPIFFS failed, error: %s\n", client.errorToString(httpCode).c_str());
				}
				f.close();
			}
			else {
				DEBUG_PRINT("Open \"%s\" for write ERR!\n", fileName.c_str());
			}
			client.end();
		}
	}
	if (strlen(configOTA["OTA"].as<const char *>()) > 0 ){
		playFile("/otaprogmem.mp3");
		runOTA(configOTA["OTA"].as<const char *>());
	}

	
}


bool loadConfig() {
	bool ret = true;
	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile) return false;
	if (configFile.size() > 1024) {
		DEBUG_PRINT("Size error config.json!\n");
		ret = false;
	}
	else {
		DeserializationError error = deserializeJson(config, configFile.readString());
		if (error) {
			DEBUG_PRINT("DeserializeJson config.json failed:%s!\n", error.c_str());
			ret = false;
		}
		else{
			isSleep = config["Sleep"];
			stationID = config["StationID"];
			stationCount = config["Size"];
		}
		//if (root.containsKey("sleep")) isSleep = root["sleep"]; else isSleep = false;
	}	
	configFile.close();
	return ret;
}

void saveConfig(){
	config["Sleep"] = isSleep;
	config["StationID"] = stationID;

	String json;
	serializeJson(config, json);

	File configFile = SPIFFS.open("/config.json", "w");
	if (!configFile) {
		DEBUG_PRINT("Error open file for write config!");
	}
	else {
		configFile.print(json);
		configFile.close();
		DEBUG_PRINT("Save config!\n");
	}		
}

bool takeHTTPConfig(){
	bool result = false;
	HTTPClient client;
	String httpGet = "http://192.168.0.60:80/objects/?script=Radio2json";
	if (!isErrConfigLoad){
		httpGet +="&sleep=" + String(isSleep);
		httpGet +="&id=" + String(stationID);
	}
	client.begin(httpGet);
	if (client.GET() > 0) {
		String httpAnswer = client.getString();
		if ( httpAnswer.indexOf("OTA=") >= 0 ) {
			result = goOTA(httpAnswer);
		}
		DeserializationError error = deserializeJson(config, httpAnswer);
		if (error) {
			DEBUG_PRINT("DeserializeJson HTTP request failed:%s!\n", error.c_str());
		}
		else{
			isSleep = config["Sleep"];
			stationID = config["StationID"];
			stationCount = config["Size"];
			saveConfig();
			result = true;
		}
	}
	else {
		DEBUG_PRINT("HTTP GET failed!\n");	
	}
	client.end();
	return result;
}

// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void* cbData, const char* type, bool isUnicode, const char* string){
	const char* ptr = reinterpret_cast<const char*>(cbData);
	(void)isUnicode; // Punt this ball for now
	// Note that the type and string may be in PROGMEM, so copy them to RAM for printf
	char s1[32], s2[64];
	strncpy_P(s1, type, sizeof(s1));
	s1[sizeof(s1) - 1] = 0;
	strncpy_P(s2, string, sizeof(s2));
	s2[sizeof(s2) - 1] = 0;
	DEBUG_PRINT("METADATA(%s) '%s' = '%s'\n", ptr, s1, s2);
}

// Called when there's a warning or error (like a buffer underflow or decode hiccup)
void StatusCallback(void* cbData, int code, const char* string){
	return;
	const char* ptr = reinterpret_cast<const char*>(cbData);
	// Note that the string may be in PROGMEM, so copy it to RAM for printf
	char s1[64];
	strncpy_P(s1, string, sizeof(s1));
	s1[sizeof(s1) - 1] = 0;
	DEBUG_PRINT("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
	//Serial.flush();
}

void clearAudioPtr(){
	if (mp3) { delete mp3; mp3 = nullptr; }
	if (out) { delete out; out = nullptr; }
	if (buff) { delete buff; buff = nullptr; }
	if (id3) { delete id3; id3 = nullptr; }
	if (file) { delete file; file = nullptr; }
}

void playFile(const char *fileName){
	clearAudioPtr();
	if (!SPIFFS.exists(fileName)) {
		DEBUG_PRINT("playFile \"%s\" ERR!", fileName);
		return;
	}
	file = new AudioFileSourceSPIFFS(fileName);
	id3 = new AudioFileSourceID3(file);
	id3->RegisterMetadataCB(MDCallback, (void *)"ID3TAG");
	out = new AudioOutputI2SNoDAC();
	mp3 = new AudioGeneratorMP3();
	mp3->begin(id3, out);
	DO_UNMUTE;
	while (mp3->isRunning()) {
		if (!mp3->loop()) mp3->stop();
		yield();
	}
	DO_MUTE;
}

void playStream(const char *streamPath){
	clearAudioPtr();	
	file = new AudioFileSourceICYStream();
	//file = new AudioFileSourceICYStream(streamPath);
	file->RegisterMetadataCB(MDCallback, (void*)"ICY");
	((AudioFileSourceICYStream *)file)->format_file("audio/mpeg");
	if (file->open(streamPath)) {
		buff = new AudioFileSourceBuffer(file, 2048);
		buff->RegisterStatusCB(StatusCallback, (void*)"buffer");
		out = new AudioOutputI2SNoDAC();
		mp3 = new AudioGeneratorMP3();
		mp3->RegisterStatusCB(StatusCallback, (void*)"mp3");
		mp3->begin(buff, out);
		DO_UNMUTE;
	}
	else {
		DEBUG_PRINT("Err open stream!");
	}
}

void playStream(){
	const char *sID =  config["Stations"][stationID]["ID"];
	DEBUG_PRINT("Select channel %i(%s) ==> %s\n", stationID, sID, config["Stations"][stationID]["stations"].as<const char*>());
	playFile((String("/") + sID + String(".mp3")).c_str());
	playStream(config["Stations"][stationID]["stations"].as<const char*>());
}

void selectStation(int8_t num){
	if (mp3 && mp3->isRunning()) {
		DO_MUTE;
		mp3->stop();
	}	
	if (num < 0){
		if (stationID == 0) stationID = stationCount - 1; else stationID--;
	}
	else if (stationID == stationCount - 1) stationID = 0; else stationID++;
	timeToSave = millis() + TIME_DELAY_SAVE;
	isAlreadyRestart = false;
	timeToRestart = 0;
	playStream();
}

inline void nextStation(){
	selectStation(1);
}

inline void prevStation(){
	selectStation(-1);
}

void kuku(){
	if (isSleep) return;
	if (mp3 && mp3->isRunning()) {
		DO_MUTE;
		mp3->stop();
	}
	playFile("/kuku.mp3");
	playStream(config["Stations"][stationID]["stations"].as<const char*>());
	//playStream();
}

void setup(){
	system_update_cpu_freq(SYS_CPU_160MHZ);
	Serial.begin(115200);
	pinMode(PIN_MUTE, OUTPUT);
	DO_MUTE;
	DEBUG_PRINT("\nStart...\n\n");

	SPIFFS.begin();
	
	keyDriver.addDevice((CKeybdDevice*)new CKeybdDigital((int16_t*)keyCodeArray_d, ARRAY_SIZE(keyCodeArray_d)));

	cClock.init();
	
	setupWiFi();

	playFile("/start.mp3");

	isErrConfigLoad = (!loadConfig() || !digitalRead(KEY_SLEEP));
#ifdef _DEBUG
	if (!isErrConfigLoad) serializeJsonPretty(config, Serial);
#endif

	if (upWiFi()) {
		if (isErrConfigLoad) {
			playFile("/loadconfig.mp3");
			isErrConfigLoad = !takeHTTPConfig();
		}
		if (isErrConfigLoad) { 
			playFile("/errconfig.mp3");
		}
		else {
			if (!isSleep) playStream();
		}
	}
	else {
		DEBUG_PRINT("No WiFi!");
		playFile("/nowifi.mp3");
	}
	DEBUG_PRINT("\nPlay, unmute...\n\n");
}

void loop(){
	static int lastms = 0;

	if (!isSleep) {
		if (mp3 && mp3->isRunning()) {
			if (millis() - lastms > 5000) {
				lastms = millis();
				DEBUG_PRINT("Running for %d ms...\n", lastms);
			}
			if (!mp3->loop()) { 
				DO_MUTE; 
				mp3->stop(); 
			}
			else isAlreadyRestart = false;
		}
		else {
			DEBUG_PRINT("MP3 stop or global error\n");
			if (upWiFi()) {
				if (isErrConfigLoad) {
					isErrConfigLoad = !takeHTTPConfig();
				}
				if (!isErrConfigLoad) { 
					if (isAlreadyRestart) {
						playFile("/error.mp3");
						if (isUpshift) nextStation(); else prevStation();
					}
					else {
						if (timeToRestart == 0) timeToRestart = millis() + 2000;
					}
				}
			}
		}
	}
	
	if (timeToRestart > 0 && timeToRestart < millis()){
		//playFile("/restart.mp3");
		//playStream();
		playStream(config["Stations"][stationID]["stations"].as<const char*>());
		isAlreadyRestart = true;
		timeToRestart = 0;
	}
	if (timeToSave > 0 && timeToSave < millis()){
		saveConfig();
		timeToSave = 0;
	}
	keyDriver.listenKeys(); 
	if (keyDriver.isKeyPressed()) {
		bool lpress;
		int currKey = keyDriver.getKeyCode(&lpress);
		DEBUG_PRINT("KeyCode = %i LP=%i\n", currKey, lpress);
		switch (currKey){
		case KEY_UP: 	nextStation(); isSleep = false; isUpshift = true; break;
		case KEY_DOWN: 	prevStation(); isSleep = false; isUpshift = false; break;
		case KEY_SLEEP: if (lpress) {
							DO_MUTE;
							if (mp3 && mp3->isRunning()) mp3->stop();
							playFile("/loadconfig.mp3");
							isErrConfigLoad = !takeHTTPConfig();
							if (isErrConfigLoad) { 
								playFile("/errconfig.mp3");
								isErrConfigLoad = !loadConfig();
							}
							if (!isErrConfigLoad && !isSleep){
								playStream();
							}
						}
						else {
							if (!isSleep) {
								DO_MUTE;
								if (mp3 && mp3->isRunning()) mp3->stop();
								playFile("/stop.mp3");
								isSleep = true;
								timeToSave = millis() + TIME_DELAY_SAVE;
							}
							else{
								isSleep = false;
								playStream();
								timeToSave = millis() + TIME_DELAY_SAVE;
							}
						}
		}
	}
	cClock.run();
	yield();
}
