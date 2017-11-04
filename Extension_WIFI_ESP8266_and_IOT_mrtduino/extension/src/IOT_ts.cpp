#include "Arduino.h"
#include "IOT_ts.h"

Iot_device::Iot_device()
{
}

void Iot_device::Iot_thinkspeak(String WriteAPI)
{
	apiKeyThinkSpeak=WriteAPI;
}

void Iot_device::Iot_ifttt(String WriteAPI)
{
	apiKeyIfttt=WriteAPI;
}


void Iot_device::showResponse(int waitTime,bool logs){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (Serial1.available()){
        c=Serial1.read();
        if (logs) Serial.print(c);
      }
    }
                   
}

bool Iot_device::thingSpeakWrite(unsigned int field,float value,bool logs ){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  Serial1.println(cmd);
  if (logs) Serial.println(cmd);
  if(Serial1.find("Error")){
    if (logs) Serial.println("AT+CIPSTART error");
    return false;
  }
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKeyThinkSpeak;
  
  getStr +="&field"+String(field)+"=";
  getStr += String(value);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial1.println(cmd);
  if (logs)  Serial.println(cmd);
  
  delay(100);
  if(Serial1.find(">")){
    Serial1.print(getStr);
    if (logs)  Serial.print(getStr);
  }
  else{
    Serial1.println("AT+CIPCLOSE");
    // alert user
    if (logs)   Serial.println("AT+CIPCLOSE");
    return false;
  }
   delay(100);
  Serial1.println("AT+CIPCLOSE");
    // alert user
    if (logs)   Serial.println("AT+CIPCLOSE");
  
  return true;
}

bool Iot_device::iftttWrite(String event,float value1,float value2, float value3,bool logs ){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "maker.ifttt.com";                               // ifttt url
  cmd += "\",80";
  Serial1.println(cmd);
  if (logs) Serial.println(cmd);
  if(Serial1.find("Error")){
    if (logs) Serial.println("AT+CIPSTART error");
    return false;
  }

   
  String getStr = "GET /trigger/";   // prepare GET string
  getStr += event+"/with/key/";
  getStr += apiKeyIfttt+"?";
  getStr += "value1="+String(value1)+"&";
  getStr += "value2="+String(value2)+"&";
  getStr += "value3="+String(value3); 
  getStr += " HTTP/1.1\r\nHost: maker.ifttt.com";
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial1.println(cmd);
  if (logs)  Serial.println(cmd);
  
  delay(50);
  if(Serial1.find(">")){
    Serial1.print(getStr);
    if (logs)  Serial.print(getStr);
  }
  else{
    Serial1.println("AT+CIPCLOSE");
    // alert user
    if (logs)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  delay(100);
  Serial1.println("AT+CIPCLOSE");
   if (logs)   Serial.println("AT+CIPCLOSE");
  return true;
}


void Iot_device::Reset_ESP8266(int wait_time,bool logs)
{
  Serial1.println("AT+RST");         // Enable this line to reset the module;
   if (logs)
	 Serial.println("AT+RST");
  showResponse(wait_time,logs);

}

 void Iot_device::Baudios_ESP8266(int baudios,int wait_time,bool logs)
 {
  Serial1.println("AT+UART_CUR="+String(baudios)+",8,1,0,0"); 
  if (logs)
    Serial.println("AT+UART_CUR="+String(baudios)+",8,1,0,0");
  showResponse(wait_time,logs);
 }
 
  void Iot_device::Mode_ESP8266(unsigned int mode,int wait_time,bool logs)
 {
   Serial1.println("AT+CWMODE="+mode);    // Enable this line to set esp8266 serial speed to 9600 bps
   if (logs)
	Serial.println("AT+CWMODE="+mode);
   showResponse(wait_time,logs);
 }
 
void Iot_device::Credentials_ESP8266(String ssid,String password, int wait_time,bool logs)
 {
   Serial1.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password
   if (logs)
	Serial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");
   showResponse(wait_time,logs);
 }