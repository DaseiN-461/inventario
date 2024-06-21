#include "FSM.h"



void setup() {
  Serial.begin(115200);

  touchButtons_init();

  
  log_init();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}

void loop() {
  FSM_UI_Handler();
  //FSM_UI_output();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
