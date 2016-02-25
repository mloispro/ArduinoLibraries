
#include "FishFeeder.h"

//FishFeeder::FishFeeder(int pin) : ServoMotor(pin){};
FishFeeder::FishFeeder(Servo servo, int pin, int shakes) : ServoMotor(servo, pin, shakes){};

vector<FishFeeder> FishFeeder::CreateFeeders(int numOfFeeders, int startingPin){
	vector<FishFeeder> feeders;

	if (startingPin > 13 || startingPin < 2) {
		Serial.println("Error: Starting pin has to be less than 14 and greater than 1");
		return feeders;
	}
	if (numOfFeeders > 11) {
		Serial.println("Error: Number of feeders has to be less than 12");
		return feeders;
	}
	
	for (int thisFeeder = 0; thisFeeder < numOfFeeders; thisFeeder++) {
		if (startingPin <= 1) return feeders; //pin 1 and 0 cant be outputs

		// twelve servo objects can be created on most boards
		//FishFeeder feeder(Servo(), 13, 2, 0, 14); //servo, pin, shakes, pos, speed
		Servo aServo;
		FishFeeder feeder(aServo, startingPin, 2);
		//feeder.Init();
		feeders.push_back(feeder);
		startingPin--;
	}
	return feeders;
}

void FishFeeder::FeedAll(vector<FishFeeder> feeders, int potVal){
	int shakesVal = GetNumberOfShakes(potVal);
	//digitalWrite(FishFeeder::FeederPwrSigPin, LOW); //to send relay signal
	for (int thisFeeder = 0; thisFeeder < feeders.size(); thisFeeder++) {
		FishFeeder feeder = feeders[thisFeeder];
		//feeder.Shakes = shakesVal; //not setting from pot right now.

		SerialExt::Print("Feeder #", thisFeeder + 1, " Feeding..");
		feeder.Feed();
	}
	//digitalWrite(FishFeeder::FeederPwrSigPin, HIGH);
}


void FishFeeder::Feed(){

	Run();
}

void FishFeeder::RunDemo(vector<FishFeeder> feeders){

	vector<ServoMotor> motors;
	copy(feeders.begin(), feeders.end(), back_inserter(motors));
	ServoMotor::RunMotorDemos(motors);

}
