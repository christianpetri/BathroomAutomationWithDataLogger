/* Pins Used: A0 Buttons, A1-A5 LEDs, 2 Humidity/Temp Sensor, 5 6 7 Relay, 8 Motion, 9 Light Sensor, SD Card 4 11 12 13*/

  


#define SDcardActive // Enable SD CARD HERE // SDcardActive or else write  SDcardNOTActive     

#ifdef SDcardActive
    
      /*Start SD Card*/
    /*SD card datalogger 
    * SD card attached to SPI bus as follows: 
    ** MOSI - pin 11
    ** MISO - pin 12
    ** CLK - pin 13
    ** CS - pin 4
    created  24 Nov 2010 modified 9 Apr 2012 by Tom Igoe
    This example code is in the public domain.
    */

  #include <SPI.h>
  #include <SD.h>

  const char chipSelect = 4;  //The SD card uses Pin 4!

  const  unsigned int delayLogCrude = 300000; //Delay Log  Crude     300000ms    = 5     Minutes
  const unsigned int delayLogPrecise =30000; //Delay Log Presice   30000ms     = 30    Seconds
  unsigned long lastTimeButtonPressed = 0; //When was the button last pressed (milliseconds from last reset) 
  unsigned long lastLogTimeCrude = 0; //Set last log time ""
  unsigned long lastLogTimePrecise = 0; //Set last log time "" delayLogPresice
  boolean active = 0; // is Button delay still active, (Butten pressed, button remains "active" for 10 Seconds)
/*End  SD Card*/ 
#endif
 
/*Start Timer Fan*/
  const unsigned long timerDelay = 180000; // for how long the timer will run in MilliSeconds (3600000 ms = 1 Hour)
  unsigned long lastTimerTime = 0;   // will store last timer time
  boolean timerRunning = 0;  //check whether the  timer is running or not
/*End Timer*/
 /*START Humidity / Temperature*/
    // Example testing sketch for various DHT humidity/temperature sensors // Written by ladyada, public domain
    #include "DHT.h"
    #define DHTPIN 2     // what digital pin we're connected to = Pin 2

    // Uncomment whatever type you're using!
    //#define DHTTYPE DHT11   // DHT 11
    #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
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
  #define relayFanPin 5 //define on what pin the fan relay is
  boolean relayFanState = 1; //state of the fan relay. For the FAN relay LOW=On

  #define relayFan1Pin 6 //define on what pin the fan1 relay is
  boolean relayFan1State = 1; //state of the fan1 relay. For the FAN relay LOW=On

  #define relayLightPin 7 //define on what pin the light relay is
  boolean relayLightState = 0; //state of the Light relay
/*End Relay*/
/*Start Fan Auto */
  boolean fanAuto = 0; //Check if Fan Auto Mode is on or off (on= 1=true , off= 0=false)
  boolean fanAutoRememberState = 0;
  boolean  autoActive = 0;
/*End Fan Auto */
/*Start Motion Detection*/
  #define motionDetectionPin 8
  boolean motionDetectionState = 0;
  boolean motionActive = 0;
  unsigned long motionDetectionTimer = 60000; //Timer before Light is turned off 180000 ms = 3 Minutes // 60000 ms = 1 Minute
  unsigned long lastMotionDetectedTime = 0;
  unsigned long lastTimeLightTurnedOffManually=0;
  int deactivateMotionDetection=0;
  const unsigned int deactivateMotionDetectionTimer=5000; // 5000 ms = 5 seconds;
/*End Motion Detection*/
/*Start 3 Way Light Switch*/
  #define lightSensorPin 9
  int lightSensorState = 0;
  const int debounceLightDelay= 1000; //= 1 Second
  unsigned long lastLightDebounce = 0;
/*ENd 3 Way Light Switch*/
/*Start Buttons*/
  #define buttonsPin A0 //All the button run through ANALOG 0
  boolean buttonFanState  = 0; //state of the Fan Button
  boolean buttonFan1State = 0; //state of the Fan1 Button
  boolean buttonFan2State = 0; //state of the Fan1 Button
  boolean buttonLampState = 0; //state of the Light Button
/*End Buttons*/
/*Start LED*/
  #define LEDFanPin A1 //define on what pin the LED Fan is
  boolean LEDFanState = 0; //state of LED Fan

  #define LEDFan1Pin A2 //define on what pin the LED Fan1 is
  boolean LEDFan1State = 0; //state of LED Fan1

  #define LEDFan2Pin A3 //define on what pin the LED Fan1 is
  boolean LEDFan2State = 0; //state of LED Fan2

  #define LEDLightPin A4 //define on what pin the LED Light  is
  boolean LEDLightState = 0; //state of LED Light
