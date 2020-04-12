/*
    HTTP over TLS (HTTPS) example sketch

    This example demonstrates how to use
    WiFiClientSecure class to connect to a TLS server.

    This example verifies server certificate using the
    root CA certificate.

    We fetch and display the status of
    esp8266/Arduino project continuous integration
    build.

    Created by Ivan Grokhotkov, 2017.
    This example is in public domain.
*/

#include <time.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>



const char* host = "www.sensor.christianpetri.ch";
int httpsPort = 443;

// Root certificate used by api.github.com.
// Defined in "CACert" tab.
extern const unsigned char caCert[] PROGMEM;
extern const unsigned int caCertLen;


extern const char*  ssid;
extern const char* password ;
extern const String secret;

WiFiClientSecure client;

//Start Sensor
#include <WEMOS_SHT3X.h>

SHT3X sht30(0x45);

int humidity = 0;
int temperature = 0;
int lastTemperatureReading = 0;
int lastHumiditiyReading = 0;
//End Sensor

//Start Time
// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0; // will store last temp was read
const long interval = 1000;       // interval at which to read sensor
int counter = 0;
//End Time

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Synchronize time useing SNTP. This is necessary to verify that
  // the TLS certificates offered by the server are currently valid.
  Serial.print("Setting time using SNTP");
  configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

  // Load root certificate in DER format into WiFiClientSecure object
  bool res = client.setCACert_P(caCert, caCertLen);
  if (!res) {
    Serial.println("Failed to load root CA certificate!");
    while (true) {
      yield();
    }
  }
}
void loop() {
  humidity = readHumidity();
  temperature = readTemperature();

  unsigned long now = millis();
  if (now - previousMillis > interval)
  {
    previousMillis = now;

    if (temperature != -1 && humidity != -1 ) {
      Serial.println(String("temperature: \t\t") +  temperature + String(" humidity: \t\t\t") + humidity) ;
      Serial.println(String("lastTemperatureReading \t") + lastTemperatureReading + String(" lastHumiditiyReading: \t" ) +  lastHumiditiyReading);
      if
      (
        (-1) < (humidity - lastHumiditiyReading) &&
        (humidity - lastHumiditiyReading) <= 1 &&
        lastTemperatureReading == temperature
      )
      {
        Serial.println("No Changes");
        counter++;
        if (counter >= 300) {
          counter = 0;
          sendContentToTheServer(temperature, humidity);
        }
      }
      else
      {
        if (0 <= humidity <= 100 && 0 <= temperature <= 65)
        {
          Serial.println("Changes");
          sendContentToTheServer(temperature, humidity);
          lastTemperatureReading = temperature;
          lastHumiditiyReading = humidity;
        }
      }
    }  else {
      Serial.println("Error: Please check the sensor");
    }
  }
}

void sendContentToTheServer(int temperature, int humidity ) {
  String content = "temperature=" + String(temperature) + "&humidity=" + String(humidity)+ "&secret=" + String(secret);
  sendPOSTrequest("/add.php", httpsPort, content);
}

void sendPOSTrequest(String url, int httpsPort, String content) {
  // Connect to remote server
  Serial.print("connecting to ");
  Serial.println(host);
  client.setTimeout(10000);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  Serial.println(F("Connected!"));
  // Verify validity of server's certificate
  if (client.verifyCertChain(host)) {
    Serial.println("Server certificate verified");
  } else {
    Serial.println("ERROR: certificate verification failed!");
    return;
  }

  Serial.print("requesting URL: ");
  Serial.println(url);
  Serial.println(content);
  client.println(String("POST ") + url + " HTTP/1.1");
  client.println(String("Host: ") + host );
  client.println("User-Agent: Arduino/1.0");
  client.println("Connection: close");;
  client.println("Content-Type: application/x-www-form-urlencoded");

  //all your other code, including the remaining HTTP-Request
  client.println(String("Content-Length: ") + content.length()); //insert, well, your content length
  client.println();
  client.println(content);
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  Serial.println(status);

  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }
  Serial.println("Sucess");
  /*
    while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    }
  */
  client.stop();
}

int readTemperature() {
  if (sht30.get() == 0) {
    return sht30.cTemp;
  }
  return -1;
}

int readHumidity() {
  if (sht30.get() == 0) {
    return sht30.humidity;
  }
  return -1;
}
