#include "FSM.h"



void setup() {
  Serial.begin(115200);

  touchButtons_init();

  setup_wifi();

  fetchInventory();
  

  

  
}

void loop() {
  FSM_UI_Handler();
 
}
