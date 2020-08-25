// 
// 
// 

//#define _DEBUG
#include "MyArduino.h"
#include "KR145IK1901.h"
#include <ESP8266WiFi.h>
extern "C" {
#include "sntp.h"
}
#include <TimeLib.h>

bool CKR145IK1901::process(){
	if (!startProcessCorrection && timeSettings.hours == 0 && timeSettings.minutes == 0) return false;

	const uint32_t t = millis();
	static uint8_t pin;
	static uint8_t state;
	static uint32_t processTime= 0;

	if (processTime == 0) {
		if (startProcessCorrection) pin = pinCorrection;
		else if (timeSettings.hours) pin = pinHours;
		else pin = pinMinutes;

		digitalWrite(pin, 1);
		processTime = t + MIN_SET_PUSH;
		state = 1;
		DEBUG_PRINT("%s [%i:%i] begin (set pin %i to %i)...", pin == pinCorrection ? "correction" : "time", timeSettings.hours, timeSettings.minutes, pin, state);
	}

	if (t >= processTime){
		if (state) {
			digitalWrite(pin, 0);
			processTime = t + MIN_SET_PUSH;
			state = 0;
			DEBUG_PRINT("set pin %i to %i...", pin, state);
		}
		else {
			processTime = 0;
			if (pin == pinHours) timeSettings.hours--;
			else if (pin == pinMinutes) timeSettings.minutes--;
			else startProcessCorrection = false;
			DEBUG_PRINT("%s end.\n", pin == pinCorrection ? "correction" : "");
		}
	}
	return true;
}

void CKR145IK1901::init(){
	pinMode(pinCorrection, OUTPUT);
	digitalWrite(pinCorrection, HIGH);
	pinMode(pinHours, OUTPUT);
	pinMode(pinMinutes, OUTPUT);
}

void CKR145IK1901::run(){
	if (!process()) {
		if (WiFi.status() == WL_CONNECTED) {
			CKR145IK1901::syncTime();
		}
		if (now() > 1514764800L) {
			if (isResetClock) {
				digitalWrite(pinCorrection, LOW);
				uint32_t newTime = now() + ((hour() + minute()) * MIN_SET_PUSH * 2) / 1000;
				DEBUG_PRINT("Set time!\n");
				resetMinute();
				setMinutes(minute(newTime));
				setHours(hour(newTime));
				isResetClock = false;
			}
			else {
				if (minute() == 0 && second() == 30 && isNeedCorrection) {
					DEBUG_PRINT("Correction!\n");
					resetMinute();
					if (gongF) (*gongF)();
					isNeedCorrection = false;
				}
				if (minute() == 1 && !isNeedCorrection) isNeedCorrection = true;
			}
		}		
	}
}

void CKR145IK1901::syncTime() {
	static uint32_t newSyncTime = 0;
	if (!newSyncTime) {
		configTime(3 * 3600, 0, NTP_SERVER);
		newSyncTime = millis() + 10000L;
	}
	else if (newSyncTime < millis()) {
		uint32_t tm = sntp_get_current_timestamp();
		if (tm > 1514764800L) {
			setTime(tm + 3 * 3600 );
			DEBUG_PRINT("Timestamp: %i.%i.%i %02i:%02i:%02i\n", day(), month(), year(), hour(), minute(), second());
			newSyncTime = millis() + 3600000L;
		}
		else {
			DEBUG_PRINT("TimeStatus = %i\n", timeStatus());
			newSyncTime = millis() + 10000L;
		}
	}
}
