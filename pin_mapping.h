#define startbutton 3
#define AMSled 4            /// It is unknown for me now how will the signal of the IMD or AMS communicate with the ESP. 
#define IMDled 5            /// I took a look at the datasheet of the IMD and I think it's possible to get an 2V yes-or-no singal of the status. I assumed something similar with the AMS. 
#define TSled 6
#define TS 7

void PIN_setup() {
  pinMode(TS, INPUT);
  pinMode(startbutton, INPUT);
  pinMode(IMDled, OUTPUT);
  pinMode(AMSled, OUTPUT);
  pinMode(TSled, OUTPUT);
}
