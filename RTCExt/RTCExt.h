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


#include "SerialExt.h"
#include "DigitalTime.h"

using namespace Models;

namespace Utils {

		////remeber: dependant functions must be defined first in namespace.
		///**Better to use template functions.
	namespace RTCExt{

		//static bool _initalized;

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
		template<typename T = time_t>
		DigitalTime GetDigitalTime(T&& seconds){
			T conv(seconds);

			int h, m, s;
			s = second(seconds); //seconds;
			m = minute(seconds);
			h = hour(seconds);

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

			String dayString = String(theDay);
			if (theDay < 10)
				dayString = "0" + dayString;
			String monthString = String(theMonth);
			if (theMonth < 10)
				monthString = "0" + monthString;
			
			theDate = monthString + "/" + dayString;

			return theDate;
		}
		template<typename T = time_t>
		String GetShortDateTimeString(T&& seconds){
			T conv(seconds);

			String result;

			String theDate = GetMonthAndDateString(seconds);
			String theTime = GetDigitalTimeString(seconds);

			result = theDate + " " + theTime;
			return result;

		}
		template<typename T = void>
		time_t GetRTCTime()
		{
			return now();
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
	}
}


#endif

