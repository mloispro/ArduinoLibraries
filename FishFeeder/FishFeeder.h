// FishFeeder.h

#ifndef _FISHFEEDER_h
#define _FISHFEEDER_h

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
using namespace Utils;

using namespace std;


class FishFeeder : public ServoMotor {

private:
	 void Feed();

public:
	FishFeeder();
	FishFeeder(Servo servo, int pin, int shakes, long runEverySeconds);
	FishFeeder(Servo servo, int pin, int shakes, short relayPin, long runEverySeconds);
	 static vector<FishFeeder> CreateFeeders(int numOfFeeders, int startingPin);
	 static void FeedAll(vector<FishFeeder> feeders, int potVal);
	 static void RunDemo(vector<FishFeeder> feeders);
	 static FishFeeder CreateFeeder(int pin, short shakes);
	 static void FeedAll(vector<FishFeeder> feeders);
};

//vector<FishFeeder> FishFeeder::Feeders;

//extern FishFeederClass FishFeeder;

#endif

