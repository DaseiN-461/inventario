#include "defs.h"

void log_init();
void save_log_in_eeprom();

void log_init(){
  // Inicializa el registro. Eventualmente deberá iniciarlo desde la memoria no volatil eeprom
  createLog();
  viewLog();
}


void save_log_in_eeprom(){
  // Implementar funcion para guardar el registo en la memoria no volatil
  Serial.println("Guardar en la eeprom (falta implementar)");
}
