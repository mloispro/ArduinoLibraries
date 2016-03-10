
#include "ServoMotor.h"

ServoMotor::ServoMotor(Servo servo, int pin, int shakes) :
	ServoMotor(servo, pin, shakes, 0, 14){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, long runEverySeconds) :
	ServoMotor(servo, pin, shakes, 0, 14, 0, runEverySeconds){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, short relayPin, long runEverySeconds, AnalogSwitch theSwitch) :
	ServoMotor(servo, pin, shakes, 0, 14, relayPin, runEverySeconds, theSwitch){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed) :
	ServoMotor(servo, pin, shakes, pos, theSpeed, 0){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed, short relayPin) :
	ServoMotor(servo, pin, shakes, 0, 14, relayPin, 0){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, short relayPin) :
	ServoMotor(servo, pin, shakes, 0, 14, relayPin){};

ServoMotor::ServoMotor(Servo servo, int pin, int shakes, int pos, int theSpeed, short relayPin, long runEverySeconds, AnalogSwitch theSwitch) :
TheServo(servo), _pin(pin), Shakes(shakes), _pos(pos), _theSpeed(theSpeed), RelayPin(relayPin), RunEverySeconds(runEverySeconds), TheSwitch(theSwitch){
	TranslateSpeed();
	Init();
};
ServoMotor::ServoMotor(){};

int ServoMotor::SetShakes(int shakes){
	//PinValMemoryExt::SavePinVal(_pin, shakes);
	PinValMemoryExt::ThePinValMemory.Pin = _pin;
	PinValMemoryExt::ThePinValMemory.Value = shakes;

	int savedShakes = PinValMemoryExt::ThePinValMemory.Value;
	//SerialExt::Debug("shakes: ", savedShakes);
	Shakes = savedShakes;
	//SerialExt::Debug("Shakes: ", Shakes);
}
int ServoMotor::GetShakes(){
	int shakes = PinValMemoryExt::ThePinValMemory.Value;
	//SerialExt::Debug("get shakes: ", shakes);
	if (shakes > -1){
		Shakes = shakes;
	}

	return Shakes;
}

