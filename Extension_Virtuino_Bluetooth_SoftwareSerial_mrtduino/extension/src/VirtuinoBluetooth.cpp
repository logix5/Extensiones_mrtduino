/* Virtuino bluetooth library
  * Created by Ilias Lamprou
 * Updated May 1 2017
 * 
 * Download latest Virtuino android app from the link: https://play.google.com/store/apps/details?id=com.virtuino_automations.virtuino
 * Video tutorial link: https://www.youtube.com/watch?v=CYR_jigRkgk
 * Contact address for questions or comments: iliaslampr@gmail.com
 */

/*========= VirtuinoBluetooth Class methods  
*  vPinMode(int pin, int state)                                  set pin as digital OUTPUT or digital INPUT.  (Insteed default pinMode method
*
*========= Virtuino General methods  
*  void vDigitalMemoryWrite(int digitalMemoryIndex, int value)   write a value to a Virtuino digital memory   (digitalMemoryIndex=0..31, value range = 0 or 1)
*  int  vDigitalMemoryRead(int digitalMemoryIndex)               read  the value of a Virtuino digital memory (digitalMemoryIndex=0..31, returned value range = 0 or 1)
*  void vMemoryWrite(int analogMemoryIndex, float value)         write a value to Virtuino float memory       (memoryIndex=0..31, value range as float value)
*  float vMemoryRead(int analogMemoryIndex)                      read the value of  Virtuino analog memory    (analogMemoryIndex=0..31, returned value range = 0..1023)
*  run()                                                         neccesary command to communicate with Virtuino android app  (on start of void loop)
*  int getPinValue(int pin)                                      read the value of a Pin. Usefull for PWM pins
*/


#include "VirtuinoBluetooth.h"

 

//================================================================== VirtuinoBluetooth init
#ifdef BLUETOOTH_USE_SOFTWARE_SERIAL
VirtuinoBluetooth::VirtuinoBluetooth(SoftwareSerial &uart, uint32_t baud): BTserial(&uart){
    BTserial->begin(baud);
    while (BTserial->available()) BTserial->read();
}

VirtuinoBluetooth::VirtuinoBluetooth(SoftwareSerial &uart): BTserial(&uart){}
#else
VirtuinoBluetooth::VirtuinoBluetooth(HardwareSerial &uart, uint32_t baud): BTserial(&uart){
    BTserial->begin(baud);
    while (BTserial->available()) BTserial->read();
}

VirtuinoBluetooth::VirtuinoBluetooth(HardwareSerial &uart): BTserial(&uart){}    

#endif

//================================================================== run
void  VirtuinoBluetooth::run() {
  checkIfIOsHaveChanged();                                   // This void informs VirtUino app every time a pin's status is changed 
  if (BTserial->available()) readBluetoothSerialData();      // while serial buffer is not empty we read and store the character to commandBuffer
  
}

//================================================================= checkIfIOsHaveChanged
void VirtuinoBluetooth::checkIfIOsHaveChanged(){
   int pin;
    int p;
    int pinValue;
       if (!connectedStatus) return;
        //---- check if digitalInput state has changed
         for (int i=0;i<bt_arduinoPinsSize; i++){
           if ((arduinoPinsMap[i]>0) && (arduinoPinsMap[i]<3)) {
              pinValue=digitalRead(i);
              if (arduinoPinsValue[i]!=pinValue){
                  arduinoPinsValue[i]=pinValue;
                  if ((arduinoPinsMap[i]==1) || (arduinoPinsMap[i]==2)) sendCommandResponse('Q',i,String(pinValue));
               }
           }
         } // end for  
        
       //---- check if digitalMemory has changed
          for (int i=0;i<bt_virtualDigitalMemorySize; i++){
           if (virtualDigitalMemoryIdol[i] != virtualDigitalMemory[i]){
              virtualDigitalMemoryIdol[i] = virtualDigitalMemory[i];
              sendCommandResponse('D',i,String(virtualDigitalMemory[i]));
           } 
          }

}
//================================================================= readBluetoothSerialData
//This void reads data from bluetooth serial and stores them to commandBuffer
//If we have a valid  command from app, void then calls the void executeReceivedCommand to execute the command
//If command doesn't have a valid format it returns an error response to App  - error code = 0
//This is a system fuction. Don't change this code

  void VirtuinoBluetooth::readBluetoothSerialData(){
        
        while (BTserial->available()) {
             char tempChar=BTserial->read();
             if (DEBUG2) Serial.print(tempChar);
             if (tempChar==bt_COMMAND_START_CHAR) {                           // a new command is starting...
             if (newCommand==false) newCommand=true;   
             else sendCommandResponse('E',0,String(bt_ERROR_COMMAND));
             commandBuffer="";                             //clear buffer for the new command
        }
        if (newCommand){                                // add serial charracters to  new command
            commandBuffer+=tempChar; 
            if (commandBuffer.length() > 100){             //error  while reading data
              newCommand=false;                           // cancel active command 
              commandBuffer="";                           // clear buffer for the next command
              break;
              sendCommandResponse('E',0,String(bt_ERROR_COMMAND));
            } 
            if (tempChar==bt_COMMAND_END_CHAR){                          // end of command
                String response=checkBluetoothCommand(commandBuffer);
                if (DEBUG2) Serial.print("\r\nResponse= "+response+"\r\n");
                BTserial->print(response);
                newCommand=false;
                commandBuffer="";
                break;
            }
        }}
  }
  
