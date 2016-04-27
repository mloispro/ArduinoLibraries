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
#include "RTCExt.h"
//#include "RunScheduleExt.h" <- only needed for non RTC Timer
//#include "PinValMemoryExt.h"
#include "AnalogSwitch.h"
using namespace Utils;


class ServoMotor {
private:
	int _pin;
	int _pos;
	void Init();
	void TranslateSpeed();
	bool ShouldSignalRelay();
	int GetNextRunInSeconds();
	bool IsSwitchOn(bool isTimeToRun);
	void RunServo();
protected:
	int _theSpeed;
	AccessoryType ServoType;

	ServoMotor(Servo servo, int pin, int shakes, long runEverySeconds);
	ServoMotor(Servo servo, int pin, int shakes, short relayPin);
	ServoMotor(Servo servo, int pin, int shakes);
	ServoMotor(Servo servo, int pin, int shakes, short relayPin, long runEverySeconds);
	ServoMotor(Servo servo, int pin, int shakes, long runEverySeconds, AnalogSwitch theSwitch);
	ServoMotor(Servo servo, int pin, int shakes, short relayPin, long runEverySeconds, AnalogSwitch theSwitch);

	ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed);
	ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed, short relayPin);
	ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed, short relayPin, long runEverySeconds);
	ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed, short relayPin, long runEverySeconds, AnalogSwitch theSwitch);
	ServoMotor();
	
	static int GetNumberOfShakes(int potVal);
	void Run();
	static void RunMotorDemos(vector<ServoMotor> motors);
	static void RunMotorDemo(Servo myServo);
public:
	Servo TheServo;
	AnalogSwitch TheSwitch;
	int Shakes;
	long RunEverySeconds;
	short RelayPin;
	//static RunTime Schedule;

	bool ShouldRunMotor(bool printToSerial);
	void PrintSerialInstructions();
	int SetShakes(int shakes);
	int GetShakes();
	bool ProcessSerialInput(int incomingNum);

	//3-6 = 6 shakes, returns true if could process successfully
	static bool ProcessSerialInput(int incomingNum, vector<ServoMotor> motors);
	
	//1 to run, 2 to run demo
	static bool ShouldRunMotorBySerialInput(int incomingByte);
	static bool ShouldRunMotorDemo(int incomingByte);

};

#endif

