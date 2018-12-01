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

#define STEER_IN 4

int STEER = 0;

void setup() {  
  Serial.begin(115200);
  while (!Serial);

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
  
  STEER = map(STEER, 0, 600, 0, 255);
  STEER = constrain(STEER, 0, 255);
  Serial.println(STEER);

  // CAN SEND /////////////////////////////////////////////////////
  CAN.beginPacket(0x15);
  CAN.write(STEER);
  CAN.endPacket();

  delay(80);
  

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
