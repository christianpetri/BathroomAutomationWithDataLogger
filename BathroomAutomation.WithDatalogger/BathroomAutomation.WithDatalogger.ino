/*Start SD Card*/
/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4; 

unsigned long delayLog =300000; //Delay log = 5 Minutes
unsigned long lastTimeButtonPressed = 0; //When was the button last pressed (milliseconds from last reset) 
unsigned long lastLogTime = 0; //Set last log time ""
int active = 0; // is Button delay still active, (Butten pressed, button remains "active" for 10 Seconds)
/*End  SD Card*/
/*Start Timer*/
  const long timerDelay = 3600000; // for how long the timer will run in MilliSeconds (3600000 ms = 1 Hour)
  unsigned long lastTimerTime = 0;   // will store last timer time
  int timerRunning = 0;  //check whether the  timer is running
/*End Timer*/
 /*START Humidity / Temperature*/
          // Example testing sketch for various DHT humidity/temperature sensors
          // Written by ladyada, public domain
          
          #include "DHT.h"
  
  #define DHTPIN 2     // what digital pin we're connected to
  
    // Uncomment whatever type you're using!
    #define DHTTYPE DHT11   // DHT 11
    //#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
    //#define DHTTYPE DHT21   // DHT 21 (AM2301)
    
    // Connect pin 1 (on the left) of the sensor to +5V
    // NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
    // to 3.3V instead of 5V!
    // Connect pin 2 of the sensor to whatever your DHTPIN is
    // Connect pin 4 (on the right) of the sensor to GROUND
    // Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
    
    // Initialize DHT sensor.
    // Note that older versions of this library took an optional third parameter to
    // tweak the timings for faster processors.  This parameter is no longer needed
    // as the current DHT reading algorithm adjusts itself to work on faster procs.
  DHT dht(DHTPIN, DHTTYPE);
/*END Humidity / Temperature*/

/*Start Relay*/
  int relayFanPin = 5; //define on what pin the fan relay is
  int relayFanState = HIGH; //state of the fan relay 
  
  int relayFan1Pin = 6; //define on what pin the fan1 relay is
  int relayFan1State = 1; //state of the fan1 relay 
   
  int relayLampPin = 7; //define on what pin the light relay is
  int relayLampState = 0; //state of the Light relay 

  int state=0; //add reverse boolean function: For relay FAN LOW=On, for relay Lamp HIGH=On
/*End Relay*/
/*Start Buttos*/
  int buttonsPin = A0; //All the button run through ANALOG 0 
  int buttonFanState  = 0; //state of the Fan Button 
  int buttonFan1State = 0; //state of the Fan1 Button 
  int buttonFan2State = 0; //state of the Fan1 Button 
  int buttonLampState = 0; //state of the Light Button
/*End Buttons*/
/*Start LED*/
  int LEDFanPin = A1; //define on what pin the LED Fan is
  int LEDFanState = 0; //state of LED Fan
  
  int LEDFan1Pin = A2; //define on what pin the LED Fan1 is
  int LEDFan1State = 0; //state of LED Fan1

  int LEDFan2Pin = A3; //define on what pin the LED Fan1 is
  int LEDFan2State = 1; //state of LED Fan2

  int LEDLampPin = A4; //define on what pin the LED Light  is
  int LEDLampState = 0; //state of LED Light 
/*End LED*/
/*Start debounce Button*/
  // Generally, you should use "unsigned long" for variables that hold time
  // The value will quickly become too large for an int to store
  unsigned long lastDebounceTime = 0;        // will store last time 
  // constants won't change :
  const long debounceDelay = 150;           // interval of debounce (milliseconds)="delay"
  int debounceReturn=0;
/*End debounce Button*/ 
/*Start Auto */
  int fanAuto=0; //The humidity senor is on Auto Mode
  int fanAutoRememberState=0;
/*End Auto */
 
