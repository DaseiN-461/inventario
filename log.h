#include "defs.h"
#include <time.h>



void setup_wifi();
void viewLog();
void fetchInventory();
bool updateVerify();
void modifyInventory(int id, bool _process, int _quantity);
void sendInventory();
void save_updateTime();
void updateInventory();
void save_log_in_eeprom();
// La información guardada en la EEPROM sólo servirá en caso de que la conexion
// con la base de datos falle. Cuando actualice la información y encuentre
// inconsistencias debe informar en pantalla para así corregir rapidamente
// la base de datos.
// Además, debería guardar en la eeprom la última fecha de actualización 
// con la base de datos y mostrarlo en pantalla.





void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}



void viewLog(){
  Serial.println("------INVENTARIO------");
  for(int i=0; i<MAX_PRODUCTS; i++){
    if(inventory[i].name != "No registrado"){
      Serial.print("Name: " + inventory[i].name);
      Serial.printf(", Cantidad: %i,",inventory[i].quantity);
      Serial.println(inventory[i].updated ? "Actualizado" : "No actualizado");
    }
  }
}

void fetchInventory() {
  HTTPClient http;
  http.begin(scriptUrl);
  int httpCode = http.GET();

  // Manejar redirecciones
  while (httpCode == HTTP_CODE_MOVED_PERMANENTLY || httpCode == HTTP_CODE_FOUND) {
    String newLocation = http.getLocation();
    http.end();
    http.begin(newLocation);
    httpCode = http.GET();
  }

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    //Serial.println("Datos recibidos:");
    //Serial.println(payload);

    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("Error al parsear JSON: ");
      Serial.println(error.c_str());
      return;
    }

    JsonArray jsonArray = doc["inventory"];
    const char* lastUpdate = doc["lastUpdate"];

    Serial.println("Actualizando inventario:");
    int i=0;
    for (JsonObject item : jsonArray) {
      const char* name = item["name"];
      int quantity = item["quantity"];
      inventory_from_update[i].name = name;
      inventory_from_update[i].quantity = quantity;
      inventory_from_update[i].updated = true;
      i++;
    }

    updateInventory();
    
    viewLog();
      
    // Convertir la cadena de fecha y hora a time_t
    struct tm tm;
    if (strptime(lastUpdate, "%Y-%m-%dT%H:%M:%S", &tm) != NULL) {
      time_t lastUpdateTime = mktime(&tm);
      Serial.print("Última actualización: ");
      Serial.printf("%d/%d\t%d:%d:%d",tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
    } else {
      Serial.println("Error al convertir la fecha y hora");
    }
    

  } else {
    Serial.print("Error en la petición HTTP: ");
    Serial.println(httpCode);
  }

  http.end();
  
}

bool updateVerify(){
    bool inventoryMatch = true;
    // Verificar si la actualización no tiene conflictos
    Serial.println("Verificando conflictos con la actualizacion");
    
    for(int i=0;i<MAX_PRODUCTS;i++){
      if(inventory_from_update[i].name != "No registrado"){
        inventory[i].updated = true;
        if(inventory_from_update[i].name != inventory[i].name){
          Serial.print("Nombre del producto ");
          Serial.println(i);
          inventory[i].updated = false;
          inventoryMatch = false;
        }
        if(inventory_from_update[i].quantity != inventory[i].quantity){
          Serial.print("La cantidad del producto ");
          Serial.println(i);
          inventory[i].updated = false;
          inventoryMatch = false;
        }
      }
    }
    return inventoryMatch;
}


void modifyInventory(int id, bool _process, int _quantity) {
  if(_process){
    inventory[id].quantity += _quantity;
  }else{
    inventory[id].quantity -= _quantity;
  }
}

void sendInventory() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptUrl);
    http.addHeader("Content-Type", "application/json");

    // Crear un documento JSON
    StaticJsonDocument<1024> doc;
    JsonArray array = doc.to<JsonArray>();

    for (int i = 0; i < 20; i++) {
      JsonObject obj = array.createNestedObject();
      obj["name"] = inventory[i].name;
      obj["quantity"] = inventory[i].quantity;
    }

    // Convertir JSON a String
    String jsonStr;
    serializeJson(doc, jsonStr);

    // Enviar solicitud POST
    int httpResponseCode = http.POST(jsonStr);

    // Manejar redirecciones
    while (httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY || httpResponseCode == HTTP_CODE_FOUND) {
      String newLocation = http.getLocation();
      http.end();
      http.begin(newLocation);
      http.addHeader("Content-Type", "application/json");
      httpResponseCode = http.POST(jsonStr);
    }

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error en la petición HTTP: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi no conectado");
  }
}

void updateInventory(){
  // Verifica si updateverify está definida
  #ifdef updateVerify
    if(updateVerify()){
      for(int i=0;i<MAX_PRODUCTS;i++){
        inventory[i]=inventory_from_update[i];
      }
      Serial.print("Inventario actualizado,\tÚltima actualización:");
      Serial.println(lastUpdate);
    }  
  #else
    for(int i=0;i<MAX_PRODUCTS;i++){
      inventory[i]=inventory_from_update[i];
    }
    Serial.print("Inventario actualizado,\tÚltima actualización:");
    Serial.println(lastUpdate);
  #endif
}

void save_log_in_eeprom(){
  // Implementar funcion para guardar el registo en la memoria no volatil
  Serial.println("Guardar en la eeprom (falta implementar)");
}

void save_updateTime(){
  
}
