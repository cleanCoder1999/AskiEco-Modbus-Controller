/*
 Name:		AskiEco_Modbus_Controller.ino
 Created:	9/13/2019 9:06:05 AM
 Author:	Lucas
*/

#include "modbus.h"


modbus_t* masterDescriptor;

// the setup function runs once when you press reset or power the board
void setup() {
	
	masterDescriptor = modbus_new_tcp("192.168.100.49", 50000);

	if()

	

}

// the loop function runs over and over again until power down or reset
void loop() {

	


}