void setup() {  // put your setup code here, to run once:
  Serial.begin(9600); 
  dht.begin(); //init the Humidity / Temperature sensor

  //relay
  pinMode(relayFanPin, OUTPUT);
  digitalWrite(relayFanPin, HIGH); //Turn Fan relay OFF
  
  pinMode(relayFan1Pin, OUTPUT);
  digitalWrite(relayFan1Pin, HIGH); //Turn Fan1 relay OFF
  
  pinMode(relayLampPin, OUTPUT);


  //buttons
    pinMode(buttonsPin, INPUT);
 
  //LEDs
  pinMode(LEDFanPin, OUTPUT);
  pinMode(LEDFan1Pin, OUTPUT);
  pinMode(LEDFan2Pin, OUTPUT);
     digitalWrite(LEDFan2Pin,LEDFan2State);
  pinMode(LEDLampPin, OUTPUT);

  //SD Card
   // Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
  }else{
    //Serial.println(" card initialized."); 
  }  
} 

void loop() { // put your main code here, to run repeatedly:
    unsigned long currentMillis = millis();
    button(); //check if buttons are pressed
    SDCard();
    if(timerRunning){ //Fan is on for "timerDelay" time
        if(timer()){ //when timer has finished. Turn off Fan  
          digitalWrite(relayFanPin,relayFanState=1); 
          digitalWrite(relayFan1Pin,relayFan1State=0); 
          digitalWrite(LEDFanPin, LEDFanState=0); 
          timerRunning = 0;
       }  
    } 
     
   if (fanAuto && !timerRunning){ //The fan is in automatic mode. If humidity rises above a certain value turn on fan, else turn off fan
       sensor(); 
   }
  
   //readHumidityTemperature(); //Uncomment when you want to check if the temp and the humidity sensor is working
}

void button(){
  // read the state of the pushbutton value (HIGH=pushed down,LOW=not pushed down):
  
   getButtonState();

   debounceReturn=debounce();

  //Fan On/Off /`**  relay FAN and FAN 1 = ON for delayTimer amount, then FAN= Off and Fan1=On
  if (buttonFanState == HIGH && debounceReturn) {
    lastDebounceTime = millis();
    //Serial.print("buttonFanPin pressed");
    if(LEDFan2State){ //if manual override is on: Turn off the LEDFan2, "turn" on (remember inverse below) LEDFan
      LEDFanState=0;
      digitalWrite(LEDFan2Pin,LEDFan2State=0);
    }
    digitalWrite(LEDFan2Pin,LEDFan2State=0);
    digitalWrite(LEDFanPin, LEDFanState=!LEDFanState);
    digitalWrite(relayFanPin,relayFanState=!LEDFanState);  
    digitalWrite(relayFan1Pin,relayFan1State=0);    //make sure that old Fan controler has no control over Fan

     if(LEDFanState){
           timerRunning = 1; 
           lastTimerTime=millis(); 
     }else{
        timerRunning=0;
     }
  }

   //Fan Auto On / Off ** if humididty is higher that x turn on fan 
   if (buttonFan1State == HIGH && debounceReturn) {
     lastDebounceTime = millis();
     //Serial.println("buttonFan1Pin pressed");
    
     if(LEDFan2State){ //if manual override is on: Turn off the LEDFan2, "turn" on (remember inverse below) LEDFan1
      LEDFan1State=0;
      digitalWrite(LEDFan2Pin,LEDFan2State=0);
     }
     
     digitalWrite(LEDFan1Pin, LEDFan1State=!LEDFan1State); 
     digitalWrite(relayFan1Pin, relayFan1State=0); //make sure that old Fan controler has no control over Fan
     fanAuto=LEDFan1State; 
     fanAutoRememberState=LEDFan1State;
     
     if(!fanAuto && !timerRunning){
      digitalWrite(relayFanPin,relayFanState=1);
     }
   }

   //Fan manual override
   if (buttonFan2State == HIGH && debounceReturn) {
     lastDebounceTime = millis();
     //Serial.println("buttonFan2Pin pressed");
     timerRunning=0;
     fanAuto=0;
     if(LEDFan1State || LEDFanState){
      LEDFan2State=0;
     }
     digitalWrite(LEDFanPin, LEDFanState=0);
     digitalWrite(LEDFan1Pin, LEDFan1State=0);
     digitalWrite(LEDFan2Pin, LEDFan2State=!LEDFan2State);
     
     if(LEDFan2State){
       digitalWrite(relayFanPin, relayFanState=1);
       digitalWrite(relayFan1Pin, relayFan1State=1); 
     }else{
       digitalWrite(relayFanPin, relayFanState=1);
       digitalWrite(relayFan1Pin, relayFan1State=0); 
     }
     
     if(fanAutoRememberState && !LEDFan2State){
      digitalWrite(LEDFan1Pin, LEDFan1State=1); 
      fanAuto=1; 
     }
   }

   //Button for the light in the room
   if (buttonLampState == HIGH && debounceReturn) {
      lastDebounceTime = millis();
     //Serial.println("buttonLight pressed");
     digitalWrite(LEDLampPin, LEDLampState=!LEDLampState);
     digitalWrite(relayLampPin,relayLampState=LEDLampState);  
  }  
 }
 

