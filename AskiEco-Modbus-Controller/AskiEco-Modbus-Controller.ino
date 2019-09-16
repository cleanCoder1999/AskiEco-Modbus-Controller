/*
 Name:		AskiEco_Modbus_Controller.ino
 Created:	9/13/2019 9:06:05 AM
 Author:	Lucas
*/

#include <ArduinoModbus.h>
#include <Ethernet.h>

/*
 * GLOBAL VARIABLES
 * */
EthernetClient client;
ModbusTCPClient* arduino;

const byte arduinoMacAddress[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x94, 0xB5 };

IPAddress arduinoIp;
const IPAddress askiEcoIp(192, 168, 100, 49);

const uint16_t port = 502;

// the setup function runs once when you press reset or power the board
void setup() {
	
	initialiseSerialMonitor();
	Serial.println("\n[INFO]: in setup: initialised serial monitor");

	initialiseNetwork();
	Serial.println("[INFO]: in setup: initialised network lib");

	arduinoIp = getArduinoIp();
	
	// print configured IP
	Serial.print("\nlocal IP: ");
	Serial.println(arduinoIp);

	establishTcpConnection();
	if (client.connected())
		Serial.println("[INFO]: in setup: client did connect via TCP");
	else
		Serial.println("[ERROR]: in setup: client did NOT connect via TCP");

	establishModbusConnection();
	if (arduino->connected())
		Serial.println("[INFO]: in setup: Modbus connection to AskiEco-Controller via TCP established");
	else
		Serial.println("[ERROR]: in setup: could not connect to AskiEco-Controller via TCP");

	/*
	ModbusTCPClient arduino(client);

	// set up connection
	arduino.begin(askiEcoIp, port);

	if (arduino.connected())
		Serial.print("[INFO]: in setup: connection to AskiEco-Controller established");
	else
		Serial.print("[ERROR]: in setup: couldn't connect to AskiEco-Controller");

	*/
}

// the loop function runs over and over again until power down or reset
void loop() {


	/*
	* to be continued...
	* */
	long valueOfHoldingReg = arduino->holdingRegisterRead(400);
	Serial.print("[INFO]: in loop: valueOfHoldingReg=");
	Serial.println(valueOfHoldingReg);

}


void initialiseSerialMonitor()
{
	Serial.begin(9600);
	delay(1000);
}
void initialiseNetwork()
{
	Ethernet.begin(arduinoMacAddress);// , arduinoIp);
}

IPAddress getArduinoIp()
{
	return Ethernet.localIP();
}

void establishTcpConnection()
{
	// connect client to server
	client.connect(askiEcoIp, port);
}
void establishModbusConnection()
{
	// allocate dynamic object
	arduino = new ModbusTCPClient(client);
	
	// set up Modbus connection
	arduino->begin(askiEcoIp, port);
}



void closeConnectionToAskiEco()
{
	closeModbusConnection();
	closeTcpConnection();
}
void closeModbusConnection()
{
	if(arduino->connected())
		arduino->end();
}
void closeTcpConnection()
{
	if(client.connected())
		client.stop();
}
/*
void printInfo(const char* info, unsigned int lengthOfInfo)
{
	Serial.println("[INFO]: " + *info);
}*/

