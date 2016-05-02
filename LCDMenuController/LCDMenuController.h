
#ifndef LCDMENUCONTROLLER_h
#define LCDMENUCONTROLLER_h

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
#include <LiquidCrystal.h>

#include "SerialExt.h"
#include <LCDMenu.h>
#include "DigitalTime.h"
#include "RTCExt.h"
using namespace Utils;

namespace Controllers {

	//typedef void(*lcdVal_callback)(int);


	class LCDMenuController{
	private:
		static const short mainMenu = 0;
		static const short feedMenu = 1;
		static const short feedFreqMenu = 2;
		static const short feedHourMenu = 3;
		static const short feedMinMenu = 4;
		static const short feedAmPmMenu = 5;
		static const short clockMenu = 6;
		static const short clockYearMenu = 7;
		static const short clockMonthMenu = 8;
		static const short clockDayMenu = 9;
		static const short feedTimeMenu = 10;
		static const short clockHourMenu = 11;
		static const short clockMinMenu = 12;
		static const short clockAmPmMenu = 13;
		static const short doserMenu = 14;
		static const short doserFreqMenu = 15;
		static const short doserHourMenu = 16;
		static const short doserMinMenu = 17;
		static const short doserAmPmMenu = 18;
		static const short doserTimeMenu = 19;
		static const short feedShakesMenu = 20;
		static const short feedSetShakesMenu = 21;
		static const short doserShakesMenu = 22;
		static const short doserSetShakesMenu = 23;

		short _scrollIndex;
		int _scrollDelay = 3000;

		int _selectDelay = 2000;

		vector<LCDMenu> _menus;
		//String _selectedText;
		short _selectedMenuId = -1;
		short _selectedOptionId = -1;

		int _lowerLimit;
		int _upperLimit;

		//--key press
		int _keyValues[5] = { 50, 200, 400, 600, 800 };
		short _numOfKeys = 5;
		int GetKeyFromVal(unsigned int input);
		void NextOption();
		void PreviousOption();
		void LeftButton();
		void SelectButton();
		void LimitRange(int lower, int upper);
		void DetectKeyPress();

		LCDMenu GetMenu(short id, short optionId);
		void PrintMenu(LCDMenu menu);
		void PrintLine(short lineNum, String text);
		void SetSelectedMenu(LCDMenu menu);

		void AddMenu(short id, short optionId, short nextMenuId, short prevMenuId, String text, String optionText, LCDMenu::RangeType rangeType);
		void AddMenu(short id, short optionId, short nextMenuId, short prevMenuId, String text, String optionText, LCDMenu::RangeType rangeType, AccessoryType accType);
		void CreateMenus();
		LCDMenu GetSelectedMenu();
		void SetClockMenu();

		String GetOptionAsNumber(String defaultNumber, bool isTwoDigits);
		String GetOptionAsNumber(String defaultNumber);

		void PrintInstructions();

		LiquidCrystal _lcd;
		//LiquidCrystal* _lcd = new LiquidCrystal(8, 9, 4, 5, 6, 7);
	
	public:
		int _optionCount;

		LCDMenuController() :_lcd(8, 9, 4, 5, 6, 7){}
		//LCDMenuController();

		void Init();

		String GetRangeOption(LCDMenu::RangeType rangeType, AccessoryType accType);
		void SaveRangeOption(LCDMenu::RangeType rangeType, AccessoryType accType);

		int GetKey();
		void SelectMainMenu();
		void ExitMainMenu();

		void PrintRunInfo(String lablel, AccessoryType accType);
		void PrintTime();
		void Scroll();

		String GetTimeFrequency(AccessoryType accType);
		String GetTimeLong(AccessoryType accType);

		void SetShakesOrTurns(AccessoryType accType, short shakesOrTurns);
		int GetShakesOrTurns(AccessoryType accType);
	};
}

#endif

