// KR145IK1901.h

#ifndef _KR145IK1901_h
#define _KR145IK1901_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define NTP_SERVER "192.168.0.60"
#define MIN_SET_PUSH 500

class CKR145IK1901 {
	struct STimeSetting {
		uint8_t minutes;
		uint8_t hours;
	} timeSettings;

	uint8_t pinCorrection = D6;
	uint8_t pinMinutes = D5;
	uint8_t pinHours = D7;

	bool isResetClock = true;
	bool isNeedCorrection = true;
	bool startProcessCorrection = false;

	void (*gongF)();

	void syncTime();

	void resetMinute() { startProcessCorrection = true;}

	void setHours(int h) { timeSettings.hours = h; }

	void setMinutes(int m) { timeSettings.minutes = m; }

	bool process();

public:
	CKR145IK1901(void (*gongF_)() = nullptr) : gongF(gongF_){}
	void init();
	void run();
};

#endif
