// Motor.h

#ifndef _SERVOMOTOR_h
#define _SERVOMOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
	
#else
	#include "WProgram.h"
#endif

#include "StandardCplusplus.h"
#include "vector"
using namespace std;

#include <Servo.h>

#include "SerialExt.h"
#include "TimerExt.h"
#include "AnalogSwitch.h"
using namespace Utils;

class ServoMotor {
private:
	int _pin;
	int _pos;
	int _theSpeed;
	void Init();
	void TranslateSpeed();
	bool ShouldSignalRelay();
	int GetNextRunInSeconds();
	bool IsTimeToRun();

protected:
	
	ServoMotor(Servo servo, int pin, int shakes, int runEverySeconds);
	ServoMotor(Servo servo, int pin, int shakes, short relayPin);
	ServoMotor(Servo servo, int pin, int shakes);
	ServoMotor(Servo servo, int pin, int shakes, short relayPin, int runEverySeconds);
	ServoMotor(Servo servo, int pin, int shakes, short relayPin, int runEverySeconds, AnalogSwitch theSwitch);

	ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed);
	ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed, short relayPin);
	ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed, short relayPin, int runEverySeconds);
	ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed, short relayPin, int runEverySeconds, AnalogSwitch theSwitch);
	
	static int GetNumberOfShakes(int potVal);
	void Run();
	static void RunMotorDemos(vector<ServoMotor> motors);
	static void RunMotorDemo(Servo myServo);
public:
	Servo TheServo;
	AnalogSwitch TheSwitch;
	int Shakes;
	int LastRunInSeconds;
	int RunEverySeconds;
	int NextRunInSeconds;
	short RelayPin;
	
	//static RunTime Schedule;

	bool ShouldRunMotor();

	//1 to run, 2 to run demo
	static bool ShouldRunMotorBySerialInput(int incomingByte);
	static bool ShouldRunMotorDemo(int incomingByte);
};

#endif