int debounce(){
    

  if (millis() - lastDebounceTime >= debounceDelay ) { 
    return 1;
  } else {
    return 0;  
  }  
}
void readHumidityTemperature(){
   
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F"); 
 }

 void sensor(){ 
    // Read humitidy 
   float humidity = dht.readHumidity(); 
   if (humidity >  50) {
         digitalWrite(relayFanPin,relayFanState=0);
    } 
    if (humidity <  45) {
       digitalWrite(relayFanPin,relayFanState=1);
    } 
 }
 
int timer(){
  unsigned long currentMillis = millis();

  if(currentMillis-lastTimerTime >= timerDelay){ 
    return 1;
  } else {
    return 0;  
  }   
}


int getButtonState(){
 int value= analogRead(A0);
  //Serial.println (value);
  if(value<=1024  &&  value >=1013){
     buttonLampState=HIGH;
     buttonPressed();
  } else {
     buttonLampState=LOW;
  }
  
   if(value <= 521 && value >= 501){
     buttonFan2State=HIGH;
     buttonPressed();
  } else {
     buttonFan2State=LOW; 
  }
  
   if(value <= 350  && value >= 330){
     buttonFan1State=HIGH;
     buttonPressed();
  } else {
     buttonFan1State=LOW;
  }
  
   if(value >= 240 && value <= 260){
     buttonFanState=HIGH;
     buttonPressed();
  } else {
     buttonFanState=LOW;
  }  
}

void buttonPressed(){ 
  lastTimeButtonPressed=millis();
  active=1;    
}

void SDCard(){
    if(active && ((millis()-lastTimeButtonPressed) > 150)){ //=0.15 Seconds 
      makeLogEntries();
      makeLogEntries();  
      active=0;
    } 
    
    if((millis()-lastLogTime) > delayLog) {
        lastLogTime=millis(); 
        makeLogEntries();
    } 
  
}



void makeLogEntries(){
          // make a string for assembling the data to log:
        String dataString = ""; 
           
        dataString +=LEDFanState;
        dataString += ",";
        dataString +=LEDFan1State;
        dataString += ",";
        dataString +=LEDFan2State;
        dataString += ",";
        dataString +=LEDLampState;
        dataString += ",";  
        dataString +=!relayFanState;
               
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        File dataFile = SD.open("datalog.txt", FILE_WRITE);
      
        // if the file is available, write to it:
        if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();
          // print to the serial port too:
          Serial.println(dataString);
        }
        
        // if the file isn't open, pop up an error:
        else {
          Serial.println("No SD Card present");
        }   
}