/*End LED*/
/*Start debounce Button*/
  // Generally, you should use "unsigned long" for variables that hold time
  // The value will quickly become too large for an int to store
  unsigned long lastDebounceTime = 0;        // will store last time
  // constants won't change :
  const unsigned char debounceDelay = 200;           // interval of debounce (milliseconds)="delay"
  boolean debounceReturn=0;
  /*End debounce Button*/
 
void setup() {  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin(); //init the Humidity / Temperature sensor

  //relay
  pinMode(relayFanPin, OUTPUT);
  digitalWrite(relayFanPin, HIGH); //Turn Fan relay OFF

  pinMode(relayFan1Pin, OUTPUT);
  digitalWrite(relayFan1Pin, LOW); //Turn Fan1 relay On

  pinMode(relayLightPin, OUTPUT);

  //buttons
  pinMode(buttonsPin, INPUT);

  //photoresistor
  pinMode(lightSensorPin, INPUT);

  //LEDs
  pinMode(LEDFanPin, OUTPUT);
  pinMode(LEDFan1Pin, OUTPUT);
  pinMode(LEDFan2Pin, OUTPUT);
     digitalWrite(LEDFan2Pin,LEDFan2State);
  pinMode(LEDLightPin, OUTPUT); 
  //SD Card
  #ifdef SDcardActive 
    //SD Card
    // Serial.print("Initializing SD card...");
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
    }else{
      Serial.println(" card initialized."); 
    }  
  #endif
}
void loop() { // put your main code here, to run repeatedly:
   button(); //check if buttons are pressed
   motionDetectionGeneral(); //check if it is enabled and turn the light On/Off accordingly
   fan(); //fan On/Off
   fanAutoOnOff(); //Check if FAN AUTO  is enabled and turn the fan on off accordingly
   //readHumidityTemperature(); //Uncomment when you want to check if the temp and the humidity sensor is working
   #ifdef SDcardActive
    SDCardLogging();
  #endif

}

