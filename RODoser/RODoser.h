// RODoser.h

#ifndef _RODOSER_h
#define _RODOSER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "Servo.h"
#include "ServoMotor.h"

#include "StandardCplusplus.h"
#include "vector"

#include "SerialExt.h"
#include "AnalogSwitch.h"
using namespace Utils;

using namespace std;


class RODoser : public ServoMotor {
private:

public:
	RODoser(Servo servo, int pin, int shakes, short relayPin, long runEverySeconds, AnalogSwitch theSwitch);
	RODoser(Servo servo, int pin, int shakes, long runEverySeconds, AnalogSwitch theSwitch);
	RODoser();
	static void RunDemo(vector<RODoser> dosers);
	void Dose();
};

//vector<RODoser> RODoser::Feeders;

//extern RODoserClass RODoser;

#endif

