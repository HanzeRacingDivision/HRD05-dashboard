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
#include <mcp2515.h>    /// Some other libraries will be included when switching to the ESP32 + Transciever setup.
#define startbutton 3
#define AMSled 4           /// It is unknown for me now how will the signal of the IMD or AMS communicate with the ESP. 
#define IMDled 5           /// I took a look at the datasheet of the IMD and I think it's possible to get an 2V yes-or-no singal of the status. I assumed something similar with the AMS. 
#define TSled 6
#define TS 7

//Shutdown circuit: 12 Volts, 3 A. 

//Brake pedal signal: CAN-bus. 

//APPS signal: CAN-bus. 

//CAN-bus: 5V CAN-bus 500000 Bd (baudrate, symbols per second). 


MCP2515 mcp2515(10);
struct can_frame MSG;                       /// Generic CAN message 
struct can_frame DASH_MSG= {0x036, 2};
struct can_frame DMC_TRQS= {0x258, 8};
struct can_frame DMC_CTRL= {0x210, 8};

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

void setup() {
  SPI.begin(); 
  Serial.begin(9600);
  pinMode(startbutton, INPUT);
  pinMode(TS, INPUT);
  pinMode(IMDled, OUTPUT);
  pinMode(AMSled, OUTPUT);
  pinMode(TSled, OUTPUT);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ);             ///Initializing mcp2515 CAN, speed/bitrate, sets to normal mode
  mcp2515.setNormalMode();

  startup_led_check();
  
  

}

void loop() {

  if(digitalRead(startbutton) == HIGH && digitalRead(TS)==HIGH)               /// Need to comment the "if" for testing purposes
    {
      DASH_MSG.can_id=0x036;
      DASH_MSG.can_dlc=2;                                                     /// Need to add latching logic for the startbutton ( not pressing the button it shouldn't reset it, but TS being off resets it
      DASH_MSG.data[0]=1;
      DASH_MSG.data[1]=0;
      mcp2515.sendMessage(&DASH_MSG);
      delay(100);
    }

    delay(500);

    DMC_TRQS.can_id=0x258;
    DMC_TRQS.can_dlc=8;
    DMC_TRQS.data[0]=1;
    DMC_TRQS.data[6]=220;

    mcp2515.sendMessage(&DMC_TRQS);
    
   if(digitalRead(TS)==HIGH)
      digitalWrite(TSled, LOW);
      else                          /// Tractive system light turns on if the tractive system is turned off.
      digitalWrite(TSled, HIGH);


   if (mcp2515.readMessage(&MSG) == MCP2515::ERROR_OK){  /// If there are no errors, start reading message pointer
    
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
