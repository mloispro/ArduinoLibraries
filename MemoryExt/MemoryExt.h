// SerialExt.h

#ifndef EEPROMMEM_h
#define EEPROMMEM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif



#include <StandardCplusplus.h>
#include <string>
#include <vector>
using namespace std;

#include <TimeLib.h>
#include <Time.h>
#include <EEPROM.h>
//#include <EEPROMVar.h>
//#include <EEPROMex.h>
#include <MemoryFree.h>

#include "SerialExt.h"
//#include "RTCExt.h"
using namespace Utils;

namespace Models{

	struct NextRunMemory{
		int AccType; //id
		long LastRun;
		long NextRun;
		long CountDown;
		long RunEvery = 0;
		short ShakesOrTurns;
		long LastSave;
	};
	struct MemAddress{
		int AccType; //id
		int Index;
	};
};
namespace Utils{
	using namespace Models;

	namespace MemoryExt{
		static vector<MemAddress> _memAddresses;

		template<typename T = String, typename M = NextRunMemory>
		void PrintNextRunMemory(T&& label, M&& mem)
		{
			T t(label);
			M m(mem);

			SerialExt::Debug(F("mem"), label);
			SerialExt::Debug(F("AccType"), mem.AccType);
			SerialExt::Debug(F("CountDown"), mem.CountDown);
			SerialExt::Debug(F("LastRun"), mem.LastRun);
			SerialExt::Debug(F("LastSave"), mem.LastSave);
			SerialExt::Debug(F("NextRun"), mem.NextRun);
			SerialExt::Debug(F("RunEvery"), mem.RunEvery);
			SerialExt::Debug(F("ShakesOrTurns"), mem.ShakesOrTurns);
		}

		template<typename T>
		int GetUpdateIndex(T&& mem){
			T t(mem);

			int index = 0;
			int memSize = sizeof(NextRunMemory);

			SerialExt::Debug(F("memSize"), memSize);
			SerialExt::Debug(F("_memAddresses.size"), _memAddresses.size());

			if (_memAddresses.size() <= 0)
				_memAddresses.push_back(MemAddress{ mem.AccType, memSize });

			SerialExt::Debug(F("_memAddresses.size1"), _memAddresses.size());

			auto& memAddress = _memAddresses[0];
			bool foundMem = false;
			int agregateMemSize = 0;
			for (auto& memAddr : _memAddresses)
			{
				SerialExt::Debug(F("memAddr.Index"), memAddr.Index);
				agregateMemSize += memAddr.Index; //totals incase we have to add a new one
				if (memAddr.AccType == mem.AccType){
					index = memAddr.Index - memSize;
					foundMem = true;
					break;
				}
			}

			SerialExt::Debug(F("foundMem"), foundMem);
			SerialExt::Debug(F("agregateMemSize"), agregateMemSize);

			if (!foundMem){ //if added, dont re-add
				agregateMemSize += memSize;
				_memAddresses.push_back(MemAddress{ mem.AccType, agregateMemSize });
				index = agregateMemSize;

				SerialExt::Debug(F("foundMem_memAddresses.size"), _memAddresses.size());
			}

			SerialExt::Debug(F("_memAddresses.size2"), _memAddresses.size());
			SerialExt::Debug(F("index"), index);
			return index;

		}

		template<typename T>
		T& SaveNextRunMem(T&& mem){
			T t(mem);

			SerialExt::Debug("--SaveNextRunMem--");

			int index = GetUpdateIndex(mem);
			mem.LastSave = now();

			PrintNextRunMemory("mem_Save1", mem);

			NextRunMemory eEEPROMmem = EEPROM.put(index, mem);

			PrintNextRunMemory("eEEPROMmem_Save1", eEEPROMmem);

			return eEEPROMmem;
		}
		template<typename T>
		T& GetNextRunMem(T&& mem){
			T t(mem);

			SerialExt::Debug("--GetNextRunMem--");

			int index = GetUpdateIndex(mem); //Move address to the next byte.

			PrintNextRunMemory("mem_Get1", mem);

			NextRunMemory eEEPROMmem; //Variable to store custom object read from EEPROM.
			EEPROM.get(index, eEEPROMmem);

			PrintNextRunMemory("eEEPROMmem_Get1", eEEPROMmem);

			return eEEPROMmem;
		}

		

		//retuns first if none have been saved.
		//MemAddress& MemoryController::FindAddress(AccessoryType accType){
		//	
		//	if (_memAddresses.size() <= 0)
		//		_memAddresses.push_back(MemAddress{ accType, 0 });
		//
		//	auto& memAddress = _memAddresses[0];
		//	for (auto& mem : _memAddresses)
		//	{
		//		if (mem.AccType == accType)
		//			return mem;
		//	}
		//
		//	return memAddress;
		//}

		
		template<typename T>
		extern void PrintFreeMemory(T&& text)
		{
			T t(text);

			String msg = String(" - ") + text;
			int mem = freeMemory();
			Serial.print(msg);
			Serial.print(F("Free Mem : "));
			Serial.println(mem);

		}

		
	};

};

#endif