//====================================================================================== checkBluetoothCommand
String  VirtuinoBluetooth::checkBluetoothCommand(String command){
  String returnedString="";
  int pos=command.lastIndexOf(bt_COMMAND_END_CHAR);     
  if (pos>5){                                          // check the size of command
      String command1= command.substring(0,pos+1);     // clear the command
      if (DEBUG2) Serial.println("\r\nCommand = "+ command1);

           while (command1.indexOf(bt_COMMAND_START_CHAR)>=0){
              int cStart=command1.indexOf(bt_COMMAND_START_CHAR);
              int cEnd=command1.indexOf(bt_COMMAND_END_CHAR);
              if (cEnd-cStart>5){
              String oneCommand = command1.substring(cStart+1,cEnd);                               // get one command
                char commandType = getCommandType(oneCommand);
                  if (commandType!='E') {
                     int pin= getCommandPin(oneCommand);
                     if (pin!=-1){
                        boolean returnInfo=false;
                        float value=0;
                        if (oneCommand.charAt(4)=='?') returnInfo=true;
                        else value = getCommandValue(oneCommand);
                        String commandResponse=executeReceivedCommand(commandType,pin , value,returnInfo);
                        lastCommunicationTime=millis();
                        returnedString += commandResponse;
                     } else  returnedString +=getErrorCommand(bt_ERROR_PIN);  // response  error pin number   !E00=1$   
                  } else returnedString +=getErrorCommand(bt_ERROR_TYPE);     // response  error type   !E00=3$   
          
              } else{
                returnedString +=getErrorCommand(bt_ERROR_SIZE);  // response  error size of command   !E00=4$   
              }
              command1=command1.substring(cEnd+1);
           }    // while
     
  }
  else  returnedString=getErrorCommand(bt_ERROR_SIZE);         // the  command is not correct
 
  return returnedString;
}




