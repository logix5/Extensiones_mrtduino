#include <Arduino.h>
#include <Wire.h>


#include <MrtDuino.h>

double Estado;
char readValue=0;
MrtButton button1(1);



void setup(){
    Serial.begin(115200);
    Serial1.begin(115200);
    Estado = 0;
    
}

void loop(){
    
    if(button1.isPressed(1)){
        if(((Estado)==(0))){
            Serial.write("Comando1\n");
            Serial1.write("AT+RST");
            Serial1.write("\r\n");        
        }
        if(((Estado)==(1))){
            Serial.write("Comando2\n");
            Serial1.write("AT+CWMODE=1");
             Serial1.write("\r\n");
        }
        if(((Estado)==(2))){
            Serial.write("Comando3\n");
            Serial1.write("AT+CWLAP");
             Serial1.write("\r\n");
        }
        if(((Estado)==(3))){
            Serial.write("Comando4\n");
            Serial1.write("AT+CWJAP=\"MOVISTAR_A550\",\"Fji4mh4jen9lo4F3lsll\"");
             Serial1.write("\r\n");
        }
        
          if(((Estado)==(4))){
            Serial.write("Comando5\n");
            Serial1.write("AT+CIPMUX=1");
             Serial1.write("\r\n");
        }    

        if(((Estado)==(5))){
            Serial.write("Comando6\n");
            Serial1.write("AT+CIPSERVER=1,80");
             Serial1.write("\r\n");
        }   
         if(((Estado)==(6))){
            Serial.write("Comando7\n");
            Serial1.write("AT+CIFSR");
             Serial1.write("\r\n");
        } 
                 
        Estado += 1;
        delay(500);
    }
    if(Serial1.available()){
        Serial.write(Serial1.read());
    }
    
    if(Serial.available()){
         readValue=Serial.read();
         Serial1.write(readValue);
    }
    
    
}


