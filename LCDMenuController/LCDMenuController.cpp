#include "LCDMenuController.h"

using namespace Controllers;

LiquidCrystal LCDMenuController::_lcd(8, 9, 4, 5, 6, 7);

LCDMenuController::LCDMenuController()
{
	CreateMenus();
}

void LCDMenuController::CreateMenus()
{

	if (_menus.size() > 0)
		_menus.clear();

	//menus
	AddMenu(mainMenu, 0, feedMenu, mainMenu, "Menu: [>] Exit", "Feeder", LCDMenu::RangeType::Nav);
	AddMenu(mainMenu, 1, doserMenu, mainMenu, "Menu: [>] Exit", "Doser", LCDMenu::RangeType::Nav);
	AddMenu(mainMenu, 2, clockMenu, mainMenu, "Menu: [>] Exit", "Clock", LCDMenu::RangeType::Nav);

	//feed menus
	AddMenu(feedMenu, 0, feedTimeMenu, mainMenu, "Feeder: [<] Back", "Feed Time", LCDMenu::RangeType::Nav, AccessoryType::Feeder);
	AddMenu(feedMenu, 1, feedFreqMenu, mainMenu, "Feeder: [<] Back", "Set Feed Time", LCDMenu::RangeType::Nav, AccessoryType::Feeder);
	AddMenu(feedMenu, 2, feedShakesMenu, mainMenu, "Feeder: [<] Back", "Set Feed Shakes", LCDMenu::RangeType::Nav, AccessoryType::Feeder);

	//feed time
	AddMenu(feedTimeMenu, 0, feedTimeMenu, feedMenu, "Feed Time: [<] Back", "Not Set", LCDMenu::RangeType::TimeFrequency, AccessoryType::Feeder);
	AddMenu(feedFreqMenu, 0, feedHourMenu, feedMenu, "Feed Frequency: [<] Back", "Frequency", LCDMenu::RangeType::Frequency, AccessoryType::Feeder);
	AddMenu(feedHourMenu, 0, feedMinMenu, feedFreqMenu, "Feed Hour: [<] Back", "Hour", LCDMenu::RangeType::Hour, AccessoryType::Feeder);
	AddMenu(feedMinMenu, 0, feedAmPmMenu, feedHourMenu, "Feed Minute: [<] Back", "Minute", LCDMenu::RangeType::Minute, AccessoryType::Feeder);
	AddMenu(feedAmPmMenu, 0, feedTimeMenu, feedMinMenu, "Feed AM-PM: [<] Back", "AM PM", LCDMenu::RangeType::AmPm, AccessoryType::Feeder);

	//feed shakes
	AddMenu(feedShakesMenu, 0, feedShakesMenu, feedMenu, "Feed Shakes: [<] Back", "Not Set", LCDMenu::RangeType::ShakesOrTurns, AccessoryType::Feeder);
	AddMenu(feedShakesMenu, 1, feedSetShakesMenu, feedShakesMenu, "Feed Shakes: [<] Back", "Set Feed Shakes", LCDMenu::RangeType::Nav, AccessoryType::Feeder);
	AddMenu(feedSetShakesMenu, 0, feedShakesMenu, feedShakesMenu, "Set Feed Shakes: [<] Back", "Not Set", LCDMenu::RangeType::SetShakesOrTurns, AccessoryType::Feeder);

	//doser menus
	AddMenu(doserMenu, 0, doserTimeMenu, mainMenu, "Doser: [<] Back", "Doser Time", LCDMenu::RangeType::Nav, AccessoryType::DryDoser);
	AddMenu(doserMenu, 1, doserFreqMenu, mainMenu, "Doser: [<] Back", "Set Doser Time", LCDMenu::RangeType::Nav, AccessoryType::DryDoser);
	AddMenu(doserMenu, 2, doserShakesMenu, mainMenu, "Doser: [<] Back", "Set Doser Shakes", LCDMenu::RangeType::Nav, AccessoryType::DryDoser);

	//dose time
	AddMenu(doserTimeMenu, 0, doserTimeMenu, doserMenu, "Doser Time: [<] Back", "Not Set", LCDMenu::RangeType::TimeFrequency, AccessoryType::DryDoser);
	AddMenu(doserFreqMenu, 0, doserHourMenu, doserMenu, "Doser Frequency: [<] Back", "Frequency", LCDMenu::RangeType::Frequency, AccessoryType::DryDoser);
	AddMenu(doserHourMenu, 0, doserMinMenu, doserFreqMenu, "Doser Hour: [<] Back", "Hour", LCDMenu::RangeType::Hour, AccessoryType::DryDoser);
	AddMenu(doserMinMenu, 0, doserAmPmMenu, doserHourMenu, "Doser Minute: [<] Back", "Minute", LCDMenu::RangeType::Minute, AccessoryType::DryDoser);
	AddMenu(doserAmPmMenu, 0, doserTimeMenu, doserMinMenu, "Doser AM-PM: [<] Back", "AM PM", LCDMenu::RangeType::AmPm, AccessoryType::DryDoser);

	//doser shakes
	AddMenu(doserShakesMenu, 0, doserShakesMenu, doserMenu, "Doser Shakes: [<] Back", "Not Set", LCDMenu::RangeType::ShakesOrTurns, AccessoryType::DryDoser);
	AddMenu(doserShakesMenu, 1, doserSetShakesMenu, doserShakesMenu, "Doser Shakes: [<] Back", "Set Doser Shakes", LCDMenu::RangeType::Nav, AccessoryType::DryDoser);
	AddMenu(doserSetShakesMenu, 0, doserShakesMenu, doserShakesMenu, "Set Doser Shakes: [<] Back", "Not Set", LCDMenu::RangeType::SetShakesOrTurns, AccessoryType::DryDoser);


	//clock menus
	AddMenu(clockMenu, 0, clockMenu, mainMenu, "Clock: [<] Back", "Time", LCDMenu::RangeType::TimeLong, AccessoryType::Clock);
	AddMenu(clockMenu, 1, clockYearMenu, clockMenu, "Clock: [<] Back", "Set Clock Time", LCDMenu::RangeType::Nav, AccessoryType::Clock);
	AddMenu(clockYearMenu, 0, clockMonthMenu, clockMenu, "Clock Year: [<] Back", "Year", LCDMenu::RangeType::Year, AccessoryType::Clock);
	AddMenu(clockMonthMenu, 0, clockDayMenu, clockYearMenu, "Clock Month: [<] Back", "Month", LCDMenu::RangeType::Month, AccessoryType::Clock);
	AddMenu(clockDayMenu, 0, clockHourMenu, clockMonthMenu, "Clock Day: [<] Back", "Day", LCDMenu::RangeType::Day, AccessoryType::Clock);
	AddMenu(clockHourMenu, 0, clockMinMenu, clockDayMenu, "Clock Hour: [<] Back", "", LCDMenu::RangeType::Hour, AccessoryType::Clock);
	AddMenu(clockMinMenu, 0, clockAmPmMenu, clockHourMenu, "Clock Min: [<] Back", "", LCDMenu::RangeType::Minute, AccessoryType::Clock);
	AddMenu(clockAmPmMenu, 0, clockMenu, clockMinMenu, "Clock AM-PM: [<] Back", "", LCDMenu::RangeType::AmPm, AccessoryType::Clock);

}
String LCDMenuController::GetRangeOption(LCDMenu::RangeType rangeType, AccessoryType accType)
{

	if (rangeType == LCDMenu::RangeType::Frequency)
	{
		LimitRange(0, 1);
		if (_optionCount <= _lowerLimit)
			return "Daily";
		else if (_optionCount >= _upperLimit)
			return "Every Other Day";

	}
	else if (rangeType == LCDMenu::RangeType::Hour)
	{
		LimitRange(1, 12);
		String hour = GetOptionAsNumber("01", true);
		return hour;
	}
	else if (rangeType == LCDMenu::RangeType::Minute)
	{
		LimitRange(0, 59);

		String minute = GetOptionAsNumber("01", true);
		return minute;
	}
	else if (rangeType == LCDMenu::RangeType::AmPm)
	{
		LimitRange(0, 1);
		if (_optionCount <= _lowerLimit)
			return "AM";
		else if (_optionCount >= _upperLimit)
			return "PM";
	}
	else if (rangeType == LCDMenu::RangeType::TimeFrequency)
	{
		//Daily, 08:30AM
		String freq = GetTimeFrequency(accType);
		return freq;
	}
	else if (rangeType == LCDMenu::RangeType::TimeLong)
	{
		// 03/04/2016 08:30AM
		String time = GetTimeLong(accType);
		return time;
	}
	else if (rangeType == LCDMenu::RangeType::Year)
	{
		LimitRange(2016, 2050);
		String txt = GetOptionAsNumber("2016");

		return txt;
	}
	else if (rangeType == LCDMenu::RangeType::Month)
	{
		LimitRange(1, 12);
		String txt = GetOptionAsNumber("01", true);
		return txt;
	}
	else if (rangeType == LCDMenu::RangeType::Day)
	{
		LimitRange(1, 31);
		String txt = GetOptionAsNumber("01", true);
		return txt;
	}
	else if (rangeType == LCDMenu::RangeType::ShakesOrTurns)
	{
		int shakes = GetShakesOrTurns(accType);
		return String(shakes);
	}
	else if (rangeType == LCDMenu::RangeType::SetShakesOrTurns)
	{
		LimitRange(0, 13);

		String txt = GetOptionAsNumber("0");
		return txt;
	}
	//else if (rangeType == LCDMenu::RangeType::OutPin)
	//{
	//	//pin 14-19 are analog pins but can be used for digital output with SoftPWM
	//	//14 cant be used because it is used by lcd A0
	//	LimitRange(2, 19);
	//	if (_optionCount == 14)
	//		_optionCount++;

	//	String txt = String txt = GetOptionAsNumber("2");
	//	return txt;
	//}
	//else if (rangeType == LCDMenu::RangeType::InPin)
	//{
	//	//pin A0 is used by lcd.
	//	LimitRange(1, 5);

	//	String txt = String txt = GetOptionAsNumber("1");
	//	return txt;
	//}
	else
		return "";


}