//================================================================= executeReceivedCommand
  String VirtuinoBluetooth::executeReceivedCommand(char activeCommandType, int activeCommandPin ,float  activeCommandValue, boolean returnInfo){
    //------------------------------------
   // Every time we have a complete command from bluetooth the global value commandState is set to 1 
   // The value activeCommandType contains command type such as Digital output, Analog output etc.
   // The value activeCommandPin contains the pin number of the command
   // The value activeCommandValue contains the value (0 or 1 for digital, 0-255 for analog outputs, 0-1023 for analog memory) 
   // In this void we have to check if the command is correct and then execute the command 
   // After executing the command we have to send a response to app
   // At last we must reset the command state value to zero 
   //This is a system fuction. Don't change this code
      
    String response=getErrorCommand(bt_ERROR_UNKNOWN); 
    String pinString="";
    
    if (activeCommandPin<10) pinString = "0"+String(activeCommandPin);
    else pinString=String(activeCommandPin);
   
    switch (activeCommandType) {
      case 'I':                      
            if ((activeCommandPin>=0) & (activeCommandPin < bt_arduinoPinsSize))
              response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+bt_COMMAND_END_CHAR;  // response 
            else   response =getErrorCommand(bt_ERROR_PIN);  // response  error pin number   !E00=1$       
          break;
      case 'Q': 
            if ((activeCommandPin>=0) & (activeCommandPin < bt_arduinoPinsSize)){
                 if (returnInfo) response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+bt_COMMAND_END_CHAR;  // response 
                 else 
                   if ((activeCommandValue == 0) || (activeCommandValue == 1)) {
                          digitalWrite(activeCommandPin,activeCommandValue);
                          arduinoPinsValue[activeCommandPin]=activeCommandValue;
                          //virtualDigitalMemoryIdol[activeCommandPin]=activeCommandValue;
                          response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(digitalRead(activeCommandPin))+bt_COMMAND_END_CHAR;  // response 
                   } else    response =getErrorCommand(bt_ERROR_VALUE);
            } else   response =getErrorCommand(bt_ERROR_PIN);  // response  error pin number   !E00=1$   
          break; 

       case 'D':
            if ((activeCommandPin>=0) & (activeCommandPin<bt_virtualDigitalMemorySize)){ 
                if (returnInfo) response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualDigitalMemory[activeCommandPin])+bt_COMMAND_END_CHAR;  // response 
                else{
                      virtualDigitalMemory[activeCommandPin]= activeCommandValue; 
                      virtualDigitalMemoryIdol[activeCommandPin]= activeCommandValue; 
                      response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualDigitalMemory[activeCommandPin])+bt_COMMAND_END_CHAR;  // response 
                } //else    response =getErrorCommand(bt_ERROR_VALUE);
            } else   response =getErrorCommand(bt_ERROR_PIN);  // response  error pin number   !E00=1$   
            break; 
      
       case 'V': 
           if ((activeCommandPin>=0) & (activeCommandPin<bt_virtualAnalogMemorySize)){
                if (returnInfo) response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualFloatMemory[activeCommandPin])+bt_COMMAND_END_CHAR;  // response
               else { 
                    virtualFloatMemory[activeCommandPin]= activeCommandValue; 
                    response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(virtualFloatMemory[activeCommandPin])+bt_COMMAND_END_CHAR;  // response
                    } 
           } else   response =getErrorCommand(bt_ERROR_PIN);
          break;
       case 'O': 
           if ((activeCommandPin>=0) & (activeCommandPin < bt_arduinoPinsSize)){
                   
                   if (returnInfo) {
                      int pwm_value = pulseIn(activeCommandPin, HIGH);
                      pwm_value = pwm_value /7.85;
                      response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String(pwm_value)+bt_COMMAND_END_CHAR;  // response 
                  }
                   else if ((activeCommandValue>=0) && (activeCommandValue<256)) {
                      arduinoPinsMap[activeCommandPin]=3; 
                      arduinoPinsValue[activeCommandPin]=(int) activeCommandValue;
                      analogWrite(activeCommandPin,(int)activeCommandValue);
                      response =bt_COMMAND_START_CHAR+String(activeCommandType)+pinString+"="+String((int)activeCommandValue)+bt_COMMAND_END_CHAR;  // response 
                  } else response =getErrorCommand(bt_ERROR_VALUE);
           } else   response =getErrorCommand(bt_ERROR_PIN);;
          break;

         case 'A':                        
            if ((activeCommandPin>=0) & (activeCommandPin < arduinoAnalogPinsSize))
              response ="!"+String(activeCommandType)+pinString+"="+String(analogRead(analogInputPinsMap[activeCommandPin]))+"$";  // response 
            else   response =getErrorCommand(bt_ERROR_PIN);  // response  error pin number   !E00=1$       
          break;  
         
           case 'C':                   
            if ((activeCommandPin>=0) & (activeCommandPin <=1)){          //0 connect via bluetooth - 1 connect via network 
               if (activeCommandValue==1) {
                  connectedStatus=true;
                 response ="!C"+pinString+"="+bt_firmwareCode+"$";
               }
              else if (activeCommandValue==0) {
                connectedStatus=false;
                response =bt_COMMAND_START_CHAR+"C"+pinString+"=0"+bt_COMMAND_END_CHAR;
               }
              else      response =getErrorCommand(bt_ERROR_VALUE);;
              
            } else   response =getErrorCommand(bt_ERROR_COMMAND);        
          break;  
           }
           
        return response;
       
  }

  //============================================================================== 
  
 //This function returns the value of the command string which was received from app
 //For digital output commands returns 0 or 1. 
 //For digital memory commands returns 0 or 1. 
 //For analog output commands returns a value between 0-255 
 //For analog memory commands returns a value between 0-1023
 //If value is out of the corect range it returns -1 as error code
 //This is a system function. Don't change this code
 
