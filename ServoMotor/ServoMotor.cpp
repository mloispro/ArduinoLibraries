
#include "ServoMotor.h"

ServoMotor::ServoMotor(Servo servo, int pin, int shakes) :
	ServoMotor(servo, pin, shakes, 0, 14){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, int runEverySeconds) :
	ServoMotor(servo, pin, shakes, 0, 14, 0, runEverySeconds){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, short relayPin, int runEverySeconds, AnalogSwitch theSwitch) :
	ServoMotor(servo, pin, shakes, 0, 14, relayPin, runEverySeconds, theSwitch){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed) :
	ServoMotor(servo, pin, shakes, pos, theSpeed, 0){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed, short relayPin) :
	ServoMotor(servo, pin, shakes, 0, 14, relayPin, 0){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, short relayPin) :
	ServoMotor(servo, pin, shakes, 0, 14, relayPin){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed, short relayPin, int runEverySeconds, AnalogSwitch theSwitch) :
TheServo(servo), _pin(pin), Shakes(shakes), _pos(pos), _theSpeed(theSpeed), RelayPin(relayPin), RunEverySeconds(runEverySeconds), TheSwitch(theSwitch){
		TranslateSpeed();
		Init();
	};


void ServoMotor::Init(){
	//TranslateSpeed();
	//ServoMotor motor;
	pinMode(RelayPin, OUTPUT);
	//TimerExt::Init();

	Serial.println("Press - 1 to run, 2 to go back and forth..");

	Serial.print("Translated Speed: ");
	Serial.println(_theSpeed);

	Serial.print("Shake while feeding: ");
	Serial.println(Shakes);

	SerialExt::Print("Servo attached to Pin: ", _pin);
	TheServo.attach(_pin);

}

void ServoMotor::Run(){
	bool signalRelay = ShouldSignalRelay();
	SerialExt::Debug("ShouldSignalRelay: ", signalRelay);
	
	if (signalRelay){
		SerialExt::Print("Signaling Relay Pin: ", RelayPin);
		digitalWrite(RelayPin, HIGH);
	}
	
	TheServo.detach();
	while (!TheServo.attached()){ //wait until servo is attached.
		TheServo.attach(_pin);
		SerialExt::Print("Servo Attached: ", TheServo.attached());
		delay(100);
	}

	TheServo.write(_pos);

	int downAngle = 200;
	//int feedAngle = 180 - _pos;
	//_utils.Debug("FeedAngle: ", feedAngle);
	for (int downPos = _pos; downPos <= downAngle; downPos += 1) { // goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		TheServo.write(downPos);              // tell servo to go to position in variable 'pos'
		delay(_theSpeed);                 // waits 15ms for the servo to reach the position
	}
	if (Shakes > 0){
		SerialExt::Print("Shaking: ", Shakes, " Times");
		//delay(4000);
		int shakeCount = 0;
		while (shakeCount < Shakes){
			TheServo.write(downAngle - 120);
			delay(220);
			TheServo.write(downAngle);
			delay(220);

			shakeCount++;
		}
	}
	SerialExt::Print("Resetting Servo Angle: ", _pos);
	TheServo.write(_pos);
	delay(2000); // wait for servo to get back to 0
	TheServo.detach();
	delay(1000);
	
	if (signalRelay)
		digitalWrite(RelayPin, LOW);

	TimerExt::TheRunSchedule.LastRunInSeconds = TimerExt::GetRuntimeInSeconds();
}

