/*

  Library to connect with cloud of bots  from  http://cloud.makeblock.es


  Version: 1.6

  Developed by Jose Carlos from makeblock.es ( jose@makeblock.es )

*/
#include "Arduino.h"
 
#ifndef CLOUD_BOT_H
#define CLOUD_BOT_H

#include "SoftwareSerial.h"
#include "StreamSend.h"

#define CLOUD_BOT_VERSION     16
#define WIFI_BAUD_RATE        19200

#define CLOUD_BOT_RET_STR     0
#define CLOUD_BOT_RET_INT     1

#ifndef CLOUD_BOT_SERIAL_TYPE
#define CLOUD_BOT_SERIAL_TYPE
 struct CLOUD_BOT_SERIAL {
      uint16_t act;  
      char data[55];
      int32_t num;
   } __attribute__((__packed__));
#endif

char concat_result[55] = "";
char* cloud_util_concat(char *s1, char *s2){
  if(strlen(s1)+strlen(s2) >54 ) return "cloud_err_overflow";
  sprintf(concat_result, "%s@%s", s1, s2); 
  return concat_result;
} 
 
class cloud {
  private:
    
    static SoftwareSerial *mySerial;
    
 
  public:
    static void begin(int rx, int tx);
    static void begin(uint8_t act);
    
    static void SerialSendData( uint16_t act, char* data, int32_t num );
    static int  CheckResponse(int act);
    static CLOUD_BOT_SERIAL getResponse( uint16_t act, char* data, char* value   );
    static CLOUD_BOT_SERIAL getResponse( uint16_t act, char* data, char* value , int32_t num  );

    static void   wifiConnect();
    static int    wifiVersion()         { return String(getResponse(1, "", NULL).data).toInt();  } 
    static int    wifiConnected()       { return getResponse(4, "", NULL).num;   }
    static int    cloudPing()           { return getResponse(5, "", NULL).num;  }
    static String lastError()           { return String(getResponse(6, "", NULL).data);  }

    static char* login(String apikey)
      { return getResponse(501, (char*) apikey.c_str(), NULL).data; }
   
    static String  myID() 
      { return String(getResponse(600, "", NULL).data);  }
    
    static char* setData(String who, String prop, String val) 
      { if(val.length()>54)  return "error_value_more_than_55_chars";
        return getResponse(10000, cloud_util_concat((char*)prop.c_str(), (char*)who.c_str())  , (char*)val.c_str()).data; }
    static char* setData(String who, String prop, double val) 
      {   String buf = String(val); 
          buf.replace(".00", "");
          return cloud::setData(who, prop, buf);
      }    
    static char* setData(String prop, String val){ return cloud::setData("me", prop, val); }
    static char* setData(String prop, double val){ return cloud::setData("me", prop, val); }
      
    static String getData(String who, String prop) 
      { return String(getResponse(1000, cloud_util_concat((char*)prop.c_str(), (char*)who.c_str())   , NULL).data); }
    static String getData(String prop) { return cloud::getData("me", prop);}

    static char* delData(String prop) 
      { return getResponse(1100, cloud_util_concat((char*)prop.c_str(), "me"), NULL).data; }

    static int count(String who, String prop) 
      { return String(getResponse(1010, cloud_util_concat((char*)prop.c_str(), (char*)who.c_str()), NULL).data).toInt(); }  
    static int count(String prop)  { return cloud::count("me", prop);} 

    static String getDataAt(String who, String prop, uint32_t num) 
      { return String(getResponse(5000, cloud_util_concat((char*)prop.c_str(), (char*)who.c_str())  , NULL, num).data); }   

    static String getDataAt(String prop, uint32_t num) { return cloud::getDataAt("me", prop, num);}    

  
    static CLOUD_BOT_SERIAL cloudTX;
    static CLOUD_BOT_SERIAL cloudRX;
    static bool reception ; //- 1 en el momento que recibo algo del wifi
    static unsigned long recvTimeOut;
 
};
 

SoftwareSerial *cloud::mySerial = 0; 
CLOUD_BOT_SERIAL cloud::cloudRX = {0,"",0};
CLOUD_BOT_SERIAL cloud::cloudTX = {0,"",0};
bool cloud::reception =0; 
unsigned long cloud::recvTimeOut=0;
 

void cloud::begin(int rx, int tx) {
  mySerial = new SoftwareSerial(tx, rx);
  mySerial->begin( WIFI_BAUD_RATE);
}

#ifdef MEPORT_H_
void cloud::begin(uint8_t port) {
  mySerial = new SoftwareSerial(mePort[port].s2, mePort[port].s1);
  mySerial->begin( WIFI_BAUD_RATE);
}
#endif



CLOUD_BOT_SERIAL cloud::getResponse( uint16_t act, char* datax, char* value, int32_t num  ){
 
  //-  hacer 2 envios serial
  if( *value && value!=NULL){
    //- 10 de acumulacion de datos
    SerialSendData(10, value, 0); 
  }

  SerialSendData(act, datax, num);
  mySerial->listen();
 
  int check2= CheckResponse(act);
  if(check2==1) return cloudRX;
  if(check2==-2){
      strcpy( cloudRX.data, "ERR_NO_RESPONSE" );
      return cloudRX;  
  } 
    
  strcpy( cloudRX.data, "ERR_CORRUPT_DATA" );
  
  return cloudRX;
  
}  

CLOUD_BOT_SERIAL cloud::getResponse( uint16_t act, char* datax, char* value  ){
  return cloud::getResponse(act, datax, value, 0);
}

void cloud::SerialSendData( uint16_t act, char* val, int32_t num ){
  
  while(mySerial->available() > 0) {
    char t = mySerial->read();
  }
  
  cloudTX.act = act;
     
  strcpy( cloudTX.data, val );
      
  cloudTX.num = num;
      
  reception   = 0;
  recvTimeOut = millis();
  cloudRX.act = 0;

  StreamSend::sendObject(*mySerial, &cloudTX, sizeof(cloudTX));
}


int cloud::CheckResponse(int act){

    while( millis() - recvTimeOut<2500 ){
      byte packetResults = StreamSend::receiveObject(*mySerial, &cloudRX, sizeof(cloudRX));
      if(StreamSend::isPacketGood(packetResults)) {
          reception = 1;
          if(cloudRX.act==act){
            return 1;  // respuesta esperada
          }else{
            return 0;  // paquete recibido pero respuesta no esperada  
          }
      }

      if(StreamSend::isPacketCorrupt(packetResults) || !StreamSend::isPacketNotFound(packetResults) )  return -1;

      
    }

    return -2; // timeout
    
}



void cloud::wifiConnect(){
  SerialSendData(2, "", 0);
}





 
#endif
