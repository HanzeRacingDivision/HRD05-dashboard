/*
      __  __                          ____             _                ____  _       _      _           
     / / / /___ _____  ____  ___     / __ \____ ______(_)___  ____ _   / __ \(_)   __(_)____(_)___  ____ 
    / /_/ / __ `/ __ \/_  / / _ \   / /_/ / __ `/ ___/ / __ \/ __ `/  / / / / / | / / / ___/ / __ \/ __ \
   / __  / /_/ / / / / / /_/  __/  / _, _/ /_/ / /__/ / / / / /_/ /  / /_/ / /| |/ / (__  ) / /_/ / / / /
  /_/ /_/\__,_/_/ /_/ /___/\___/  /_/ |_|\__,_/\___/_/_/ /_/\__, /  /_____/_/ |___/_/____/_/\____/_/ /_/ 
  
  Dashboard code for the HRD05
  Mark Oosting, 2020
  Armand Micu, 2020
  
*/

#include <SPI.h>
#include "variables.h"              /// Variables specific to the pedalbox
#include "variables_dmc.h"          /// Variables used for the DMC514 inverter
#include "pin_mapping.h"            /// Arduino pin mapping + input and output settings
#include "can.h"                    /// Our own CAN variables and code

/// Shutdown circuit: 12 Volts, 3 A. 

/// Brake pedal signal: CAN-bus. 

/// APPS signal: CAN-bus.

/// CAN-bus: 5V CAN-bus 500000 Bd (baudrate, symbols per second). 

void setup() {
  
  SPI.begin(); 
  Serial.begin(9600);

  PIN_setup();

  CAN_setup();
  
  startup_led_check();
  
}

void loop() {

  CAN_update();

  if(digitalRead(startbutton) == HIGH && digitalRead(TS) == HIGH)           /// Need to comment the "if" for testing purposes
  {
    DASH_MSG.can_id=0x036;
    DASH_MSG.can_dlc=2;                                                     /// Need to add latching logic for the startbutton ( not pressing the button it shouldn't reset it, but TS being off resets it
    DASH_MSG.data[0]=1;
    DASH_MSG.data[1]=0;
    mcp2515.sendMessage(&DASH_MSG);
    delay(100);
  }

  delay(500);

  if(digitalRead(TS) == HIGH)
    digitalWrite(TSled, LOW);
  else                                /// Tractive system light turns on if the tractive system is turned off.
    digitalWrite(TSled, HIGH);

}

void startup_led_check(){
  digitalWrite(TSled, HIGH);
  delay(300);
  digitalWrite(AMSled, HIGH);
  delay(300);
  digitalWrite(IMDled, HIGH);
  delay(300);
  digitalWrite(TSled, LOW);
  digitalWrite(AMSled, LOW);
  digitalWrite(IMDled, LOW);
  delay(300);
  digitalWrite(TSled, HIGH);
  digitalWrite(AMSled, HIGH);
  digitalWrite(IMDled, HIGH);
  delay(100);
  digitalWrite(TSled, LOW);
  digitalWrite(AMSled, LOW);
  digitalWrite(IMDled, LOW);     
}
