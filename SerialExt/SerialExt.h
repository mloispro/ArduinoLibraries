// SerialExt.h

#ifndef UTILS_h
#define UTILS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace Utils {
	namespace SerialExt{

		//T& will only take references, T&& will take ref, pointer, or val, so always do this.
		
		//template<typename T = void>
		//void Init()
		//{
		//	// initialize serial:
		//	Serial.begin(9600);
		//	//Wait for four seconds or till data is available on serial, whichever occurs first.
		//	//while (!Serial);
		//	while (Serial.available() == 0 && millis() < 2000);
		//}
		
		template<typename T>
		extern void Print(T&& text)
		{
			T t(text);
			Serial.println(text);
		}
		template<typename T, typename F>
		extern void Print(T&& text, F&& val)
		{
			T t(text);
			F f(val);
			Serial.print(text);
			Serial.println(val);
			Serial.flush();
		}
		template<typename T, typename F, typename G>
		extern void Print(T&& text, F&& val, G&& val2)
		{
			T t(text);
			F f(val);
			G g(val2);
			Serial.print(text);
			Serial.print(val);
			Serial.println(val2);
		}
		
		//static return example
		template <int... T>
		extern auto Read() -> int{
			
			int num;
			while (Serial.available() > 0) {
				//int incomingbyte = Serial.read(); Serial.readString();// read the incoming byte:
				
				int option = Serial.parseInt();
				long val = 0;
				String valString = "";
				if (Serial.read() == '-') {
					val = Serial.parseInt();
					valString = String(val);
				}
				
				String optionString(option);
				String numString = optionString + valString;
				num = numString.toInt();

				if (Serial.read() == '\n') {
					break;
				}

				//int num = 5;

				//SerialExt::Print("Incomming Text: ", incomingbyte);
			}
			if (num > 0){
				SerialExt::Print("Incomming number: ", num);
				return num;
			}
			return 0;
		}
		//use debug prints for debug statements that should be removed, after debugging.
		template<typename T>
		extern void Debug(T&& text)
		{
			T t(text);
			String msg = String("~DEBUG~ ") + text;
			Serial.println(msg);
		}
		//use debug prints for debug statements that should be removed, after debugging.
		template<typename T, typename F>
		extern void Debug(T&& text, F&& val)
		{
			T t(text);
			F f(val);
			String msg = String("~DEBUG~ ") + text;
			Serial.print(msg);
			Serial.println(val);
			Serial.flush();
		}
		
	}
}


#endif

