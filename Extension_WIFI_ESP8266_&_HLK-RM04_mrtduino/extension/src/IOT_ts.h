
#ifndef IOT_ts_h
#define IOT_ts_h

#include "Arduino.h"


class Iot_thinkspeak
{
 
  public:
    Iot_thinkspeak(String);
    bool thingSpeakWrite(unsigned int,float ,bool );
	  void Reset_ESP8266(int ,bool );
    void Baudios_ESP8266(int,int,bool);
    void Mode_ESP8266(unsigned int ,int ,bool);
	  void Credentials_ESP8266(String ,String,int,bool);
	private:
       // replace with your channel's thingspeak WRITE API key
    void showResponse(int ,bool );
    String apiKey;
};



#endif