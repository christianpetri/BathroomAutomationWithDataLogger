/* DHTServer - ESP8266 Webserver with a DHT sensor as an input

   Based on ESP8266Webserver, DHTexample, and BlinkWithoutDelay (thank you)

   Version 1.0  5/3/2014  Version 1.0   Mike Barela for Adafruit Industries
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#define DHTTYPE DHT22
#define DHTPIN  2

const char* ssid     = "yourNetwork"; // your network SSID (name)
const char* password = "PasswordForWifi"; // your network key
char serverName[] = "www.example.com"; //your websites url // www.sensor.christianpetri.ch
int secretKey  = 1234567890; //match with your html/php code      
// change to your server's port
int serverPort = 80;

// change to the page on that server
char pageName[] = "/add.php";
int totalCount = 1;
// insure params is big enough to hold your variables
char params[32];

// set this to the number of milliseconds delay
// this is 30 seconds 30000UL
#define delayMillis 5000UL

unsigned long thisMillis = 0;
unsigned long lastMillis = 0;

ESP8266WebServer server(80);
WiFiClient client;
// Initialize DHT sensor
// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
// you need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// This is for the ESP8266 processor on ESP-01
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

float humidity, temp_c;  // Values read from sensor
String webString="";     // String to display
int temperature;
int lastTemperatureRead = 0;
int lastHumiditiyRead = 0;

// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

void handle_root() {
   gettemperature();       // read sensor
     webString="<html><head><meta http-equiv=\"refresh\" content=\"5\"></head><body>Humidity: " + String((int)humidity)+"% ";
    webString+="Temperature: " + String((int)temp_c) + "&deg; C </body></html>";
     server.send(200, "text/html", webString);               // send to someones browser when asked
  delay(100);
}

void setup(void)
{
  // You can open the Arduino IDE Serial Monitor window to see what the code is doing
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable
  dht.begin();           // initialize temperature sensor

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to re-connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("DHT Weather Reading Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_root);

  server.on("/temp", [](){  // if you add this subdirectory to your webserver call, you get text below :)
    gettemperature();       // read sensor
    webString="<html><head><meta http-equiv=\"refresh\" content=\"5\"></head><body>Temperature:"+String((int)temp_c)+" C</body></html>";   // Arduino has a hard time with float to string
    server.send(200, "text/html", webString);            // send to someones browser when asked
  });

  server.on("/humidity", [](){  // if you add this subdirectory to your webserver call, you get text below :)
    gettemperature();           // read sensor
    webString="<html><head><meta http-equiv=\"refresh\" content=\"5\"></head><body>Humidity: "+String((int)humidity)+"% ";
    webString+="Temperature: "+String((int)temp_c)+"&deg; C </body></html>";
    server.send(200, "text/html", webString);               // send to someones browser when asked
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
  thisMillis = millis();
  if (WiFi.status() != WL_CONNECTED) {
      Serial.print("\n\r \n\rWorking to (re-) connect");
      WiFi.begin(ssid, password);
      int connectLoop = 0;
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        connectLoop++;
        if(connectLoop > 300)
        {
         Serial.print("Try again");
         break;
        }
      }
      if(WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
      }
  }
  if(thisMillis - lastMillis > delayMillis)
  {
    lastMillis = thisMillis;
    temperature=dht.readTemperature();
    humidity=dht.readHumidity();
    if(lastTemperatureRead == temperature &&  lastHumiditiyRead == (int)humidity){
        Serial.println("No Changes");
    }
    else {
       if(humidity <= 100 && temperature <= 80){
          // params must be url encoded.
          sprintf(params,"temp1=%i&hum1=%i&secret=%i", temperature, (int)humidity, secretKey);
          if(!postPage(serverName,serverPort,pageName,params)) Serial.print(F("Fail "));
          else Serial.print(F("Pass "));
          totalCount++;
          Serial.println(totalCount,DEC);
       }
    }
    lastTemperatureRead=temperature;
    lastHumiditiyRead=(int) humidity;
  }
}


void gettemperature() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor
    previousMillis = currentMillis;

    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();          // Read humidity (percent)
    temp_c = dht.readTemperature();     // Read temperature as Celsius
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_c)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}

byte postPage(char* domainBuffer,int thisPort,char* page,char* thisData)
{
  int inChar;
  char outBuf[64];

  Serial.print(F("connecting..."));

  if(client.connect(domainBuffer,thisPort) == 1)
  {
    Serial.println(F("connected"));

    // send the header
    sprintf(outBuf,"POST %s HTTP/1.1",page);
    client.println(outBuf);
    sprintf(outBuf,"Host: %s",domainBuffer);
    client.println(outBuf);
    client.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));
    sprintf(outBuf,"Content-Length: %u\r\n",strlen(thisData));
    client.println(outBuf);

    // send the body (variables)
    client.print(thisData);
    Serial.println(thisData);
  }
  else
  {
    Serial.println(F("failed"));
    return 0;
  }

  int connectLoop = 0;

  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      Serial.write(inChar);
      connectLoop = 0;
    }

    delay(1);
    connectLoop++;
    if(connectLoop > 10000)
    {
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
  }

  Serial.println();
  Serial.println(F("disconnecting."));
  client.stop();
  return 1;
}
