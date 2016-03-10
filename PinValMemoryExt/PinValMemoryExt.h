// SerialExt.h

#ifndef PINVALMEMORY_h
#define PINVALMEMORY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <StandardCplusplus.h>
#include <string>
#include <vector>
using namespace std;

#include <EEPROM.h>
#include <EEWrap.h>

//#include "TimerExt.h"
#include "SerialExt.h"

namespace Utils {

	struct PinValMemory{
		int16_e Pin;
		int64_e Value;
	};
		////remeber: dependant functions must be defined first in namespace.
		///**Better to use template functions.
	namespace PinValMemoryExt {
		static PinValMemory ThePinValMemory EEMEM;
		static PinValMemory ThePinValMemory2 EEMEM;
		static PinValMemory ThePinValMemory3 EEMEM;
		static PinValMemory ThePinValMemory4 EEMEM;
		static PinValMemory ThePinValMemory5 EEMEM;
		static PinValMemory ThePinValMemory6 EEMEM;

		/*static vector<PinValMemory> PinValMemoryCollection;

		template<typename T = void>
		static void Init()
		{
			int noPin = 0;
			if (ThePinValMemory.Pin == noPin)
				ThePinValMemory.Pin = -1;
			if (ThePinValMemory2.Pin == noPin)
				ThePinValMemory2.Pin = -1;
			if (ThePinValMemory3.Pin == noPin)
				ThePinValMemory3.Pin = -1;
			if (ThePinValMemory4.Pin == noPin)
				ThePinValMemory4.Pin = -1;
			if (ThePinValMemory5.Pin == noPin)
				ThePinValMemory5.Pin = -1;
			if (ThePinValMemory6.Pin == noPin)
				ThePinValMemory6.Pin = -1;
			PinValMemoryCollection.push_back(ThePinValMemory);
			PinValMemoryCollection.push_back(ThePinValMemory2);
			PinValMemoryCollection.push_back(ThePinValMemory3);
			PinValMemoryCollection.push_back(ThePinValMemory4);
			PinValMemoryCollection.push_back(ThePinValMemory5);
			PinValMemoryCollection.push_back(ThePinValMemory6);
		}*/

		//template<typename T = int>
		//PinValMemory Get(T&& pin){
		//	T conv(pin);

		//	PinValMemory mem = ThePinValMemory;
		//	//int runTime = TimerExt::GetRuntimeInSeconds();

		//	if (ThePinValMemory.Pin == -1){
		//		//todo, collection is being

		//		//Init();
		//		//mem.Pin = pin;
		//		////schedule.RuntimeInSeconds = runTime;
		//		//return mem;
		//	}

		//	//if there are no schedules, then create them.
		//	if (PinValMemoryCollection.size() <= 0){
		//		Init();
		//		mem.Pin = pin;
		//		//schedule.RuntimeInSeconds = runTime;
		//		return mem;
		//	}

		//	//find shedule by pin
		//	bool found;
		//	for (const auto& pinMem : PinValMemoryCollection)
		//	{
		//		int thePin = mem.Pin;

		//		if (thePin == pin)
		//		{
		//			found = true;
		//			mem = pinMem;
		//			break;
		//		}
		//	}

		//	//if not found, find first availble one.
		//	if (!found){
		//		int noPin = -1;
		//		for (const auto& pinMem : PinValMemoryCollection)
		//		{
		//			int thePin = mem.Pin;

		//			if (thePin == noPin)
		//			{
		//				found = true;
		//				mem = pinMem;
		//				break;
		//			}
		//		}
		//	}
		//	mem.Pin = pin;

		//	Utils::SerialExt::Print("mem.Pin :", mem.Pin);

		//	return mem;
		//}

		//template<typename T = int, typename T2 = long>
		//void SavePinVal(T&& pin, T2&& val){
		//	T conv(pin);
		//	T2 conv2(val);

		//	PinValMemory mem = Get(pin);
		//	mem.Value = val;
		//}
		
	}
}


#endif

