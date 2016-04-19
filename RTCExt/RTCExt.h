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
#include <EEPROM.h>
#include <EEWrap.h>

#include "SerialExt.h"
#include "DigitalTime.h"
#include "LCDMenu.h"

using namespace Utils;

using namespace Models;

namespace Utils {

	struct NextRunMemory{
		int64_e LastRun;
		int64_e NextRun;
		int64_e CountDown;
		int64_e RunEvery;
		int32_e ShakesOrTurns;
	};

		////remeber: dependant functions must be defined first in namespace.
		///**Better to use template functions.
	namespace RTCExt{

		//static bool _initalized;
		static NextRunMemory NextFeedInfo EEMEM;
		static NextRunMemory NextDoseInfo EEMEM;

		static tmElements_t _timeBuffer;
		static tmElements_t _nextRunBuffer;

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
			//String secString = String(seconds);
			//if (seconds < 10)
			//	secString = "0" + secString;

			//String timeString = String(hours) + ":";
			String timeString = hourString + ":";
			timeString += minString; //+":";
			//timeString += secString;

			return timeString;
		}

		template<typename T = DigitalTime>
		String FormatDigialTime(T&& time){

			T conv(time);
			String timeString = FormatDigialTime(time.Hours, time.Minutes, time.Seconds);

			return timeString;
		}
		template<typename T = time_t>
		DigitalTime GetDigitalTime(T&& seconds){
			T conv(seconds);

			int h, m, s;
			s = second(seconds); //seconds;
			m = minute(seconds);
			h = hourFormat12(seconds);

			DigitalTime time = DigitalTime(h, m, s);

			return time;
		}
		template<typename T = time_t>
		String GetDigitalTimeString(T&& seconds){
			T conv(seconds);

			DigitalTime time = GetDigitalTime(seconds);

			String runTime = FormatDigialTime(time);
			return runTime;
		}
		template<typename T = time_t>
		String GetMonthAndDateString(T&& seconds){
			T conv(seconds);
			
			String theDate;

			int theDay = day(seconds);
			int theMonth = month(seconds);
			int theYear = year(seconds);

			//SerialExt::Debug("theDay", theDay);

			String dayString = String(theDay);
			if (theDay < 10)
				dayString = "0" + dayString;
			String monthString = String(theMonth);
			if (theMonth < 10)
				monthString = "0" + monthString;
			String yearString = String(theYear);
			yearString.remove(0, 2);
			
			theDate = monthString + "/" + dayString + "/" + yearString;

			return theDate;
		}
		//returns digital date & time ##/##/#### ##:##:##
		template<typename T = time_t>
		String GetShortDateTimeString(T&& seconds){
			T conv(seconds);

			String result;

			String theDate = GetMonthAndDateString(seconds);
			String theTime = GetDigitalTimeString(seconds);

			result = theDate + " " + theTime;

			String am = "AM";
			if (isPM(seconds))
				am = "PM";

			result += am;

			return result;

		}


		template<typename T = void>
		time_t GetRTCTime()
		{
			return RTC.get();//now();
		}
		template<typename T = void>
		void Init(){

			//if (_initalized) return;

			setSyncProvider(RTC.get);   // the function to get the time from the RTC
			if (timeStatus() != timeSet)
				SerialExt::Print("Unable to sync with the RTC");
			else{
				String digitalTime = GetDigitalTimeString(GetRTCTime());
				SerialExt::Print("RTC Initialized: ", digitalTime);
				//_initalized = true;
			}
		}
		
		template<typename T = void>
		void SetRTCTime(int theHour, int theMinute, int theSecond, int theDay, int theMonth, int theYear)
		{
			//23h31m30s on 13Feb2009
			//setTime(8, 22, 00, 28, 2, 2016);
			setTime(theHour, theMinute, theSecond, theDay, theMonth, theYear);
			RTC.set(now());
		}

		template<typename T = void>
		bool IsRTCTimeSet()
		{
			auto time = GetRTCTime();
			auto theYear = year(time);
			if (theYear < 2016)return false;
			return true;
		}

		template<typename T = AccessoryType>
		NextRunMemory& FindNextRunInfo(T&& accType){
			if (accType == AccessoryType::Feeder)
				return NextFeedInfo;
			else if (accType == AccessoryType::DryDoser)
				return NextDoseInfo;
		}

