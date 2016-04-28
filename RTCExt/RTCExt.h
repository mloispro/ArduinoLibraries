// SerialExt.h

#ifndef RTCEXT_h
#define RTCEXT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <StandardCplusplus.h>
#include <string>
#include <vector>
using namespace std;

#include <Wire.h>
#include <TimeLib.h>
#include <Time.h>
#include <DS3232RTC.h>

//#include "EEPRomMem.h"
//#include <EEPROM.h>

#include "MemoryExt.h"
#include "SerialExt.h"
#include "DigitalTime.h"
#include "LCDMenu.h"

using namespace Utils;

using namespace Models;
using namespace Time;


namespace Utils {
	using namespace MemoryExt;

		////remeber: dependant functions must be defined first in namespace.
		///**Better to use template functions.
	namespace RTCExt{

		static NextRunMemory NextFeedInfo;
		static NextRunMemory NextDoseInfo;

		//static only sticks local
		static tmElements_t _timeBuffer;
		//static tmElements_t _nextRunBuffer;

		template<typename T = void>
		time_t GetRTCTime()
		{
			return now();
		}
		template<typename T = void>
		void SetRTCTime(int theHour, int theMinute, int theSecond, int theDay, int theMonth, int theYear)
		{
			//23h31m30s on 13Feb2009
			//setTime(8, 22, 00, 28, 2, 2016);
			setTime(theHour, theMinute, theSecond, theDay, theMonth, theYear);
			RTC.set(now());
			//tmElements_t tm;
			//tm.Hour = theHour;
			//tm.Minute = theMinute;
			//tm.Day = theDay;
			//tm.Month = theMonth;
			//tm.Year = theYear;
			//time_t t = makeTime(tm);
			//RTC.set(t);
		}
		/*template<typename T = void>
		void SetDefaultRTCTime()
		{
			SetRTCTime(1, 1, 1, 1, 1, 2000);
		}
		template<typename T = void>
		bool IsDefaultRTCTime()
		{
			time_t rtcTime = GetRTCTime();
			int y = year(rtcTime);
			int m = month(rtcTime);
			int d = day(rtcTime);
			int h = hour(rtcTime);
			if (y == 2000 && m == 1 && d == 1 && h == 1)
				return true;

			return false;

		}*/
		template<typename T = void>
		bool IsRTCTimeSet()
		{
			auto time = GetRTCTime();
			auto theYear = year(time);
			if (theYear < 2016)return false;
			return true;
		}
		template<typename T=void>
		void LoadNextRunInfos(){
			int accTypeFeed = static_cast<int>(AccessoryType::Feeder);
			int accTypeDoser = static_cast<int>(AccessoryType::DryDoser);

			NextFeedInfo.AccType = accTypeFeed;
			NextFeedInfo = MemoryExt::GetNextRunMem(NextFeedInfo);

			NextDoseInfo.AccType = accTypeDoser;
			NextDoseInfo = MemoryExt::GetNextRunMem(NextDoseInfo);

		}
		template<typename T = void>
		void Init(){
			
			LoadNextRunInfos();

			setSyncProvider(RTC.get);   // the function to get the time from the RTC
			if (timeStatus() != timeSet)
				SerialExt::Print(F("Unable to sync with the RTC, time not set."));
			else{
				String digitalTime = GetDigitalTimeString(GetRTCTime());
				SerialExt::Print(F("RTC Initialized: "), digitalTime);
				//_initalized = true;
			}
		}
		template<typename T = AccessoryType>
		NextRunMemory& FindNextRunInfo(T&& accType){
			if (accType == AccessoryType::Feeder)
				return NextFeedInfo;
			else if (accType == AccessoryType::DryDoser)
				return NextDoseInfo;
		}
		template<typename T = AccessoryType>
		void SaveNextRunInfo(T&& accType){
			int accTypeInt = static_cast<int>(accType);
			NextRunMemory& mem = FindNextRunInfo(accType);
			
			mem.AccType = accTypeInt;
			mem = MemoryExt::SaveNextRunMem(mem);
			
		}
		

		

