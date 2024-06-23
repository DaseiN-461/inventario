#include "defs.h"




void setup_wifi();
void viewLog();
void fetchInventory();
void modifyInventory(int id, bool _process, int _quantity);
void sendInventory();

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
    Serial.println("Name: " + inventory[i].name);
    Serial.printf("Cantidad: %i\r\n",inventory[i].quantity);
  }
}

void fetchInventory() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptUrl);

    int httpResponseCode = http.GET();

    // Manejar redirecciones
    while (httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY || httpResponseCode == HTTP_CODE_FOUND) {
      String newLocation = http.getLocation();
      http.end();
      http.begin(newLocation);
      httpResponseCode = http.GET();
    }

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);

      // Parsear JSON
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, response);

      if (!error) {
        JsonArray array = doc.as<JsonArray>();

        for (int i = 0; i < array.size(); i++) {
          JsonObject obj = array[i];
          inventory[i].name = obj["name"].as<String>();
          inventory[i].quantity = obj["quantity"].as<int>();
        }

        // Imprimir inventario actualizado
        for (int i = 0; i < 20; i++) {
          Serial.print("Producto: ");
          Serial.print(inventory[i].name);
          Serial.print(" - Cantidad: ");
          Serial.println(inventory[i].quantity);
        }
      } else {
        Serial.print("Error al parsear JSON: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("Error en la petición HTTP: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi no conectado");
  }
  viewLog();
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



void save_log_in_eeprom(){
  // Implementar funcion para guardar el registo en la memoria no volatil
  Serial.println("Guardar en la eeprom (falta implementar)");
}
