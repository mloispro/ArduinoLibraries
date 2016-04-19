// SerialExt.h

#ifndef DIGITALTIME_h
#define DIGITALTIME_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <StandardCplusplus.h>
#include <string>
#include <vector>
using namespace std;


#include <Time.h>
#include "SerialExt.h"

namespace Models {

	//template<class T>
	class DigitalTime{

	public:
		int Hours;
		int Minutes;
		int Seconds;
		
		DigitalTime(int hours, int minutes, int seconds) : Hours(hours), Minutes(minutes), Seconds(seconds){}
	};
}


#endif