		template<typename T = AccessoryType>
		void UpdateNextRun(T&& accType){

			auto rtcTime = GetRTCTime();
			NextRunMemory& nextRunMem = FindNextRunInfo(accType);

			int runEvery = 0;
			int countDown = 0;
			int nextRun = 0;
			int lastRun = 0;

			runEvery = nextRunMem.RunEvery;
			countDown = nextRunMem.CountDown;
			nextRun = nextRunMem.NextRun;
			lastRun = nextRunMem.LastRun;

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

			/*	auto nr2 = GetShortDateTimeString(nextRun);
			auto cd2 = GetTimeRemainingString(countDown);
			auto re2 = GetTimeRemainingString(runEvery);*/

			nextRunMem.RunEvery = runEvery;
			nextRunMem.CountDown = countDown;
			nextRunMem.NextRun = nextRun;
			nextRunMem.LastRun = lastRun;

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

		template<typename T = void>
		long ConvHoursToSec(int hour)
		{
			//T t(hour);
			long sec = (long)(hour * SECS_PER_HOUR);
			return sec;
		}
		template<typename T = void>
		int ConvSecToHour(long seconds)
		{
			//T t(hour);
			int h = (int)(seconds / SECS_PER_HOUR);
			return h;
		}

		template<typename T = long, typename N = long>
		String GetTimeFrequencyString(T&& runEvery, N&& nextRun)
		{
			String freq = "";

			String am = "AM";
			if (isPM(nextRun))
				am = "PM";

			String theTime = GetDigitalTimeString(nextRun);
			theTime = theTime + am;

			int h = ConvSecToHour(runEvery);

			if (h == 24)
				freq = ", Daily";
			else if (h = 48)
				freq = ", Every Other Day";

			String freqTime = theTime + freq;
			return freqTime;
		}

		template<typename T = long, typename M = AccessoryType>
		void SetRunEvery(T&& hour, M&& accType)
		{
			T t(hour);
			auto sec = ConvHoursToSec(hour);
			NextRunMemory& nextRunMem = FindNextRunInfo(accType);

			nextRunMem.RunEvery = sec;
			nextRunMem.NextRun = 0; //need to set to 0 so it recalculates

			UpdateNextRun(accType);
		}
		template<typename T = void>
		void SetRTCTimeFromTemp()
		{
			SetRTCTime(_timeBuffer.Hour, _timeBuffer.Minute, _timeBuffer.Second, _timeBuffer.Day, _timeBuffer.Month, _timeBuffer.Year);
		}

		template<typename T, typename M = LCDMenu::RangeType>
		void SetTimeTemp(T&& val, M&& rangeType)
		{
			T t(val);

			if (rangeType == LCDMenu::RangeType::Year)
				_timeBuffer.Year = val;
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

			//time_t nrTime;
			
			//nrTime = time_t(nextRunMem.NextRun);
			
			//localtime_s(&timePtr, &nrTime);
			
			if (rangeType == LCDMenu::RangeType::Minute)
				_nextRunBuffer.Minute = val;
			else if (rangeType == LCDMenu::RangeType::Hour)
				_nextRunBuffer.Hour = val;
			else if (rangeType == LCDMenu::RangeType::AmPm)
			{
				if (val == 0 && _nextRunBuffer.Hour > 12) //AM
					_nextRunBuffer.Hour = _nextRunBuffer.Hour - 12;
				else if (val == 1 && _nextRunBuffer.Hour < 12)//PM
					_nextRunBuffer.Hour = _nextRunBuffer.Hour + 12;

				//meridian is last step so update time
				auto newNrTime = makeTime(_nextRunBuffer);

				//todo: remove
				String nrTimes = GetShortDateTimeString(newNrTime);
				SerialExt::Debug("nrTimes", nrTimes);

				NextRunMemory& nextRunMem = FindNextRunInfo(accType);
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
		template<typename T = time_t>
		DigitalTime GetTimeRemaining(T&& seconds){
			T conv(seconds);

			int h, m;
			m = seconds / SECS_PER_MIN;
			h = seconds / SECS_PER_HOUR;
			auto min = fmod(m, 60);

			DigitalTime time = DigitalTime(h, min, 0);

			return time;
		}
		template<typename T = long>
		String GetTimeRemainingString(T&& seconds){
			T conv(seconds);

			DigitalTime time = GetTimeRemaining(seconds);
			auto timeString = FormatDigialTime(time.Hours, time.Minutes, time.Seconds);
			return timeString;
		}

	}
}


#endif

