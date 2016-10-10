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

/*Start Relais*/
  int relaisFanPin = 5; //define on what pin the fan relais is
  int relaisFanState = HIGH; //state of the fan relais 
  
  int relaisFan1Pin = 6; //define on what pin the fan1 relais is
  int relaisFan1State = 1; //state of the fan1 relais 
   
  int relaisLampPin = 7; //define on what pin the light relais is
  int relaisLampState = 0; //state of the Light relais 

  int state=0; //add reverse boolean function: For Relais FAN LOW=On, for Relais Lamp HIGH=On
/*End Relais*/
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
  int LEDFan2State = 0; //state of LED Fan1

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
/*Start Timer*/
  const long timerDelay = 3600000; // for how long the timer will run in MilliSeconds (3600000 ms = 1 Hour)
  unsigned long lastTimerTime = 0;   // will store last timer time
  int timerRunning = 0;  //check whether the  timer is running
/*End Timer*/
/*Start Auto */
  int fanAuto=0; //The humidity senor is on Auto Mode
  int fanAutoRememberState=0;
/*End Auto */
/*Loop Start*/
 unsigned int  i=0;
/*Loop End*/
void setup() {  // put your setup code here, to run once:
  Serial.begin(9600); 
  dht.begin(); //init the Humidity / Temperature sensor

  //relais
  pinMode(relaisFanPin, OUTPUT);
  digitalWrite(relaisFanPin, HIGH); //Turn Fan Relais OFF
  
  pinMode(relaisFan1Pin, OUTPUT);
  digitalWrite(relaisFan1Pin, HIGH); //Turn Fan1 Relais OFF
  
  pinMode(relaisLampPin, OUTPUT);


  //buttons
    pinMode(buttonsPin, INPUT);
 
  //LEDs
  pinMode(LEDFanPin, OUTPUT);
  pinMode(LEDFan1Pin, OUTPUT);
  pinMode(LEDFan2Pin, OUTPUT);
     digitalWrite(LEDFan2Pin,HIGH);
  pinMode(LEDLampPin, OUTPUT);

  //Print humiditiy to the console
  //Serial.println(dht.readHumidity(),0);
} 

void loop() { // put your main code here, to run repeatedly:
   
    button(); //check if buttons are pressed
   
    if(timerRunning){ //Fan is on for "timerDelay" time
        if(timer()){ //when timer has finished. Turn off Fan  
          digitalWrite(relaisFanPin,1); 
          digitalWrite(relaisFan1Pin,0); 
          digitalWrite(LEDFanPin, LEDFanState=0); 
          timerRunning = 0;
       }  
    } 
     
   if (fanAuto && !timerRunning){ //The fan is in automatic mode. If humidity rises above a certain value turn on fan, else turn off fan
       sensor(); 
   }
    
   
      //Serial.println(dht.readHumidity(),0);
      i=0;  
    //delay(10000);
    
   //readHumidityTemperature(); //Uncomment when you want to check if the temp and the humidity sensor is working
}

void button(){
  // read the state of the pushbutton value (HIGH=pushed down,LOW=not pushed down):
  
   getButtonState();

   debounceReturn=debounce();

  //Fan On/Off /`**  Relais FAN and FAN 1 = ON for delayTimer amount, then FAN= Off and Fan1=On
  if (buttonFanState == HIGH && debounceReturn) {
    //Serial.print("buttonFanPin pressed");
    if(LEDFan2State){ //if manual override is on: Turn off the LEDFan2, "turn" on (remember inverse below) LEDFan
      LEDFanState=0;
      digitalWrite(LEDFan2Pin,LEDFan2State=0);
    }
    digitalWrite(LEDFan2Pin,LEDFan2State=0);
    digitalWrite(LEDFanPin, LEDFanState=!LEDFanState);
    digitalWrite(relaisFanPin,relaisFanState=!LEDFanState);  
    digitalWrite(relaisFan1Pin,relaisFan1State=0);   

     if(LEDFanState){
           timerRunning = 1;
           unsigned long currentMillis = millis();
           lastTimerTime=currentMillis; 
     }else{
        timerRunning=0;
     }
  }

   //Fan Auto On / Off ** if humididty is higher that x turn on fan 
   if (buttonFan1State == HIGH && debounceReturn) {
     //Serial.println("buttonFan1Pin pressed");
    
     if(LEDFan2State){ //if manual override is on: Turn off the LEDFan2, "turn" on (remember inverse below) LEDFan1
      LEDFan1State=0;
      digitalWrite(LEDFan2Pin,LEDFan2State=0);
     }
     
     digitalWrite(LEDFan1Pin, LEDFan1State=!LEDFan1State); 
     digitalWrite(relaisFan1Pin, 0);
     fanAuto=LEDFan1State; 
     fanAutoRememberState=LEDFan1State;
     
     if(!fanAuto && !timerRunning){
      digitalWrite(relaisFanPin, 1);
     }
   }

   //Fan manual override
   if (buttonFan2State == HIGH && debounceReturn) {
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
       digitalWrite(relaisFanPin, 1);
       digitalWrite(relaisFan1Pin, 1); 
     }else{
       digitalWrite(relaisFanPin, 1);
       digitalWrite(relaisFan1Pin, 0); 
     }
     
     if(fanAutoRememberState && !LEDFan2State){
      digitalWrite(LEDFan1Pin, 1); 
      fanAuto=1; 
     }
   }

   //Button for the light in the room
   if (buttonLampState == HIGH && debounceReturn) {
     //Serial.println("buttonLight pressed");
     digitalWrite(LEDLampPin, LEDLampState=!LEDLampState);
     digitalWrite(relaisLampPin,LEDLampState);  
  }  
 }
 

int debounce(){
   unsigned long currentMillis = millis();

  if (currentMillis - lastDebounceTime >= debounceDelay ) {
    // save the last time  
    lastDebounceTime = currentMillis;
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
         digitalWrite(relaisFanPin,0);
    } 
    if (humidity <  45) {
       digitalWrite(relaisFanPin,1);
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
  } else {
     buttonLampState=LOW;
  }
  
   if(value <= 521 && value >= 501){
     buttonFan2State=HIGH;
  } else {
     buttonFan2State=LOW;
  }
  
   if(value <= 350  && value >= 330){
     buttonFan1State=HIGH;
  } else {
     buttonFan1State=LOW;
  }
  
   if(value >= 240 && value <= 260){
     buttonFanState=HIGH;
  } else {
     buttonFanState=LOW;
  }  
}
