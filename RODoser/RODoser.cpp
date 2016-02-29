
#include "RODoser.h"

RODoser::RODoser(Servo servo, int pin, int shakes, short relayPin, long runEverySeconds, AnalogSwitch theSwitch) :
ServoMotor(servo, pin, shakes, relayPin, runEverySeconds, theSwitch){};

RODoser::RODoser(){};

void RODoser::Dose(){
	
	Run();
}

void RODoser::RunDemo(vector<RODoser> dosers){
	
	vector<ServoMotor> motors;
	copy(dosers.begin(), dosers.end(), back_inserter(motors));
	ServoMotor::RunMotorDemos(motors);

}