		template<typename T = AccessoryType>
		void UpdateNextRun(T&& accType){

			auto rtcTime = GetRTCTime();
			NextRunMemory& nextRunMem = FindNextRunInfo(accType);

			long runEvery = nextRunMem.RunEvery;
			long countDown = nextRunMem.CountDown;
			long nextRun = nextRunMem.NextRun;
			long lastRun = nextRunMem.LastRun;

			//TODO comment out
			auto rtc1 = GetShortDateTimeString(rtcTime);
			auto nr1 = GetShortDateTimeString(nextRun);
			auto cd1 = GetTimeRemainingString(countDown);
			auto re1 = GetTimeRemainingString(runEvery);

			SerialExt::Debug("accType", accType);
			SerialExt::Debug("rtc1", rtc1);
			SerialExt::Debug("nr1", nr1);
			SerialExt::Debug("cd1", cd1);
			SerialExt::Debug("re1", re1);

			if (!IsRTCTimeSet() || runEvery == 0)
				return;// rtcTime;

			if (lastRun == 0) //before first feeding
				lastRun = rtcTime;

			if (lastRun > rtcTime){
				lastRun = rtcTime;
				nextRun = 0;
			}

			if (nextRun <= 0)
				nextRun = rtcTime + runEvery; //08:51:49

			if (lastRun > 0 && nextRun <= rtcTime)
				nextRun = lastRun + runEvery;

			countDown = nextRun - rtcTime; //00:00:06

			if (nextRun <= rtcTime){
				nextRun = rtcTime;
				countDown = 0;
			}

			//todo comment out
			auto nr2 = GetShortDateTimeString(nextRun);
			auto cd2 = GetTimeRemainingString(countDown);
			auto re2 = GetTimeRemainingString(runEvery);

			SerialExt::Debug("nr2", nr2);
			SerialExt::Debug("cd2", cd2);
			SerialExt::Debug("re2", re2);

			nextRunMem.RunEvery = runEvery;
			nextRunMem.CountDown = countDown;
			nextRunMem.NextRun = nextRun;
			nextRunMem.LastRun = lastRun;

			//todo change to 900
			//save every 15 min. 900
			long saveTime = nextRunMem.LastSave + 60;
			if (saveTime <= rtcTime){
				SerialExt::Debug("time to save nr", GetDigitalTimeString(rtcTime));
				SaveNextRunInfo(accType);
			}

		}

		template<typename T = void>
		bool IsTimeToRun(AccessoryType accType){

			NextRunMemory& nextRunMem = FindNextRunInfo(accType);

			if (nextRunMem.RunEvery <= 0)return true; //not using rtc

			time_t runTime = RTCExt::GetRTCTime();
			UpdateNextRun(accType);
			time_t nextRun = nextRunMem.NextRun;
			//int runTime = TimerExt::GetRuntimeInSeconds();
			if (nextRun <= runTime){
				return true;
			}
			return false;
		}

		template<typename T = long, typename N = long>
		String GetTimeFrequencyString(T&& runEvery, N&& nextRun)
		{
			String freq = "";

			String am = F("AM");
			if (isPM(nextRun))
				am = F("PM");

			String theTime = GetDigitalTimeString(nextRun);
			theTime = theTime + am;

			int h = ConvSecToHour(runEvery);

			if (h == 24)
				freq = F(", Daily");
			else if (h = 48)
				freq = F(", Every Other Day");

			String freqTime = theTime + freq;
			return freqTime;
		}