void LCDMenuController::SaveRangeOption(LCDMenu::RangeType rangeType, AccessoryType accType)
{
	//SerialExt::Debug("save", selectedMenu.OptionText);

	if (rangeType == LCDMenu::RangeType::Frequency &&
		(accType == AccessoryType::Feeder ||
		accType == AccessoryType::DryDoser))
	{
		if (_optionCount == 0) //Daily
			RTCExt::SetRunEvery(24, accType);
		else // ot day
			RTCExt::SetRunEvery(48, accType);
	}
	else if ((rangeType == LCDMenu::RangeType::Hour ||
		rangeType == LCDMenu::RangeType::Minute ||
		rangeType == LCDMenu::RangeType::AmPm) &&
		(accType == AccessoryType::Feeder ||
		accType == AccessoryType::DryDoser))
	{
		RTCExt::SetNextRun(_optionCount, rangeType, accType);
	}
	else if (rangeType == LCDMenu::RangeType::TimeFrequency)
	{
		//Daily, 08:30AM
	}
	else if ((rangeType == LCDMenu::RangeType::Year ||
		rangeType == LCDMenu::RangeType::Month ||
		rangeType == LCDMenu::RangeType::Day ||
		rangeType == LCDMenu::RangeType::Hour ||
		rangeType == LCDMenu::RangeType::Minute) &&
		(accType == AccessoryType::Clock))
	{
		RTCExt::SetTimeTemp(_optionCount, rangeType);
	}
	else if (rangeType == LCDMenu::RangeType::AmPm &&
		accType == AccessoryType::Clock)
	{
		RTCExt::SetTimeTemp(_optionCount, rangeType);
		RTCExt::SetRTCTimeFromTemp();
	}
	else if (rangeType == LCDMenu::RangeType::SetShakesOrTurns &&
		(accType == AccessoryType::Feeder ||
		accType == AccessoryType::DryDoser))
	{
		SetShakesOrTurns(accType, _optionCount);
		//_lcdValCallBack(_optionCount);
	}
	//else if (rangeType == LCDMenu::RangeType::OutPin &&
	//	(accType == AccessoryType::Feeder ||
	//	accType == AccessoryType::DryDoser))
	//{
	//	//_lcdValCallBack(_optionCount);
	//}

}
//--Menu functions
void LCDMenuController::AddMenu(short id, short optionId, short nextMenuId, short prevMenuId, String text, String optionText, LCDMenu::RangeType rangeType, AccessoryType accType)
{
	LCDMenu menu(id, optionId, nextMenuId, prevMenuId, text, optionText, rangeType, accType);
	_menus.push_back(menu);
}
void LCDMenuController::AddMenu(short id, short optionId, short nextMenuId, short prevMenuId, String text, String optionText, LCDMenu::RangeType rangeType)
{
	AddMenu(id, optionId, nextMenuId, prevMenuId, text, optionText, rangeType, AccessoryType::None);
}

