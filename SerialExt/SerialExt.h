// SerialExt.h

#ifndef UTILS_h
#define UTILS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif



namespace Utils {

	enum AccessoryType : int
	{
		None,
		Clock,
		Feeder,
		LiqDoser,
		DryDoser,
		Pump,
		Lcd
	};

	

	////remeber: dependant functions must be defined first in namespace.
	///**Better to use template functions.
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
				SerialExt::Print(F("Incomming number: "), num);
				return num;
			}
			return 0;
		}
		//use debug prints for debug statements that should be removed, after debugging.
		template<typename T>
		extern void Debug(T&& text)
		{
			T t(text);
			String msg = String("~ ") + text;
			Serial.println(msg);
		}
		//use debug prints for debug statements that should be removed, after debugging.
		template<typename T, typename F>
		extern void Debug(T&& text, F&& val)
		{
			T t(text);
			F f(val);

			//auto msg = "~ " + text + ": " + val;
			String msg = String("~ ") + text;
			Serial.print(msg + ": ");
			Serial.println(val);
			Serial.flush();
		}

		
		
	};
	namespace MegaPins{
		const int PWM_2 = 2;
		const int PWM_3 = 3;
		const int PWM_4 = 4;
		const int PWM_5 = 5;
		const int PWM_6 = 6;
		const int PWM_7 = 7;
		const int PWM_8 = 8;
		const int PWM_9 = 9;
		const int PWM_10 = 10;
		const int PWM_11 = 11;
		const int PWM_12 = 12;
		const int PWM_13 = 13;
		const int PWM_44 = 44;
		const int PWM_45 = 45;
		const int PWM_46 = 46;

		//analog pins
		const int A_0 = 0;
		const int A_1 = 1;
		const int A_2 = 2;
		const int A_3 = 3;
		const int A_4 = 4;
		const int A_5 = 5;
		const int A_6 = 6;
		const int A_7 = 7;
		const int A_8 = 8;
		const int A_9 = 9;
		const int A_10 = 10;
		const int A_11 = 11;
		const int A_12 = 12;
		const int A_13 = 13;
		const int A_14 = 14; 
		const int A_15 = 15;

		//communication
		const int SDA_20 = 20;
		const int SCL_21 = 21;

		
	};
	namespace UnoPins{
		const int PWM_3 = 3;
		const int PWM_5 = 5;
		const int PWM_6 = 6;
		const int PWM_9 = 9;
		const int PWM_10 = 10;
		const int PWM_11 = 11;
		const int PWM_13 = 13;
	}
	namespace LCDPins{
		
		// LCD Analog Pins: A1, A2, A3, A4, A5
		const int A_1 = 1;
		const int A_2 = 2;
		const int A_3 = 3;
		const int A_4 = 4;
		const int A_5 = 5;

		// LCD Analog or Digital Pins: 15, 16, 17, 18, 19
		const int AD_1 = 15;
		const int AD_2 = 16;
		const int AD_3 = 17;
		const int AD_4 = 18;
		const int AD_5 = 19;

		// LCD Digital Pins: 0, 1, 2, 3, 11, 12, 13
		const int D_1 = 1;
		const int D_2 = 2;
		const int D_3 = 3;
		const int D_11 = 11;
		const int D_12 = 12;
		const int D_13 = 13;

	};
};


#endif

