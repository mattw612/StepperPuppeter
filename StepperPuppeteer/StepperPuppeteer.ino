

// (0.597mm dispense)/(1 pulse to stepper driver) updated 3.30 


// Send a high signal for more than 0 and less than 200 ms to move stepper for 'First Dispense'
// Send a high signal for more than 199 and less than 400 ms to move stepper for 'Second Dispense'



#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_MCP23X17.h>

Adafruit_MCP23X17 mcp;
Adafruit_ADS1015 ads; /* Use this for the 12-bit version, 73 if the jumper is shorted) */

int number_of_steps = 0;
int pulseLength = 0;
long pulseStart = 0;
int pulseEnd = 0;
bool armed = false;
int testLength = 500;

int retractForCut = 5;  
int feedForCut = 1653;


#define signalInputPin 8
#define pulseOutputPin 0
#define delay_ms 1  // pulse delay in ms

void setup() {
  mcp.begin_I2C(0x21);  // use default address 0x21

  Serial.begin(9600);

  ads.setGain(GAIN_ONE);
  ads.begin();
  mcp.pinMode(pulseOutputPin, OUTPUT);
  mcp.pinMode(signalInputPin, INPUT);
}



void loop() {

  pulseLength = 0;

  if (mcp.digitalRead(8) == HIGH) {
    armed = true;

  } else if (armed == true) {

    Serial.print("checking for low");

    if (mcp.digitalRead(8) == LOW) {
      delay(20);
      pulseStart = millis();
      Serial.println("got start signal");
    }

    while (mcp.digitalRead(8) == LOW) {
      Serial.println(millis());
      Serial.println(pulseStart);
      if (millis() - pulseStart > 5000) {
        Serial.println("Pulse in is too long, Play again");

        break;
      }
    }

    if (millis() - pulseStart < 5000) {

      pulseLength = (millis() - pulseStart);
      Serial.print("PulseLength : ");
      Serial.println(pulseLength);
    }

    armed = false;

  } else {
    //Serial.println("WTF");
  }


  if (pulseLength > 0) {
    move(pulseLength, delay_ms);
    //  move(testLength, delay_ms);
  }

  delay(1);
}

void move(int pulseL, int theDelay) {

  if (pulseL > 0 && pulseL < 200) {
    for (int i = 0; i < feedForCut; i++) {
      mcp.digitalWrite(0, HIGH);
      delay(1);
      mcp.digitalWrite(0, LOW);
      delay(theDelay);
    }
  } else if (pulseL > 199 && pulseL < 4000) {
    for (int i = 0; i < retractForCut; i++) {
      mcp.digitalWrite(0, HIGH);
      delay(1);
      mcp.digitalWrite(0, LOW);
      delay(theDelay);
    }
  } else {
    Serial.println("Pulse length out of expected range");
  }
}