String LCDMenuController::GetOptionAsNumber(String defaultNumber){
	return GetOptionAsNumber(defaultNumber, false);
}
String LCDMenuController::GetOptionAsNumber(String defaultNumber, bool isTwoDigits){
	String txt = defaultNumber;
	if (_optionCount <= _upperLimit && _optionCount >= _lowerLimit)
	{
		txt = String(_optionCount);
		if (_optionCount < 10 && isTwoDigits)
			txt = "0" + txt;
	}
}

//get by ref example
//auto& menu = GetMenu(5);
LCDMenu LCDMenuController::GetMenu(short id, short optionId)
{
	auto selectedMenu = _menus[0];
	for (auto menu : _menus)
	{
		if (menu.Id == id && menu.OptionId == optionId)
			return menu;
	}

	return selectedMenu;
}
LCDMenu LCDMenuController::GetSelectedMenu()
{
	auto selectedMenu = GetMenu(_selectedMenuId, _selectedOptionId);
	return selectedMenu;
}
void LCDMenuController::SetSelectedMenu(LCDMenu menu)
{
	_selectedMenuId = menu.Id;
	_selectedOptionId = menu.OptionId;

}
void LCDMenuController::SetShakesOrTurns(AccessoryType accType, short shakesOrTurns){
	NextRunMemory& mem = RTCExt::FindNextRunInfo(accType);
	mem.ShakesOrTurns = shakesOrTurns;
}
int LCDMenuController::GetShakesOrTurns(AccessoryType accType){
	NextRunMemory& mem = RTCExt::FindNextRunInfo(accType);
	int shakes = mem.ShakesOrTurns;
	return shakes;
}

