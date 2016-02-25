
#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>


using namespace std;

//// <iostream> declares cout/cerr, but the application must define them
//// because it's up to you what to do with them.
//namespace std
//{
//  ohserialstream cout(Serial);
//}

vector<String> strings;

void setup(void)
{
  Serial.begin(9600);

  strings.push_back("Hello,");
  strings.push_back("world!");
  /*copy(strings.begin(),strings.end(),ostream_iterator<string>(cout," "));
  cout << endl;*/
}

void loop(void)
{
	Serial.println("here");
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
