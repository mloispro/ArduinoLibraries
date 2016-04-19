// SerialExt.h

#ifndef LCDMENU_h
#define LCDMENU_h

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
using namespace Utils;

class LCDMenu
{

public:
	enum RangeType
	{
		Nav,
		Year,
		Month,
		Day,
		Hour,
		Minute,
		AmPm,
		Frequency,
		TimeLong,
		TimeShort,
		TimeFrequency,
		ShakesOrTurns,
		SetShakesOrTurns
	};

	short Id;
	short OptionId;
	short NextMenuId;
	short  PrevMenuId;
	String Text;
	String  OptionText;
	RangeType  TheRangeType;
	AccessoryType AccType;

	//vector<LCDMenu> MenuItems; //dont work, fuck it!!!!!!!!!!

	LCDMenu(short id, short optionId, short nextMenuId, short prevMenuId, String text, String optionText, RangeType rangeType, AccessoryType accType) :
		Id(id), OptionId(optionId), NextMenuId(nextMenuId), PrevMenuId(prevMenuId), Text(text), OptionText(optionText), TheRangeType(rangeType), AccType(accType)
	{
		//MenuItems();
	}
	LCDMenu(short id, short optionId, short nextMenuId, short prevMenuId, String text, String optionText, RangeType rangeType) :
		LCDMenu(id, optionId, nextMenuId, prevMenuId, text, optionText, rangeType, AccessoryType::None)
	{
	}

};

#endif

