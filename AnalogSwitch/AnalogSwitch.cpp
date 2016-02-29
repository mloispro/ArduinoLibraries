
#include "AnalogSwitch.h"

AnalogSwitch::AnalogSwitch(short analogPin, int max) :
AnalogPin(analogPin), Max(max){
	Init();
};

AnalogSwitch::AnalogSwitch(short analogPin) :
AnalogPin(analogPin){
	Init();
};
AnalogSwitch::AnalogSwitch(){};

void AnalogSwitch::Init(){
	if (AnalogPin >= 0 && AnalogPin <= 5)
		pinMode(AnalogPin, INPUT); //receive switch signal
}

bool AnalogSwitch::IsOn(){
	//read switch
	SwitchReading = analogRead(AnalogPin);
	
	if (SwitchReading > Max - 50){
		return true;
	}
	return false;
}

