#include "Arduino.h"
#include "IOT_ts.h"

Iot_device::Iot_device(String WriteAPI)
{
	apiKey=WriteAPI;
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
  getStr += apiKey;
  
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