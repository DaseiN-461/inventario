#include "FSM.h"


void setup() {
  Serial.begin(115200);

  touchButtons_init();
  createLog();
  viewLog();
  
}

void loop() {
  FSM_UI_Handler();
  //FSM_UI_output();
}
