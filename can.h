#include <mcp2515.h>                                    /// https://github.com/autowp/arduino-mcp2515
MCP2515 mcp2515(10);

unsigned long last_can;                                 /// Holds time in ms.

struct can_frame MSG;                                   /// Generic CAN message
struct can_frame DASH_MSG = { 0x036, 2 };               /// Our specified dashboard message

void CAN_setup() {
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ);             ///Initializing mcp2515 CAN, speed/bitrate, sets to normal mode
  mcp2515.setNormalMode();
  last_can = millis();
}

void CAN_update() {
  
  if (mcp2515.readMessage(&MSG) == MCP2515::ERROR_OK){      /// If there are no errors, start reading message pointer
    
    if(DMC_CTRL.can_id == MSG.can_id) {
//      for(int j=0; j<7; j++)
//        Serial.print(bitRead(MSG.data[0],j ));
//      Serial.println();                                     /// PROBLEM WITH FIRST BYTE OF DMC_CONTROL
      for(int i=0; i<6; i++){
        Serial.print(MSG.data[i], BIN);
        Serial.println();
      }
      Serial.println();

    }
  }
}