void button(){
  // read the state of the Push Buttons
   getButtonState();
   debounceReturn=debounce();

  //Fan On/Off **  relay FAN and FAN 1 = ON for delayTimer amount, then FAN= Off and Fan1=On
  if (buttonFanState == HIGH && debounceReturn) {
    lastDebounceTime = millis();
    //Serial.print("buttonFanPin pressed");
    if(LEDFan2State){ //if manual override is on: Turn off the LEDFan2, "turn" on (remember inverse below) LEDFan
      LEDFanState=0;
      digitalWrite(LEDFan2Pin,LEDFan2State=0);
    }
    digitalWrite(LEDFan2Pin,LEDFan2State=0); //turn off LEDFan 2
    digitalWrite(LEDFanPin, LEDFanState=!LEDFanState); //turn on/off LEDFan on=1 off=0
    digitalWrite(relayFanPin,relayFanState=!LEDFanState);  //turn on Fan on=0 off=1
    digitalWrite(relayFan1Pin,relayFan1State=0);    //make sure that old Fan controler has no control over Fan

     if(LEDFanState){ //if LEDFan is on start the timer to turn off the light
           timerRunning = 1;
           lastTimerTime=millis();
     }else{
        timerRunning=0; //if LEDFan is off "cancel" the timer, so the
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
     timerRunning=0; //cancel Light Off timer
     fanAuto=0; //turn off Fan Auto
     if(LEDFan1State || LEDFanState){ //if LEDFan1 or LEDFan is on make sure that LEDFan2 is on
      LEDFan2State=0; //inverse below =!0 =1
     }
     digitalWrite(LEDFanPin, LEDFanState=0);
     digitalWrite(LEDFan1Pin, LEDFan1State=0);
     digitalWrite(LEDFan2Pin, LEDFan2State=!LEDFan2State);

     if(LEDFan2State){ //if LED FAN2 is on, turn the relay FAN and FAN 1 High (High = relay OFF, LOW = relay ON)
       digitalWrite(relayFanPin, relayFanState=1);
       digitalWrite(relayFan1Pin, relayFan1State=1);
     }else{ //else LED FAN2 is off, turn the relay FAN High and FAN 1 LOW, "deactivae" old Fan control  (High = relay OFF, LOW = relay ON)
       digitalWrite(relayFanPin, relayFanState=1);
       digitalWrite(relayFan1Pin, relayFan1State=0);
     }

     if(fanAutoRememberState && !LEDFan2State){ //if (Fan Auto was on and LEDFan2 is off){Turn Fan Auto on again}
      digitalWrite(LEDFan1Pin, LEDFan1State=1);
      fanAuto=1;
     }
   }

   //Button for the light in the room = Motionen Detection On/Off: LED=on=Motionen Detection=off, LED=0ff=Motionen Detection=on
   if (buttonLampState == HIGH && debounceReturn) {
     lastDebounceTime = millis();
     //Serial.println("buttonLight pressed");
     if(deactivateMotionDetection){
         //if light  was manually turned off,  turn deativateMotionDetection off and turn MotionDetection off
         deactivateMotionDetection=0;
         LEDLightState=0; //turn LED on, inverse below
     }
     digitalWrite(LEDLightPin,LEDLightState=!LEDLightState);
  }
}
int  debounce(){
  if (millis() - lastDebounceTime >= debounceDelay ) {
    return 1;
  } else {
    return 0;
  }
}
int  getButtonState(){
   //Read the Analog value from the resistor ladder and determine what button was pushed
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
  #ifdef SDcardActive
    lastTimeButtonPressed=millis();
    active = 1;
  #endif 
  
}

//SDCard Start
#ifdef SDcardActive
  void SDCardLogging(){
   //make Log entry after button is pressed
    if(active && ((millis()-lastTimeButtonPressed) > 150)){ //=0.15 Seconds 
      makeLogEntriesPrecise();
      makeLogEntriesPrecise();  
      active=0;
    }  
   //make a Precise Log entry
    if(millis()-lastLogTimePrecise > delayLogPrecise){  
       lastLogTimePrecise=millis();
     makeLogEntriesPrecise();
    } 
    //make a Crude Log entry
    if((millis()-lastLogTimeCrude) > delayLogCrude) {
        lastLogTimeCrude=millis(); 
        makeLogEntriesCrude();
    }    
} 
void makeLogEntriesCrude(){ 
    // make a string for assembling the data to log:
    /*
    dataString = "";    
    
   int humidity = dht.readHumidity(); 
   if(humidity>0){ 
   dataString +=humidity;
    }
   int temperature=dht.readTemperature();
   if(temperature>0){ 
     dataString += ",";  
     dataString +=temperature;
   }
   */
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("humtemp.txt", FILE_WRITE); //datalog5MinutesCrude
    // if the file is available, write to it:
    if (dataFile) {
      //dataFile.println(dataString);
      if( (int) dht.readHumidity()>0){ 
        dataFile.print( (int) dht.readHumidity());
      }
      dataFile.print(",");
      if( (int) dht.readTemperature()>0){ 
        dataFile.print(( int) dht.readTemperature());
      }
      dataFile.println();
      dataFile.close();
      // print to the serial port too:
      //Serial.println(dataString);
    } else { // if the file isn't open, pop up an error:
      Serial.println("No crude.txt present");
    }   
}
void makeLogEntriesPrecise(){ 
   // make a string for assembling the data to log: Fan On/Off , Fan Auto , Fan Manual , Light On/Off, Fan State, Humidity , Temperature
    /*
    dataString = "";    
    dataString +=LEDFanState; //Fan On/Off
    dataString += ",";
    dataString +=LEDFan1State; //Fan Auto
    dataString += ",";
    dataString +=LEDFan2State; //Fan Manual
    dataString += ",";
    dataString +=digitalRead(lightSensorPin); //Light On/Off
    dataString += ",";  
    dataString +=!relayFanState; //When the relay is 0=LOW, Fan it is on. for the log 1 means on, so I have to get the inverse
  
    dataString += ",";   
    int humidity = dht.readHumidity();
    dataString +=humidity; 
  
    dataString += ",";  
    int temperature=dht.readTemperature();
    dataString +=temperature; 
      */    
   // open the file. note that only one file can be open at a time,
   // so you have to close this one before opening another.
   File dataFile = SD.open("precise.txt", FILE_WRITE);
   // if the file is available, write to it:
    if (dataFile) {
      //dataFile.println(dataString);
      dataFile.print(LEDFanState);
      dataFile.print(",");
      dataFile.print(LEDFan1State);
      dataFile.print(",");
      dataFile.print(LEDFan2State);
      dataFile.print(",");
      dataFile.print(digitalRead(lightSensorPin));
      dataFile.print(",");
      dataFile.print(!relayFanState);
      dataFile.print(",");
      dataFile.print( (int) dht.readHumidity());
      dataFile.print(",");
      dataFile.print(( int) dht.readTemperature());
      dataFile.println();
      dataFile.close();
      // print to the serial port too:
      //Serial.println(dataString);
    } else { // if the file isn't open, pop up an error:
      Serial.println("No precise.txt present");
    } 
}
#endif
//SDCard End

void motionDetectionGeneral(){
   if(!digitalRead(lightSensorPin) && motionActive && debounceLightSensor() && !LEDLightState) {
       //if the Light is turned off manually and motion is still active disable motion detection for 10 Seconds
       digitalWrite(LEDLightPin, LEDLightState=1);
       lastTimeLightTurnedOffManually=millis();
       deactivateMotionDetection=1;
       motionActive=0;
   }
   if(millis()-lastTimeLightTurnedOffManually > deactivateMotionDetectionTimer && deactivateMotionDetection){
       deactivateMotionDetection=0;
       digitalWrite(LEDLightPin, LEDLightState=0);
   }
   if(!LEDLightState){ //if the Motion detection Auto is turned off, allow motion detection
       motionDetection();
   }
}
void motionDetection(){
  if(digitalRead(motionDetectionPin)) { //if motion is detected, turn on light
    turnLightOn();
    lastMotionDetectedTime=millis();  //reset the timer
    //Serial.println("Moition detected");
    motionActive=1;
  }
  if(!digitalRead(relayFanPin)){ //if the fan is running, make the delay 15 Minutes, else 3 Minutes //relayFan LOW = ON
    motionDetectionTimer= 900000; // 900000 ms = 15 Minutes
  } else{
    motionDetectionTimer= 180000; // 180000 ms = 3 Minutes
  }

  if(millis()-lastMotionDetectedTime > motionDetectionTimer && motionActive){
    turnLightOff();
    motionActive=0;
  }
}
void turnLightOn(){
   if(!digitalRead(lightSensorPin)) {
    //if the Light is off, find out what the current Light-Realy state is (0 or 1)
    //and turn the light on
      if(debounceLightSensor()){
        digitalWrite(relayLightPin , relayLightState=!digitalRead(relayLightPin));
      }
    lastLightDebounce = millis();
   }
}
void turnLightOff(){
   if(digitalRead(lightSensorPin)) {
    //if the Light is on, find out what the current Light Realy state is (0 or 1)
    //and turn the light off
      if(debounceLightSensor()){
        digitalWrite(relayLightPin , relayLightState=!digitalRead(relayLightPin));
      }
    lastLightDebounce = millis();
   }
}
int  debounceLightSensor(){
  if(millis()-lastLightDebounce>debounceLightDelay){
    return 1;
  } else{
    return 0;
  }
}

void fan(){
  if(timerRunning){ //Fan is on for "timerDelay" time
      /*
      Serial.print(millis()-lastTimerTime);
      Serial.print("   TimerRunning. "); 
      Serial.println(timerDelay);
      */
      if(millis()-lastTimerTime >= timerDelay){ //when timer has finished. Turn off Fan
        digitalWrite(relayFanPin,relayFanState=1);
        digitalWrite(relayFan1Pin,relayFan1State=0);
        digitalWrite(LEDFanPin, LEDFanState=0);
        timerRunning = 0;
        autoActive=1;
      }
   }
}
void fanAutoOnOff(){
  if (fanAuto && !timerRunning){ //The fan is in automatic mode. If humidity rises above a certain value turn on fan, else turn off fan
       sensor();
   }
}
void sensor(){
    // Read humitidy
   int humidity = dht.readHumidity();
   if (humidity >  70) {
         digitalWrite(relayFanPin,relayFanState=0); //turn fan on
    }
    if (humidity <  65) {
       digitalWrite(relayFanPin,relayFanState=1); //turn fan off
    }
 }

void readHumidityTemperature(){
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  int f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  int hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  int hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F(" %\t"));
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F(" *C "));
  Serial.print(f);
  Serial.print(F(" *F\t"));
  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.print(F(" *C "));
  Serial.print(hif);
  Serial.println(F(" *F"));
}

