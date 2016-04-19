
#include "RODoser.h"

RODoser::RODoser(Servo servo, int pin, int shakes, short relayPin, long runEverySeconds, AnalogSwitch theSwitch) :
ServoMotor(servo, pin, shakes, relayPin, runEverySeconds, theSwitch){
	ServoType = AccessoryType::DryDoser;
};

RODoser::RODoser(Servo servo, int pin, int shakes, long runEverySeconds, AnalogSwitch theSwitch) :
ServoMotor(servo, pin, shakes, runEverySeconds, theSwitch){
	ServoType = AccessoryType::DryDoser;
};

RODoser::RODoser(){ ServoType = AccessoryType::DryDoser; };

void RODoser::Dose(){
	_theSpeed = 10;
	Run();
}

void RODoser::RunDemo(vector<RODoser> dosers){
	
	vector<ServoMotor> motors;
	copy(dosers.begin(), dosers.end(), back_inserter(motors));
	ServoMotor::RunMotorDemos(motors);

}
