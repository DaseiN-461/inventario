#include "defs.h"
#include "mqtt.h"



void createLog();
void viewLog();
void log_init();

void logUpdate(int product, bool process, int cantity);
void save_log_in_eeprom();
void send_update_mqtt(int id);







void createLog(){
  for(int i=0; i<MAX_PRODUCTS; i++){
    products[i] = {i, "Unknown", 0};
  }
}
void viewLog(){
  Serial.println("------LOG------");
  for(int i=0; i<MAX_PRODUCTS; i++){
    
    Serial.printf("ID: %i\r\n",products[i].id);
    Serial.println("Name: " + products[i].product_name);
    Serial.printf("Cantidad: %i\r\n",products[i].quantity);
  }
}
void log_init(){
  // Inicializa el registro. Eventualmente deberá iniciarlo desde la memoria no volatil eeprom
  createLog();
  viewLog();
}

void logUpdate(int product, bool process, int quantity){
  Serial.printf("Product: %i\r\n Process: ",product);
  Serial.println(process ? "true" : "false");
  Serial.printf("Quantity: %i\r\n",quantity);
  if(process){
    products[product].quantity += quantity;
  }else{
    products[product].quantity -= quantity;
  }
}



void save_log_in_eeprom(){
  // Implementar funcion para guardar el registo en la memoria no volatil
  Serial.println("Guardar en la eeprom (falta implementar)");
}

void send_update_mqtt(int id){
  // Implementar funcion para enviar actualizacion via mqtt

  String msg = String(products[id].quantity);

  char topic[50];
  snprintf(topic, sizeof(topic), "Mycodo/inventario/product%d", id); // Formatea el tópico dinámicamente
    

  Serial.print("Publicado en el topico");
  Serial.println(topic);
  client.publish(topic, msg.c_str());
}