void LCDMenuController::PrintLine(short lineNum, String text)
{
	_lcd.setCursor(0, lineNum);
	_lcd.print(text);
}

void LCDMenuController::PrintMenu(LCDMenu menu)
{
	/*SerialExt::Debug("menu", menu.Text);
	SerialExt::Debug("option", menu.OptionText);
	*/

	//auto menu = GetSelectedMenu();
	//String optionText = menu.OptionText;
	String optionText = menu.OptionText;

	String rangeOptionText = GetRangeOption(menu.TheRangeType, menu.AccType);

	if (rangeOptionText != "")
	{
		optionText = rangeOptionText;
		//optionText = StaticUtils::ParseString(rangeOptionText);
	}

	_lcd.clear();
	PrintLine(0, menu.Text);
	PrintLine(1, optionText);
}

//template<typename T = void>
//void Clear()
//{
//	_lcd.clear();
//}

void LCDMenuController::SelectMainMenu()
{
	
	_selectedMenuId = 0;
	_selectedOptionId = 0;

	auto menu = GetSelectedMenu();
	PrintMenu(menu);

	while (_selectedMenuId > -1)
	{
		DetectKeyPress();
		delay(200);
	}
}

void LCDMenuController::ExitMainMenu()
{
	_selectedMenuId = -1;
	_selectedOptionId = -1;
}


void LCDMenuController::LimitRange(int lower, int upper)
{
	_lowerLimit = lower;
	_upperLimit = upper;
	if (_optionCount <= lower)
	{
		_optionCount = lower; //limit
	}
	else if (_optionCount >= upper)
	{
		_optionCount = upper; //limit
	}
}

void LCDMenuController::NextOption()
{

	if (_selectedOptionId >= 0)
	{
		short nextOptionId = _selectedOptionId + 1;
		auto nextMenu = GetMenu(_selectedMenuId, nextOptionId);
		if (nextMenu.Id == _selectedMenuId && nextMenu.OptionId == nextOptionId)
			_selectedOptionId = nextOptionId;
	}

	auto selectedMenu = GetSelectedMenu();

	_optionCount++;
	PrintMenu(selectedMenu);
}



void LCDMenuController::PreviousOption()
{

	if (_selectedOptionId > 0)
	{
		short prevOptionId = _selectedOptionId - 1;
		auto prevMenu = GetMenu(_selectedMenuId, prevOptionId);
		if (prevMenu.Id == _selectedMenuId && prevMenu.OptionId == prevOptionId)
			_selectedOptionId = prevOptionId;
		
	}

	auto selectedMenu = GetSelectedMenu();

	_optionCount--;
	PrintMenu(selectedMenu);
}


void LCDMenuController::LeftButton()
{
	//navigates back
	_optionCount = 0;
	auto selectedMenu = GetSelectedMenu();
	auto prevMenu = GetMenu(selectedMenu.PrevMenuId, 0);
	SetSelectedMenu(prevMenu);
	PrintMenu(prevMenu);
}




