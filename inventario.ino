#include "FSM.h"

#include 

void setup() {
  Serial.begin(115200);

  touchButtons_init();

  
  log_init();
  
}

void loop() {
  FSM_UI_Handler();
  //FSM_UI_output();
}