//================================================================= getCommandValue
float VirtuinoBluetooth::getCommandValue(String aCommand){
     float value=0;
     String valueString=aCommand.substring(4);
     boolean er=false;
       for (int i=0;i<valueString.length();i++){
        char c=valueString.charAt(i);
        if (! ((c=='.') || (c=='-') || (isDigit(valueString.charAt(i))))) er=true;
       }
       if (er==false) value=valueString.toFloat();
      
    return value;
  }

 
   //================================================================= getCommandType
  //This function returns the command type which was received from app
  //The second character of command string determines the command type
  // I  digital input read command    
  // Q  digital ouput write
  // D  digital memory read - write commang
  // A  analog input analog input read command
  // O  analog output write command
  // V  analog memory read - write command
  // C  connect or disconect command    
  // E  error
  
  // Other characters are recognized as an error and function returns the character E
  //This is a system fuction. Don't change this code
  
  char  VirtuinoBluetooth::getCommandType(String aCommand){
   char commandType= aCommand.charAt(0);
    if (!(commandType == 'I' || commandType == 'Q' || commandType == 'D' ||commandType == 'A' ||commandType == 'O' || commandType == 'V' || commandType == 'C')){
      commandType='E'; //error  command
    }
    return commandType;
  }
  //================================================================= getCommandPin
  //This function returns the pin number of the command string which was received from app
  //Fuction checks if pin number is correct. If not it returns -1 as error code
  //This is a system fuction. Don't change this code
 
  int  VirtuinoBluetooth::getCommandPin(String aCommand){
    char p1= aCommand.charAt(1);
    char p2= aCommand.charAt(2);
    String s="-1";
    if (isDigit(p1) && isDigit(p2)) {
       s="";
       s+=p1;
       s+=p2;
    }
     return s.toInt();
  }


 //==================================================================  sendCommandResponse  
 void  VirtuinoBluetooth::sendCommandResponse(char commandType, int commandPin , String commandValueString){
    if (!connectedStatus) return;
    String s="";
    s+=bt_COMMAND_START_CHAR;
    s+=commandType;
    if (commandPin<10) s+='0';
    s=s+String(commandPin);
    s+='=';
    s+=commandValueString;
    s+=bt_COMMAND_END_CHAR;
    if (DEBUG2) Serial.print("\r\nResponse= "+s+"\r\n");
    BTserial->print(s);
   
    delay(10);              
}


//=================================================================== getErrorCommand
String VirtuinoBluetooth::getErrorCommand(byte code){
  return bt_COMMAND_START_CHAR+bt_COMMAND_ERROR+String(code)+bt_COMMAND_END_CHAR;
}
//=================================================================== vDigitalMemoryWrite
 void VirtuinoBluetooth::vDigitalMemoryWrite(int digitalMemoryIndex, int value){
  if ((digitalMemoryIndex>=0) && (digitalMemoryIndex<bt_virtualDigitalMemorySize)){
    virtualDigitalMemory[digitalMemoryIndex]=value;
  }
  
}
//=================================================================== vDigitalMemoryRead
 int VirtuinoBluetooth::vDigitalMemoryRead(int digitalMemoryIndex){
  if ((digitalMemoryIndex>=0) & digitalMemoryIndex<bt_virtualDigitalMemorySize){
    return virtualDigitalMemory[digitalMemoryIndex];
  }
  else return 0;    // error
}

//=================================================================== vMemoryWrite
void VirtuinoBluetooth::vMemoryWrite(int memoryIndex, float value){
  if ((memoryIndex>=0) & memoryIndex<bt_virtualAnalogMemorySize){
      virtualFloatMemory[memoryIndex]=value;
  }
}

//=================================================================== vMemoryRead
 float VirtuinoBluetooth::vMemoryRead(int memoryIndex){
  if ((memoryIndex>=0) & memoryIndex<bt_virtualAnalogMemorySize){
    return virtualFloatMemory[memoryIndex];
  }
  else return 0;    // error
}


//=================================================================== getPinValue
int VirtuinoBluetooth::getPinValue(int pin){
  if (pin>=0 && pin<bt_arduinoPinsSize) return arduinoPinsValue[pin];
  else return 0;
  }

//=================================================================== vPinMode
void VirtuinoBluetooth::vPinMode(int pin, int mode){
  if (pin>=0 && pin<bt_arduinoPinsSize){
    pinMode(pin,mode);
    if (mode == OUTPUT ) arduinoPinsMap[pin] =  1;
      else if (mode == INPUT ) arduinoPinsMap[pin] =  2;
  }
}


//=================================================================== vDelay
void VirtuinoBluetooth::vDelay(long milliseconds){
  long timeStart=millis();
  while (millis()-timeStart<milliseconds) run();
}