void LCDMenuController::SelectButton()
{

	auto selectedMenu = GetSelectedMenu();
	SaveRangeOption(selectedMenu.TheRangeType, selectedMenu.AccType);
	_optionCount = 0;

	auto nextMenu = GetMenu(selectedMenu.NextMenuId, 0);
	SetSelectedMenu(nextMenu);
	PrintMenu(nextMenu);

}


void LCDMenuController::DetectKeyPress()
{
	int key = GetKey();
	//SerialExt::Debug("key:", key);

	switch (key) {
	case 0: //right
		ExitMainMenu();
		break;
	case 1: //up
		NextOption();
		break;
	case 2: //down
		PreviousOption();
		break;
	case 3: //left
		LeftButton();
		break;
	case 4: //select
		SelectButton();
		break;
	default:
		break;

	}

}
void LCDMenuController::PrintTime()
{
	auto time = RTCExt::GetRTCTime();
	//auto theMonth = month(time);

	auto dateTimeString = RTCExt::GetShortDateTimeString(time);

	//SerialExt::Debug("dtStringp", dateTimeString);

	PrintLine(0, "Clock:");
	PrintLine(1, dateTimeString);

	delay(_scrollDelay);
}

void LCDMenuController::PrintRunInfo(AccessoryType accType)
{
	String label;
	NextRunMemory& nextRunMem = RTCExt::FindNextRunInfo(accType);

	if (accType == AccessoryType::Feeder)
		label = "Feed";
	else if (accType == AccessoryType::DryDoser)
		label = "Dose";
	
	if (nextRunMem.RunEvery == 0LL){
		PrintLine(0, label + " Not Set");
		return;
	}


	for (int i = 0; i <= 3; i++)
	{
		RTCExt::UpdateNextRun(accType);

		String nextRun;
		String lastRun;
		String countDown;

		lastRun = RTCExt::GetShortDateTimeString(nextRunMem.LastRun);
		countDown = RTCExt::GetTimeRemainingString(nextRunMem.CountDown);
		nextRun = RTCExt::GetShortDateTimeString(nextRunMem.NextRun);

		switch (i) {
		case 0:
			PrintLine(0, label + " Last Run:");
			PrintLine(1, lastRun);
			break;
		case 1:
			PrintLine(0, label + " Count Down:");
			PrintLine(1, countDown);
			break;
		case 2:
			PrintLine(0, label + " Next Run:");
			PrintLine(1, nextRun);
			break;
		default:
			break;
		}
		delay(_scrollDelay);
	}
}
void LCDMenuController::Scroll()
{
	_lcd.clear();

	switch (_scrollIndex) {
	case 0:
		PrintInstructions();
		break;
	case 1:
		PrintRunInfo(AccessoryType::Feeder);
		break;
	case 2:
		PrintRunInfo(AccessoryType::DryDoser);
		break;
	default:
		PrintTime();
		_scrollIndex = 0;
		return;
	}
	_scrollIndex++;

}
void LCDMenuController::PrintInstructions()
{
	PrintLine(0, "Hold [Select]");
	PrintLine(1, "for Menu");
	delay(_scrollDelay);
}
String LCDMenuController::GetTimeLong(AccessoryType accType)
{
	long time;

	if (accType == AccessoryType::Clock)
	{
		time = RTCExt::GetRTCTime();
	}

	String timeString = RTCExt::GetShortDateTimeString(time);
	return timeString;

}

String LCDMenuController::GetTimeFrequency(AccessoryType accType)
{
	long runEvery;
	long nextRun;

	NextRunMemory& nextRunMem = RTCExt::FindNextRunInfo(accType);

	runEvery = nextRunMem.RunEvery;
	nextRun = nextRunMem.NextRun;

	if (runEvery <= 0)
		return "Not Set";

	String freqTime = RTCExt::GetTimeFrequencyString(runEvery, nextRun);
	return freqTime;

}

//--key press
int LCDMenuController::GetKeyFromVal(unsigned int input) {
	int k;
	for (k = 0; k < _numOfKeys; k++)
	{
		if (input < _keyValues[k])
		{
			return k;
		}
	}
	if (k >= _numOfKeys) k = -1;
	return k;
}

int LCDMenuController::GetKey()
{
	int keyVal = analogRead(0);
	SerialExt::Debug("keyVal", keyVal);
	int key = GetKeyFromVal(keyVal);
	SerialExt::Debug("key", key);

	return key;
}