void ServoMotor::Init(){
	//TranslateSpeed();
	//ServoMotor motor;
	pinMode(RelayPin, OUTPUT);

	if (RunEverySeconds > 0){
		RTCExt::Init(); //using rtc
		NextRunInSeconds = RTCExt::GetRTCTime() + RunEverySeconds;
		if (LastRunInSeconds > NextRunInSeconds)
			LastRunInSeconds = RTCExt::GetRTCTime();
	}
		
	TheServo.attach(_pin);

}
void ServoMotor::PrintSerialInstructions(){
	Serial.println("1 to Run, 2 to Demo.");
	Serial.println("3-# to set Shakes, ex: 3-4.");

	Serial.print("Translated Speed: ");
	Serial.println(_theSpeed);

	Serial.print("Shake while feeding: ");
	Serial.println(GetShakes());

	SerialExt::Print("Servo attached to Pin: ", _pin);
}
void ServoMotor::Run(){
	bool signalRelay = ShouldSignalRelay();
	//SerialExt::Debug("ShouldSignalRelay: ", signalRelay);
	
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
	if (GetShakes() > 0){
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
	//SerialExt::Print("Resetting Servo Angle: ", _pos);
	TheServo.write(_pos);
	delay(2000); // wait for servo to get back to 0
	TheServo.detach();
	delay(1000);
	
	if (signalRelay)
		digitalWrite(RelayPin, LOW);

	//LastRunInSeconds = TimerExt::GetRuntimeInSeconds();
	//RunScheduleExt::SaveTheRun(_pin, RunEverySeconds, LastRunInSeconds);
	
	if (RunEverySeconds>0)
		LastRunInSeconds = RTCExt::GetRTCTime(); //using rtc
	
	
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
bool ServoMotor::ShouldRunMotor(bool printToSerial)
{
	bool runMotor;
	bool isTimeToRun = IsTimeToRun();
	//SerialExt::Debug("Is Time To Run: ", isTimeToRun);

	if (printToSerial && RunEverySeconds>0) //using rtc
	{
		SerialExt::Print("Time: ", RTCExt::GetShortDateTimeString(RTCExt::GetRTCTime()));
		SerialExt::Print("Run Count Down: ", RTCExt::GetDigitalTimeString(RunCountDownInSeconds));
		SerialExt::Print("Next Run: ", RTCExt::GetShortDateTimeString(NextRunInSeconds));
		SerialExt::Print("Last Run: ", RTCExt::GetShortDateTimeString(LastRunInSeconds));
		SerialExt::Print("Run Every: ", RTCExt::GetDigitalTimeString(RunEverySeconds));
	}

	bool isSwitchOn;
	//see if this motor has a switch
	if (TheSwitch.AnalogPin >= 0){

		if (isTimeToRun)
			isSwitchOn = TheSwitch.IsOn();

		//if (isSwitchOn)
			//SerialExt::Debug("Switch Val: ", TheSwitch.SwitchReading);
	}
	else{
		isSwitchOn = true; //no switch to turn it on.
	}

	runMotor = (isTimeToRun) && (isSwitchOn);
	return runMotor;
}

bool ServoMotor::IsTimeToRun(){

	if (RunEverySeconds <= 0)return true; //not using rtc

	time_t runTime = RTCExt::GetRTCTime();
	time_t nextRun = GetNextRunInSeconds(runTime);
	//int runTime = TimerExt::GetRuntimeInSeconds();
	if (nextRun <= runTime){
		return true;
	}
	return false;
}

time_t ServoMotor::GetNextRunInSeconds(time_t runTime){

	if (NextRunInSeconds <= 0)
		NextRunInSeconds = runTime + RunEverySeconds; //08:51:49

	if (LastRunInSeconds > 0 && NextRunInSeconds <= runTime){
		NextRunInSeconds = LastRunInSeconds + RunEverySeconds;
	}
	RunCountDownInSeconds = NextRunInSeconds - runTime; //00:00:06

	if (NextRunInSeconds <= runTime){
		NextRunInSeconds = runTime;
		RunCountDownInSeconds = 0;
	}

	//if (NextRunInSeconds <= 0){
	//	NextRunInSeconds = runTime;
	//}
	//if (LastRunInSeconds <= 0){
	//	NextRunInSeconds = NextRunInSeconds + RunEverySeconds;
	//}
	//else{
	//	NextRunInSeconds = LastRunInSeconds + RunEverySeconds;
	//}
	//
	//RunCountDownInSeconds = runTime - NextRunInSeconds;

	return NextRunInSeconds;
}

//not complete, doesnt handle restart or power outage.
//int ServoMotor::GetNextRunInSeconds(){
//	
//	RunSchedule schedule = RunScheduleExt::GetRunSchedule(_pin);
//	int runTime = TimerExt::GetRuntimeInSeconds();
//	int nextRunCountDown = schedule.NextRunCountDown;
//
//	int elapsedTime = runTime - schedule.LastRunInSeconds;
//
//	if (nextRunCountDown <= 0 && schedule.LastRunInSeconds <= 0){ //first run
//		NextRunInSeconds = RunEverySeconds - runTime;
//		nextRunCountDown = NextRunInSeconds;
//		
//		//return nextRun;
//	}
//	else if (elapsedTime > NextRunInSeconds){
//		nextRunCountDown = 0;
//		NextRunInSeconds = runTime;
//	}
//	else if (NextRunInSeconds > runTime){
//		LastRunInSeconds = 0;
//		nextRunCountDown = nextRunCountDown - runTime;
//		NextRunInSeconds = runTime + schedule.NextRunCountDown;
//	}
//	else{
//		if (LastRunInSeconds <= 0){ //restart, or countdown expired
//			nextRunCountDown = nextRunCountDown - runTime;
//		}
//		else{
//			nextRunCountDown = RunEverySeconds - elapsedTime;
//			NextRunInSeconds = LastRunInSeconds + RunEverySeconds;
//		}
//
//	}
//	if (nextRunCountDown <= 0){ //needs to run set to 0
//		nextRunCountDown = 0;
//		NextRunInSeconds = runTime;
//	}
//	
//	SerialExt::Print("Last Run: ", TimerExt::GetDigitalTimeFromSeconds(LastRunInSeconds));
//	SerialExt::Print("Next Run: ", TimerExt::GetDigitalTimeFromSeconds(NextRunInSeconds));
//
//	//Save to EEPROM
//	//schedule.LastRunInSeconds = LastRunInSeconds;
//	schedule.NextRunCountDown = nextRunCountDown;
//
//	SerialExt::Debug("nextRunCountDown: ", TimerExt::GetDigitalTimeFromSeconds(nextRunCountDown));
//	SerialExt::Debug("NextRunCountDown: ", TimerExt::GetDigitalTimeFromSeconds(schedule.NextRunCountDown));
//
//	return NextRunInSeconds;
//}

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
	case 1: //1
		return true;
	default:
		return false;
	}
}

//1 to run, 2 to run demo
bool ServoMotor::ShouldRunMotorDemo(int incomingByte){

	switch (incomingByte) {
	case 2: //2
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
bool ServoMotor::ProcessSerialInput(int incomingNum, vector<ServoMotor> motors){
	bool processed;

	for (int thisMotor = 0; thisMotor < motors.size(); thisMotor++) {
		ServoMotor motor = motors[thisMotor];

		processed = motor.ProcessSerialInput(incomingNum);

	}

	return processed;
}
bool ServoMotor::ProcessSerialInput(int incomingNum){
	bool processed;

	String inString(incomingNum);
	if (inString.length()>1 && inString.startsWith("3")){
		//set shakes
		String shakesString = inString.substring(1);
		long shks = shakesString.toInt();
		//SerialExt::Debug("shks: ", shks);
		SetShakes(shks);
		processed = true;
	}

	return processed;
}
