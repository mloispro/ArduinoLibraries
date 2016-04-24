
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

#include <LiquidCrystal.h>

#include "SerialExt.h"
#include <LCDMenu.h>
#include "RTCExt.h"
using namespace Utils;

namespace Controllers {

	//typedef void(*lcdVal_callback)(int);


	class LCDMenuController{
	private:
		const short mainMenu = 0;
		const short feedMenu = 1;
		const short feedFreqMenu = 2;
		const short feedHourMenu = 3;
		const short feedMinMenu = 4;
		const short feedAmPmMenu = 5;
		const short clockMenu = 6;
		const short clockYearMenu = 7;
		const short clockMonthMenu = 8;
		const short clockDayMenu = 9;
		const short feedTimeMenu = 10;
		const short clockHourMenu = 11;
		const short clockMinMenu = 12;
		const short clockAmPmMenu = 13;
		const short doserMenu = 14;
		const short doserFreqMenu = 15;
		const short doserHourMenu = 16;
		const short doserMinMenu = 17;
		const short doserAmPmMenu = 18;
		const short doserTimeMenu = 19;
		const short feedShakesMenu = 20;
		const short feedSetShakesMenu = 21;
		const short doserShakesMenu = 22;
		const short doserSetShakesMenu = 23;

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

		void PrintRunInfo(AccessoryType accType);
		void PrintTime();
		void Scroll();

		String GetTimeFrequency(AccessoryType accType);
		String GetTimeLong(AccessoryType accType);

		void SetShakesOrTurns(AccessoryType accType, short shakesOrTurns);
		int GetShakesOrTurns(AccessoryType accType);
	};
}

#endif

