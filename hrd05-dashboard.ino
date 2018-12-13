/*
      __  __                          ____             _                ____  _       _      _           
     / / / /___ _____  ____  ___     / __ \____ ______(_)___  ____ _   / __ \(_)   __(_)____(_)___  ____ 
    / /_/ / __ `/ __ \/_  / / _ \   / /_/ / __ `/ ___/ / __ \/ __ `/  / / / / / | / / / ___/ / __ \/ __ \
   / __  / /_/ / / / / / /_/  __/  / _, _/ /_/ / /__/ / / / / /_/ /  / /_/ / /| |/ / (__  ) / /_/ / / / /
  /_/ /_/\__,_/_/ /_/ /___/\___/  /_/ |_|\__,_/\___/_/_/ /_/\__, /  /_____/_/ |___/_/____/_/\____/_/ /_/ 
  
  CAN Reciever code for the HRD05
  Mark Oosting, Wout Veltman, 2019
  
*/

#include <CAN.h>
#include <TM1637Display.h>  //https://github.com/avishorp/TM1637

#define STEER_IN 4

const int CLK = 4; //Set the CLK pin connection to the display
const int DIO = 3; //Set the DIO pin connection to the display

TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.

int STEER = 0;

void setup() {  
  Serial.begin(115200);
  while (!Serial);

  display.setBrightness(0x0a);  //set the diplay to maximum brightness

  Serial.println("HRD05 DASHBOARD");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {

  // GET SENSOR VALUES /////////////////////////////////////////////
  STEER = analogRead(STEER_IN);
  
  STEER = map(STEER, 700, 1020, 0, 255);
  STEER = constrain(STEER, 0, 255);
  Serial.println(STEER);

  // CAN SEND /////////////////////////////////////////////////////
  CAN.beginPacket(0x15);
  CAN.write(STEER);
  CAN.endPacket();

  delay(50);

  // DISPLAY ///////////////////////////////////////////////////////
  display.showNumberDec(STEER); //Display the Variable value

//  // CAN READ /////////////////////////////////////////////////////
//  int packetSize = CAN.parsePacket();
//
//  if(packetSize) {
//
//    int i = 0;
//    int packetID = CAN.packetId();
//
//    byte data[11];
//
//    switch (packetID) {
//      case 0x258:
//        while (CAN.available()) {
//          data[i] = CAN.read();         
//          i++;
//        }
//
//        word SpdAct = data[6];
//        SpdAct << 8;
//        SpdAct | data[7];
//
//        Serial.println(SpdAct);
//       break;
//
//      default:
//        Serial.print("Unknown ID: ");
//        Serial.print(packetID);
//        Serial.println();
//        break;
//    }
//  }
}
