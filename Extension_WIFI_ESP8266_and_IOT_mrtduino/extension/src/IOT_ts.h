
#ifndef IOT_ts_h
#define IOT_ts_h

#include "Arduino.h"


class Iot_device
{
 
  public:
    Iot_device();
	void Iot_thinkspeak(String);
	void Iot_ifttt(String);
    bool thingSpeakWrite(unsigned int,float ,bool );
	bool iftttWrite(String,float value1,float value2,float value3,bool);
	void Reset_ESP8266(int ,bool );
    void Baudios_ESP8266(long int,int,bool);
    void Mode_ESP8266(unsigned int ,int ,bool);
	void Credentials_ESP8266(String ,String,int,bool);
	private:
       // replace with your channel's thingspeak WRITE API key
    void showResponse(int ,bool );
    String apiKeyThinkSpeak;
	String apiKeyIfttt;
};



#endif