int ServoMotor::GetNumberOfShakes(int potVal){
	//_utils.Debug("In val: ", val);
	int shakesVal = map(potVal, 0, 1023, 0, 10); // scale it to use it with the servo (value between 0 and 180)
	return shakesVal;

}
bool ServoMotor::ShouldSignalRelay(){
	if (RelayPin >= 2 && RelayPin <= 13) 
		return true;
	return false;
}
int ServoMotor::GetNextRunInSeconds(){
	
	RunSchedule schedule = TimerExt::GetRunSchedule(_pin);
	int runTime = TimerExt::GetRuntimeInSeconds();

	//Get from EEPROM
	LastRunInSeconds = TimerExt::TheRunSchedule.LastRunInSeconds;
	NextRunInSeconds = TimerExt::TheRunSchedule.NextRunInSeconds;
	
	SerialExt::Debug("saved LastRunInSeconds: ", LastRunInSeconds);
	SerialExt::Debug("saved NextRunInSeconds: ", NextRunInSeconds);


	SerialExt::Debug("runTime: ", runTime);
	if (runTime < 30){ //00:00:00
		int difference = NextRunInSeconds - LastRunInSeconds; //00:00:10
		SerialExt::Debug("difference: ", difference);
		NextRunInSeconds = runTime + difference; //00:00:10
		LastRunInSeconds = 0;
		
	}
	else{
		NextRunInSeconds = LastRunInSeconds + RunEverySeconds;
	}
	SerialExt::Print("Last Run: ", TimerExt::GetDigitalTimeFromSeconds(LastRunInSeconds));
	SerialExt::Print("Next Run: ", TimerExt::GetDigitalTimeFromSeconds(NextRunInSeconds));

	//Save to EEPROM
	TimerExt::TheRunSchedule.LastRunInSeconds = LastRunInSeconds;
	TimerExt::TheRunSchedule.NextRunInSeconds = NextRunInSeconds;

	//Save(LastRunInSeconds, NextRunInSeconds);
	return NextRunInSeconds;
}
bool ServoMotor::IsTimeToRun(){
	int nextRun = GetNextRunInSeconds();
	int runTime = TimerExt::GetRuntimeInSeconds();
	if (nextRun <= runTime){
		return true;
	}
	return false;
}
void ServoMotor::RunMotorDemo(Servo myServo){
	Serial.println("Demoing motor..");
	int pos = 0;
	for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		myServo.write(pos);              // tell servo to go to position in variable 'pos'
		delay(5);                       // waits 15ms for the servo to reach the position
	}
	for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
		myServo.write(pos);              // tell servo to go to position in variable 'pos'
		delay(10);                       // waits 15ms for the servo to reach the position
	}
}
void ServoMotor::RunMotorDemos(vector<ServoMotor> motors){

	for (int thisMotor = 0; thisMotor < motors.size(); thisMotor++) {
		ServoMotor motor = motors[thisMotor];
		Servo aServo = motor.TheServo;

		ServoMotor::RunMotorDemo(aServo);

	}
}
//1 to run, 2 to run demo
bool ServoMotor::ShouldRunMotorBySerialInput(int incomingByte){

	switch (incomingByte) {
	case 49: //1
		return true;
	default:
		return false;
	}
}
bool ServoMotor::ShouldRunMotor()
{
	bool runMotor;
	bool isTimeToRun = IsTimeToRun();
	SerialExt::Debug("Is Time To Run: ", isTimeToRun);

	bool isSwitchOn;
	//see if this motor has a switch
	if (TheSwitch.AnalogPin >= 0){

		if (isTimeToRun)
			isSwitchOn = TheSwitch.IsOn();

		if (isSwitchOn)
			SerialExt::Debug("Switch Val: ", TheSwitch.SwitchReading);
	}
	else{
		isSwitchOn = true; //no switch to turn it on.
	}

	runMotor = (isTimeToRun) && (isSwitchOn);
	return runMotor;
}
//1 to run, 2 to run demo
bool ServoMotor::ShouldRunMotorDemo(int incomingByte){

	switch (incomingByte) {
	case 50: //2
		return true;
	default:
		return false;
	}
}

void ServoMotor::TranslateSpeed(){
	if (_theSpeed >= 14){
		_theSpeed = 2;
		return;
	}
	switch (_theSpeed) {
	case 13:
		_theSpeed = 3;
		break;
	case 12:
		_theSpeed = 4;
		break;
	case 11:
		_theSpeed = 5;
		break;
	case 10:
		_theSpeed = 6;
		break;
	case 9:
		_theSpeed = 7;
		break;
	case 8:
		_theSpeed = 8;
		break;
	case 7:
		_theSpeed = 9;
		break;
	case 6:
		_theSpeed = 10;
		break;
	case 5:
		_theSpeed = 11;
		break;
	case 4:
		_theSpeed = 12;
		break;
	case 3:
		_theSpeed = 13;
		break;
	case 2:
		_theSpeed = 14;
		break;
	case 1:
		_theSpeed = 15;
		break;
	default:
		break;
	}
}

