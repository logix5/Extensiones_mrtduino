#include "Arduino.h"

#include "Bluetooth.h"




Bluetooth::Bluetooth()
{
	
}

void Bluetooth::BTinit(void)
{
	clearValues();
}

void Bluetooth::BTReadMobileApplicationCommand(void)
{
	for (int i = 0; i <= commandSize; i++)
		{
		command[i] = (byte)Serial1.read(); // nos quedamos con el comando
		}
		
	  for (int i=0;i<6;i++)// Limpia buffer de f
      	{
			f[i] = false;
		}
      
      calculateValues();

      bluetooth_X = value[0];
      bluetooth_Y = value[1];
      bluetooth_Z = value[2];
      bluetooth_W = value[3];
      bluetooth_aX = value[4];
      bluetooth_aY = value[5];
      bluetooth_aZ = value[6];

      bluetooth_F1 = f[0];
      bluetooth_F2 = f[1];
      bluetooth_F3 = f[2];
      bluetooth_F4 = f[3];
      bluetooth_F5 = f[4];
      bluetooth_F6 = f[5];
		
}
  
bool Bluetooth::BT_F1_received(void)
{
	return bluetooth_F1;
}

bool Bluetooth::BT_F2_received(void)
{
	return bluetooth_F2;
}

bool Bluetooth::BT_F3_received(void)
{
	return bluetooth_F3;
}

bool Bluetooth::BT_F4_received(void)
{
	return bluetooth_F4;
}

bool Bluetooth::BT_F5_received(void)
{
	return bluetooth_F5;
}

  
bool Bluetooth::BT_F6_received(void)
{
	return bluetooth_F6;
}

int Bluetooth::BT_Value_X(void)
{
 return bluetooth_X;
}

int Bluetooth::BT_Value_Y(void)
{
 return bluetooth_Y;
}

int Bluetooth::BT_Value_Z(void)
{
 return bluetooth_Z;
}

int Bluetooth::BT_Value_W(void)
{
 return bluetooth_W;
}

int Bluetooth::BT_Value_aX(void)
{
 return bluetooth_aX;
}

int Bluetooth::BT_Value_aY(void)
{
 return bluetooth_aY;
}

int Bluetooth::BT_Value_aZ(void)
{
 return bluetooth_aZ;
}

void Bluetooth::clearValues(void) 
{
	for (int i=0;i<6;i++) //Borramos todos los datos
		f[i] = false;
	for (int i=0;i<7;i++)
		value[i]=0;
	for (int i=0;i<9;i++)
		command[i] = 0x00;
}

void Bluetooth::calculateValues(void)
   {  
     int fData = (int)command[7];

      // Recogemos los valores
      if (fData - 64 >= 0) { f[5] = true; fData -= 64; } // Ver estado F6
      if (fData - 32 >= 0) { f[4] = true; fData -= 32; } // Ver estado F5
      if (fData - 16 >= 0) { f[3] = true; fData -= 16; } // Ver estado F4
      if (fData - 8 >= 0) { f[2] = true; fData -= 8; } // Ver estado F3
      if (fData - 4 >= 0) { f[1] = true; fData -= 4; } // Ver estado F2
      if (fData - 2 >= 0) { f[0] = true; fData -= 2; } // Ver estado F1
      
      int buffer;
      for (int i=0;i<7;i++) {
      	buffer = (int)command[i];
        if (buffer > 128) // 
        	buffer -= 256;
        value[i] = buffer;
    }
   }
