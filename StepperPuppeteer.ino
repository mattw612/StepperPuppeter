#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp;
Adafruit_ADS1015 ads;     /* Use this for the 12-bit version, 73 if the jumper is shorted) */

int number_of_steps = 0;
int pulseLength = 0;
long pulseStart = 0;
int pulseEnd = 0;
bool armed = false;
int testLength = 500;
# define disablePin 9
# define signalInputPin 8
# define pulseOutputPin 0
# define delay_ms 3  // pulse delay in ms

void setup() {  
  mcp.begin_I2C(0x21);      // use default address 0x21

  Serial.begin(9600);
  
  ads.setGain(GAIN_ONE);
  ads.begin();
  mcp.pinMode(pulseOutputPin, OUTPUT);
  mcp.pinMode(signalInputPin, INPUT);
  mcp.pinMode(disablePin, INPUT);

}



void loop() {
  // int16_t adc0, adc1, adc2, adc3;
  pulseLength = 0;
  // pulseStart = 0;
  // pulseEnd = 0;
  // Serial.println(mcp.digitalRead(8));
  // mcp.digitalWrite(0, LOW);
  
  if (mcp.digitalRead(8) == HIGH) {
    armed = true;
        
  } else if (armed == true) {

    Serial.print("checking for low");

    if (mcp.digitalRead(8) == LOW){
      delay(20);
      pulseStart = millis();
      Serial.println("got start signal");
    }

    while (mcp.digitalRead(8) == LOW){
     Serial.println(millis());
     Serial.println(pulseStart);
      if (millis() - pulseStart > 5000){
        Serial.println("Pulse in is too long, Play again");
        
        break;
      }
      delay(1);
    }
    
    if (millis() - pulseStart < 5000) {

      pulseLength = (millis() - pulseStart);
      Serial.print("PulseLength : ");
      Serial.println(pulseLength);
      number_of_steps = pulseLength;  // we will tell stepper to step number equal to the length of the pulse in ms
              
    }

    armed = false;

  }
  else {
    //Serial.println("WTF");
      }    
  // if (mcp.digitalRead(8)){
  //   mcp.digitalWrite(0, HIGH);
  // } else{
  //   mcp.digitalWrite(0, LOW);
  // }

  if(pulseLength > 0){
   move(number_of_steps, delay_ms);
  //  move(testLength, delay_ms);
  }
  


// mcp.digitalWrite(0, HIGH);
// delay(1000);
// mcp.digitalWrite(0, LOW);

  // adc0 = ads.readADC_SingleEnded(0);
  // adc1 = ads.readADC_SingleEnded(1);
  // adc2 = ads.readADC_SingleEnded(2);
  // adc3 = ads.readADC_SingleEnded(3);
  // Serial.print("AIN0: "); Serial.println(adc0);
  // Serial.print("AIN1: "); Serial.println(adc1);
  // Serial.print("AIN2: "); Serial.println(adc2);
  // Serial.print("AIN3: "); Serial.println(adc3);
  // Serial.println(" ");
  
  delay(1);

}

void move(int theDistance, int theDelay){
  
    for (int i = 0; i < theDistance; i++){
      mcp.digitalWrite(0, HIGH);
      delay(1);
      mcp.digitalWrite(0, LOW);
      delay(theDelay);
      Serial.print(i); Serial.println(theDistance);      
      
    }

} 
