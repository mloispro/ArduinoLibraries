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

#include <EEPROM.h>
#include <EEWrap.h>

namespace Utils {

	//template<class T>
	class DigitalTime{

	public:
		int Hours;
		int Minutes;
		int Seconds;
		DigitalTime(int hours, int minutes, int seconds) : Hours(hours), Minutes(minutes), Seconds(seconds){}
		//DigitalTime(int&& hours, int&& minutes, int&& seconds) : Hours(hours), Minutes(minutes), Seconds(seconds){}
		//DigitalTime(int& hours, int& minutes, int& seconds) : Hours(hours), Minutes(minutes), Seconds(seconds){}
		//DigitalTime(DigitalTime time) : Hours(time.hours), Minutes(minutes), Seconds(seconds){}
	};
	struct RunSchedule{
		uint32_e Pin;
		uint32_e LastRunInSeconds;
		uint32_e NextRunInSeconds;
		uint32_e RuntimeInSeconds;
	};

	namespace TimerExt {
		static RunSchedule TheRunSchedule EEMEM;
		static RunSchedule TheRunSchedule2 EEMEM;
		static RunSchedule TheRunSchedule3 EEMEM;
		static RunSchedule TheRunSchedule4 EEMEM;

		static vector<RunSchedule> RunSchedules;

		template<typename T = void>
		static void InitSchedules()
		{
			TheRunSchedule.Pin = -1;
			TheRunSchedule2.Pin = -1;
			TheRunSchedule3.Pin = -1;
			TheRunSchedule4.Pin = -1;
			RunSchedules.push_back(TheRunSchedule);
			RunSchedules.push_back(TheRunSchedule2);
			RunSchedules.push_back(TheRunSchedule3);
			RunSchedules.push_back(TheRunSchedule4);
		}
		
		template<typename T = int>
		static RunSchedule GetRunSchedule(T&& pin){
			T conv(pin);

			RunSchedule schedule = TheRunSchedule;
			//int runTime = TimerExt::GetRuntimeInSeconds();

			if (RunSchedules.size() <= 0){
				InitSchedules();
				schedule.Pin = pin;
				//schedule.RuntimeInSeconds = runTime;
				return schedule;
			}

			//uint32_e thePin = EEPROM.; //(uint16_e)pin;
			uint32_e b(pin);
			//find shedule by pin
			bool found;
			for (const auto& runSched : RunSchedules)
			{
				const uint32_e runSchedPin = runSched.Pin;
				if (runSchedPin == pin)
				{
					found = true;
					schedule = runSched;
					break;
				}
			}

			////if not found, find first availble one.
			//if (!found){
			//	short&& noPin = -1;
			//	for (const auto& runSched : RunSchedules)
			//	{
			//		if (runSched.Pin == noPin)
			//		{
			//			found = true;
			//			schedule = runSched;
			//			break;
			//		}
			//	}
			//}
			//schedule.Pin = pin;
			///schedule.RuntimeInSeconds = runTime;

			return schedule;
		}

		////remeber: dependant functions must be defined first in namespace.
		///**Better to use template functions.

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

		template<int...>
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

