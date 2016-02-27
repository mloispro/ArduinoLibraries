// SerialExt.h

#ifndef RUNSCHEDULE_h
#define RUNSCHEDULE_h

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

#include "TimerExt.h"
#include "SerialExt.h"

namespace Utils {

	struct RunSchedule{
		uint16_e Pin;
		uint32_e NextRunCountDown;
		uint32_e LastRunInSeconds;
		//uint32_e NextRunInSeconds;
		//uint32_e RuntimeInSeconds;
	};
		////remeber: dependant functions must be defined first in namespace.
		///**Better to use template functions.
	namespace RunScheduleExt {
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
		RunSchedule GetRunSchedule(T&& pin){
			T conv(pin);

			RunSchedule schedule = TheRunSchedule;
			int runTime = TimerExt::GetRuntimeInSeconds();

			//if there are no schedules, then create them.
			if (RunSchedules.size() <= 0){
				InitSchedules();
				schedule.Pin = pin;
				//schedule.RuntimeInSeconds = runTime;
				return schedule;
			}

			//find shedule by pin
			bool found;
			for (const auto& runSched : RunSchedules)
			{
				int runSchedPin = schedule.Pin;

				if (runSchedPin == pin)
				{
					found = true;
					schedule = runSched;
					break;
				}
			}

			//if not found, find first availble one.
			if (!found){
				int noPin = -1;
				for (const auto& runSched : RunSchedules)
				{
					int runSchedPin = schedule.Pin;

					if (runSchedPin == noPin)
					{
						found = true;
						schedule = runSched;
						break;
					}
				}
			}
			schedule.Pin = pin;
			//schedule.RuntimeInSeconds = runTime;

			Utils::SerialExt::Debug("schedule.Pin :", schedule.Pin);
//			Utils::SerialExt::Debug("schedule.RuntimeInSeconds :", schedule.RuntimeInSeconds);

			return schedule;
		}

		template<typename T = int, typename T2 = int>
		void SaveTheRun(T&& pin, T2&& runNext){
			T conv(pin);

			RunSchedule schedule = GetRunSchedule(pin);
			schedule.NextRunCountDown = runNext;
			schedule.LastRunInSeconds = TimerExt::GetRuntimeInSeconds();
		}
		
	}
}


#endif

