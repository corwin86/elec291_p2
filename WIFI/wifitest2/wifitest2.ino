#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                         SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "Connectify-1"   // cannot be longer than 32 characters!
#define WLAN_PASS       "bigballs"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define LISTEN_PORT           80      // What TCP port to listen on for connections.  
// The HTTP protocol uses port 80 by default.

#define MAX_ACTION            10      // Maximum length of the HTTP action that can be parsed.

#define MAX_PATH              64      // Maximum length of the HTTP request path that can be parsed.
// There isn't much memory available so keep this short!

#define BUFFER_SIZE           MAX_ACTION + MAX_PATH + 20  // Size of buffer for incoming request data.
// Since only the first line is parsed this
// needs to be as large as the maximum action
// and path plus a little for whitespace and
// HTTP version.

#define TIMEOUT_MS            500    // Amount of time in milliseconds to wait for
// an incoming request to finish.  Don't set this
// too high or your server could be slow to respond.

Adafruit_CC3000_Server httpServer(LISTEN_PORT);
uint8_t buffer[BUFFER_SIZE + 1];
int bufindex = 0;
char action[MAX_ACTION + 1];
char path[MAX_PATH + 1];

void setup(void)
{
  Serial.begin(115200);

  //can be safely removed later
  Serial.println(F("Hello, CC3000!\n"));
  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);

  // Initialise the module
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while (1);
  }

  //attempt to connect to wifi network
  Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while (1);
  }

  //can be safely removed later
  Serial.println(F("Connected!"));
  Serial.println(F("Request DHCP"));

  while (!cc3000.checkDHCP())
  {
    delay(100);
  }

//   Display the IP address DNS, Gateway, etc.
//    while (! displayConnectionDetails()) {
//      delay(1000);
//    }

  // Start listening for connections
  httpServer.begin();
  Serial.println(F("Listening for connections..."));
}

void loop(void)
{
  // Declare client
  Adafruit_CC3000_ClientRef client = httpServer.available();
  
  // If client is connected... start processing its request
  if (client) {
    //can be removed later
    Serial.println(F("Client connected."));
    // Clear the incoming data buffer and point to the beginning of it
    bufindex = 0;
    memset(&buffer, 0, sizeof(buffer));
    memset(&action, 0, sizeof(action));
    memset(&path,   0, sizeof(path));

    // Set a timeout for reading all the incoming data.
    unsigned long endtime = millis() + TIMEOUT_MS;

    // Read all the incoming data until it can be parsed or the timeout expires.
    bool parsed = false;
    parsed = parsingRequest(client, parsed, endtime); //function to parse request

    // Handle the request if it was parsed.
    if (parsed) {
      //can be removed later
      Serial.println(F("Processing request"));
      Serial.print(F("Action: ")); Serial.println(action);
      Serial.print(F("Path: ")); Serial.println(path);

      processRequest(client, action); //function to process request

      // Wait a short period to make sure the response had time to send before
      // the connection is closed (the CC3000 sends data asyncronously).
      delay(100);

      // Close the connection when done.
      Serial.println(F("Client disconnected"));
      client.close();
    }
  }
}

//Parses request by calling sub function parseRequest, puts parsed request data into respective fields
bool parsingRequest(Adafruit_CC3000_ClientRef client, bool parsed, unsigned long endtime)
{
  while (!parsed && (millis() < endtime) && (bufindex < BUFFER_SIZE)) {
    if (client.available()) {
      buffer[bufindex++] = client.read();
    }
    parsed = parseRequest(buffer, bufindex, action, path);
  }
  return parsed;
}

//Once the request is parsed, determine which type of request and perfrom action accordingly
void processRequest(Adafruit_CC3000_ClientRef client, char* action)
{
  //RESPONSE FOR GET REQUEST (NOT NEEDED)
  if (strcmp(action, "GET") == 0) {
    respondGet(client);
  }

  // RESPONSE FOR POST REQUEST
  else if (strcmp(action, "POST") == 0) {
    respondPost(client);
  }
  else {
    // Unsupported action, respond with an HTTP 405 method not allowed error.
    client.fastrprintln(F("HTTP/1.1 200 OK"));
    client.fastrprintln(F(""));
    client.fastrprintln(F("-1"));
  }
}

void respondGet(Adafruit_CC3000_ClientRef client){
  // First send the success response code.
    client.fastrprintln(F("HTTP/1.1 200 OK"));
    //        // Then send a few headers to identify the type of data returned and that
    //        // the connection will not be held open.
    client.fastrprintln(F("Content-Type: text/plain"));
    client.fastrprintln(F("Connection: close"));
    client.fastrprintln(F("Server: Adafruit CC3000"));
    //        // Send an empty line to signal start of body.
    client.fastrprintln(F(""));
    // Now send the response data.
    client.fastrprintln(F("Connection successful"));
    client.fastrprint(F("You accessed path: ")); client.fastrprintln(path);
    client.fastrprint(F("what is this shit"));
}

int count = 0;
void respondPost(Adafruit_CC3000_ClientRef client){
      //Read data from post request body and store info into fields
    String data = "";
    bool StartBody = false;
    while (client.available()) {
      //Serial.write(client.read());
      char currentChar = client.read();

      if (currentChar == '\n')
        StartBody = true;

      if (StartBody == true) {
        data += (String) currentChar;
      }

    } Serial.println(data); //can be removed later

    //Send server response back to client
    //This would be used to send stuff like game state, win alert, etc back to client
    client.fastrprintln(F("HTTP/1.1 200 OK"));
    client.fastrprintln(F("Content-Type: text/plain"));
    client.fastrprintln(F("Connection: close"));
    client.fastrprintln(F("Server: Adafruit CC3000"));

    // Send an empty line to signal start of body.
    client.fastrprintln(F(""));

    // Can be removed later
    client.fastrprintln(count % 2 == 0 ? "1" : "2");
    Serial.print("PLAYER: "); Serial.println(count % 2 == 0 ? "1" : "2");
    count++;
//    client.fastrprintln(F("Connection successful"));
//    client.fastrprint(F("You accessed path: ")); client.fastrprintln(path);

    // TODO: Return player turn
}


// Return true if the buffer contains an HTTP request.  Also returns the request
// path and action strings if the request was parsed.  This does not attempt to
// parse any HTTP headers because there really isn't enough memory to process
// them all.
// HTTP request looks like:
//  [method] [path] [version] \r\n
//  Header_key_1: Header_value_1 \r\n
//  ...
//  Header_key_n: Header_value_n \r\n
//  \r\n
bool parseRequest(uint8_t* buf, int bufSize, char* action, char* path) {
  // Check if the request ends with \r\n to signal end of first line.
  if (bufSize < 2)
    return false;
  if (buf[bufSize - 2] == '\r' && buf[bufSize - 1] == '\n') {
    parseFirstLine((char*)buf, action, path);
    return true;
  }
  return false;
}

// Parse the action and path from the first line of an HTTP request.
void parseFirstLine(char* line, char* action, char* path) {
  // Parse first word up to whitespace as action.
  char* lineaction = strtok(line, " ");
  if (lineaction != NULL)
    strncpy(action, lineaction, MAX_ACTION);
  // Parse second word up to whitespace as path.
  char* linepath = strtok(NULL, " ");
  if (linepath != NULL)
    strncpy(path, linepath, MAX_PATH);
}

// Tries to read the IP address and other connection details
bool displayConnectionDetails(void) {
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if (!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}
