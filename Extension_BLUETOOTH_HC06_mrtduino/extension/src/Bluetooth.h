
#include "Arduino.h"

#ifndef Bluetooth_cpp
#define Bluetooth_cpp

class Bluetooth
{
	
	public:
	int commandSize = 9;
    byte command[9];
	boolean f[6];
    int value[7];

	int bluetooth_X, bluetooth_Y, bluetooth_Z, bluetooth_W, bluetooth_aX, bluetooth_aY,bluetooth_aZ;
	bool bluetooth_F1, bluetooth_F2, bluetooth_F3, bluetooth_F4, bluetooth_F5, bluetooth_F6;

	void clearValues(void);
	void calculateValues(void);
	
	Bluetooth();
    void BTinit(void);
	void BTReadMobileApplicationCommand(void);
	bool BT_F1_received(void);
	bool BT_F2_received(void);
	bool BT_F3_received(void);
	bool BT_F4_received(void);
	bool BT_F5_received(void);
	bool BT_F6_received(void);
	int BT_Value_X(void);
	int BT_Value_Y(void);
	int BT_Value_Z(void);
	int BT_Value_W(void);
	int BT_Value_aX(void);
	int BT_Value_aY(void);
	int BT_Value_aZ(void);
};

    

#endif