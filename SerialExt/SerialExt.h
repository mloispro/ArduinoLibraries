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
			if (Serial.available() > 0) {
				int incomingbyte = Serial.read(); // read the incoming byte:
				//T t(incomingbyte);
				SerialExt::Print("Incomming byte: ", incomingbyte);
				return incomingbyte;
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
		}
		
	}
}


#endif

