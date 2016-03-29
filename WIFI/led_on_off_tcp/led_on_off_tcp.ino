// Include files.
#include <SPI.h>
#include <WiFi.h>

// Your network SSID (network name).
// TODO: Enter the name of your wifi network here.
char ssid[] = "";

// Your network password.
// TODO: Enter the password of your wifi network here.
char pass[] = "";

// Your network key Index number (needed only for WEP).
int keyIndex = 0;

// Server status flag.
int status = WL_IDLE_STATUS;

// Create WiFi server listening on the given port.
WiFiServer server(3300);

void setup()
{
	Serial.begin(9600);

	if (WiFi.status() == WL_NO_SHIELD)
	{
		Serial.println("WiFi shield not present");
		status = WL_NO_SHIELD;
		return;
	}

	// Connect to Wifi network.
	while (status != WL_CONNECTED)
	{
		Serial.print("Connecting to Network named: ");
		Serial.println(ssid);

		// Connect to WPA/WPA2 network. Update this line if
		// using open or WEP network.
		status = WiFi.begin(ssid, pass);

		// Wait for connection.
		delay(1000);
	}

	// Start the server.
	server.begin();

	// Print WiFi status.
	printWifiStatus();
}

void loop()
{
	// Check that we are connected.
	if (status != WL_CONNECTED)
	{
		return;
	}

	// Listen for incoming client requests.
	WiFiClient client = server.available();
	if (!client)
	{
		return;
	}

	Serial.println("Client connected");

	String request = readRequest(&client);
	executeRequest(&client, &request);

}

// Read the request line. The string from the client ends with a newline.
String readRequest(WiFiClient* client)
{
	String request = "";

	// Loop while the client is connected.
	while (client->connected())
	{
		// Read available bytes.
		while (client->available())
		{
			// Read a byte.
			char c = client->read();

			// Print the value (for debugging).
			Serial.write(c);

			// Exit loop if end of line.
			if ('\n' == c)
			{
				return request;
			}

			// Add byte to request line.
			request += c;
		}
	}
	return request;
}

void executeRequest(WiFiClient* client, String* request)
{
	char command = readCommand(request);
	int n = readParam(request);
	if ('O' == command)
	{
		pinMode(n, OUTPUT);
	}
	else if ('I' == command)
	{
		pinMode(n, INPUT);
	}
	else if ('L' == command)
	{
		digitalWrite(n, LOW);
	}
	else if ('H' == command)
	{
		digitalWrite(n, HIGH);
	}
}

// Read the command from the request string.
char readCommand(String* request)
{
	String commandString = request->substring(0, 1);
	return commandString.charAt(0);
}

// Read the parameter from the request string.
int readParam(String* request)
{
	// This handles a hex digit 0 to F (0 to 15).
	char buffer[2];
	buffer[0] = request->charAt(1);
	buffer[1] = 0;
	return (int) strtol(buffer, NULL, 16);
}

void printWifiStatus()
{
	Serial.println("WiFi status");

	// Print network name.
	Serial.print("  SSID: ");
	Serial.println(WiFi.SSID());

	// Print WiFi shield IP address.
	IPAddress ip = WiFi.localIP();
	Serial.print("  IP Address: ");
	Serial.println(ip);

	// Print the signal strength.
	long rssi = WiFi.RSSI();
	Serial.print("  Signal strength (RSSI):");
	Serial.print(rssi);
	Serial.println(" dBm");
}
