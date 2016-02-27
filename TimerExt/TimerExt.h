// SerialExt.h

#ifndef _TIMEREXT_h
#define _TIMEREXT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <StandardCplusplus.h>
#include <string>
#include <vector>
using namespace std;


#include "SerialExt.h"

namespace Utils {

	//template<class T>
	class DigitalTime{

	public:
		int Hours;
		int Minutes;
		int Seconds;
		DigitalTime(int hours, int minutes, int seconds) : Hours(hours), Minutes(minutes), Seconds(seconds){}
	};
		////remeber: dependant functions must be defined first in namespace.
		///**Better to use template functions.
	namespace TimerExt{
		template<typename H = int, typename M = int, typename S = int>
		String FormatDigialTime(H&& hours, M&& minutes, S&& seconds)
		{
			H convH(hours);
			M convM(minutes);
			S convS(seconds);

			String hourString = String(hours);
			if (hours < 10)
				hourString = "0" + hourString;
			String minString = String(minutes);
			if (minutes < 10)
				minString = "0" + minString;
			String secString = String(seconds);
			if (seconds < 10)
				secString = "0" + secString;
			//String timeString = String(hours) + ":";
			String timeString = hourString + ":";
			timeString += minString + ":";
			timeString += secString;
			return timeString;
		}

		template<typename T = DigitalTime>
		String FormatDigialTime(T&& time){

			T conv(time);
			String timeString = FormatDigialTime(time.Hours, time.Minutes, time.Seconds);
			return timeString;
		}

		template<typename T = void>
		int GetRuntimeInSeconds(){
			int s;
			//T t(s);
			s = millis() / 1000;
			return s;
		}
		
		template<typename T = int>
		DigitalTime GetTimeFromSeconds(T&& seconds){
			T conv(seconds);
	
			int h, m, s;
			s = seconds;
			m = s / 60;
			h = s / 3600;
			s = s - m * 60;
			m = m - h * 60;
			DigitalTime time = DigitalTime(h, m, s);

			return time;
		}
		
		template<typename T = int>
		String GetDigitalTimeFromSeconds(T&& seconds){
			T conv(seconds);
			DigitalTime time = GetTimeFromSeconds(seconds);

			String runTime = FormatDigialTime(time);
			return runTime;
		}
		template<typename T = String>
		T GetDigitalRuntime(){
			int s = GetRuntimeInSeconds();

			String runTime = GetDigitalTimeFromSeconds(s);
			return runTime;
		}
		
		template<typename T = void>
		void PrintDigitalRuntime(){
			String runtime = GetDigitalRuntime();
			Serial.print("System Run Time: ");
			Serial.println(runtime);
		}

		template<typename T = int>
		void PrintDigitalTime(T&& seconds){
			T conv(seconds);
			String dTime = GetDigitalTimeFromSeconds(seconds);
			Serial.println(dTime);
		}
		//template<typename T = RunTime>
		//void Save(T&& lastRunTime){
		//	T conv(lastRunTime);
		//	//T2 conv2(nextRunTime);

		//	T lastRunTime EEMEM;
		//}
		
		
	}
}


#endif

