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
//ModbusTCPClient* arduino;
ModbusTCPClient arduino(client);

// INITIALISING
bool initReturnValues[4]{false, false, false, false };

// MODBUS
const int SLAVE_ADDRESS{ 1 };
long valueOfHoldingReg{ -1 };
int requestedValues{ -1 };

// NETWORK
byte arduinoMacAddress[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x94, 0xB5 };
IPAddress arduinoIp(172, 16, 17, 40);
IPAddress subnet(255, 255, 0, 0);
IPAddress gateWay(172, 16, 17, 1);
IPAddress dnsServer(192, 168, 100, 1);

IPAddress askiEcoIp(192, 168, 100, 49);

uint16_t port{ 502 };


// the setup function runs once when you press reset or power the board
void setup() {

	/*
	 * INIT SERIAL INTERFACE (MONITOR)
	 * */
	initialiseSerialMonitor();
	if(Serial)
		initReturnValues[0] = true;

	//Serial.println("[INFO]: in setup: initalising...");

	/*
	 * INIT ETHERNET SHIELD
	 * */
	initReturnValues[1] = (bool) initialiseNetworkDHCP();
	delay(2000);
	if (!initReturnValues[1])
	{
		initialiseNetworkStatic();
		delay(2000);

		arduinoIp = {0, 0, 0, 0};
		arduinoIp = getArduinoIp();
		initReturnValues[1] = ipObtained();
	}

	// print configured IP
	Serial.print("\nlocal IP: ");
	Serial.println(arduinoIp);

	/*
	 * CONNECT TO ETHERNET SERVER
	 * */
	establishTcpConnection();
	delay(2000);
	if (client.connected() == 1)
		initReturnValues[2] = true;
	else
		initReturnValues[2] = false;
	
	/*
	 * BEGIN CONNECTION TO MODBUS SLAVE
	 * */
	establishModbusConnection();
	delay(2000);
	if(client.connected())
		initReturnValues[3] = true;
	else
		initReturnValues[3] = false;

	/*
	 * CHECK IF INIT SUCCEEDED
	 * */
	bool everythingWorked{ true };
	for (unsigned int i = 0; i < sizeof(initReturnValues) / sizeof(bool); ++i)
	{
		if (!initReturnValues[i])
			everythingWorked = false;
	}

	if (everythingWorked)
		Serial.println("[INFO]: init succeeded");
	else
		Serial.println("[INFO]: init did NOT succeed");
}

// the loop function runs over and over again until power down or reset
void loop() {

	/*
	 * to be continued...
	 * */

	//valueOfHoldingReg = arduino->holdingRegisterRead(1, 432);
	//requestedValues = arduino->requestFrom(1, HOLDING_REGISTERS, 22, 2);
	//requestedValues = arduino.requestFrom(1, HOLDING_REGISTERS, 22, 2);
	
	//delay(1000ul);

	//Serial.print("[INFO]: in loop: requestedValues=");
	//Serial.println(valueOfHoldingReg);
	//Serial.println(requestedValues);
}

void initialiseSerialMonitor()
{
	Serial.begin(9600);
	delay(1000);
}

bool initialiseNetworkDHCP()
{
	return (bool) Ethernet.begin(arduinoMacAddress);
}

void initialiseNetworkStatic()
{
	Ethernet.begin(arduinoMacAddress, arduinoIp, dnsServer, gateWay, subnet);
}

IPAddress getArduinoIp()
{
	return Ethernet.localIP();
}

bool ipObtained()
{
	IPAddress compareIp(0,0,0,0);
	return arduinoIp != compareIp;
}

void establishTcpConnection()
{
	// connect client to server
	client.connect(askiEcoIp, port);
	//client.connect({172,16,17,39}, port);
}

void establishModbusConnection()
{
	// allocate dynamic object
	//arduino = new ModbusTCPClient(client);
	
	// set up Modbus connection
	arduino.begin(askiEcoIp, port);
}


void closeConnectionToAskiEco()
{
	closeModbusConnection();
	closeTcpConnection();
}
void closeModbusConnection()
{
	if(arduino.connected())
		arduino.end();
}
void closeTcpConnection()
{
	if(client.connected())
		client.stop();
}