		template<typename T = long, typename M = AccessoryType>
		void SetRunEvery(T&& hour, M&& accType)
		{
			T t(hour);
			long sec = ConvHoursToSec(hour);
			NextRunMemory& nextRunMem = FindNextRunInfo(accType);

			SerialExt::Debug("sec", sec);

			nextRunMem.RunEvery = sec;
			nextRunMem.NextRun = 0; //need to set to 0 so it recalculates

			long re = nextRunMem.RunEvery;
			SerialExt::Debug("nextRunMem.RunEvery", re);

			UpdateNextRun(accType);
		}

		template<typename T = void>
		void SetRTCTimeFromTemp()
		{
			SetRTCTime(_timeBuffer.Hour, _timeBuffer.Minute, _timeBuffer.Second, _timeBuffer.Day, _timeBuffer.Month, _timeBuffer.Year);
		}

		template<typename T = void>
		void ClearTimeTemp()
		{
			_timeBuffer.Hour = 0;
			_timeBuffer.Minute = 0;
			_timeBuffer.Second = 0;
			_timeBuffer.Day = 0;
			_timeBuffer.Month = 0;
			_timeBuffer.Year = 0;
			_timeBuffer.Wday = 0;
		}

		template<typename T, typename M = LCDMenu::RangeType>
		void SetTimeTemp(T&& val, M&& rangeType)
		{
			T t(val);

			if (rangeType == LCDMenu::RangeType::Year){
				ClearTimeTemp();
				_timeBuffer.Year = val;
			}
			else if (rangeType == LCDMenu::RangeType::Month)
				_timeBuffer.Month = val;
			else if (rangeType == LCDMenu::RangeType::Day)
				_timeBuffer.Day = val;
			else if (rangeType == LCDMenu::RangeType::Hour)
				_timeBuffer.Hour = val;
			else if (rangeType == LCDMenu::RangeType::Minute)
				_timeBuffer.Minute = val;
			else if (rangeType == LCDMenu::RangeType::AmPm)
			{

				if (val == 0 && _timeBuffer.Hour == 12) // val = 0->AM
					_timeBuffer.Hour = 0;
				else if (val == 1 && _timeBuffer.Hour == 12) // val = 1->PM
					_timeBuffer.Hour = 23;
				else if (val == 1 && _timeBuffer.Hour < 12)// val = 1->PM
					_timeBuffer.Hour = _timeBuffer.Hour + 12;
			}
		}

		

		template<typename T, typename M = LCDMenu::RangeType, typename P = AccessoryType>
		void SetNextRun(T&& val, M&& rangeType, P&& accType)
		{
			T t(val);
			M m(rangeType);
			P p(accType);
			
			if (rangeType == LCDMenu::RangeType::Hour){
				ClearTimeTemp();
				_timeBuffer.Hour = val;
			}
			else if (rangeType == LCDMenu::RangeType::Minute)
				_timeBuffer.Minute = val;
			else if (rangeType == LCDMenu::RangeType::AmPm)
			{
				
				NextRunMemory& nextRunMem = FindNextRunInfo(accType);

				//next run in seconds.
				long nrSecs = nextRunMem.NextRun;
				
				if (val == 1)//pm
					_timeBuffer.Hour = _timeBuffer.Hour + 12;

				int y = year(nrSecs);
				_timeBuffer.Year = CalendarYrToTm(y);
				_timeBuffer.Month = month(nrSecs);
				_timeBuffer.Day = day(nrSecs);
				_timeBuffer.Second = second(nrSecs);

				//meridian is last step so update time
				time_t newNrTime = makeTime(_timeBuffer);

				SerialExt::Debug("nextRunBuffer_min2", _timeBuffer.Minute);
				SerialExt::Debug("nextRunBuffer_hour2", _timeBuffer.Hour);

				nextRunMem.NextRun = newNrTime;

				UpdateNextRun(accType);
			}
			else
				return;

			
		}
		template<typename P = AccessoryType>
		void SetLastRun(P&& accType)
		{
			NextRunMemory& nextRunMem = FindNextRunInfo(accType);
			nextRunMem.LastRun = GetRTCTime();
			UpdateNextRun(accType);
		}


	}
}


#endif

