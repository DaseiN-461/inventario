// WiFi
const char* ssid = "Copérnico 5G";
const char* password = "MMM...copito";

// URL del script de Google Apps Script
const char* scriptUrl = "https://script.google.com/macros/s/AKfycbyzmbO4d70uM426vGb-P67jTANACuJ9tkI4sopNVSqS_l3TjI8iTn9jTHcbKNhxDP5K/exec";

const int MAX_PRODUCTS = 20;



// Define la clase Product
class Product {
  public:
    String name;
    int quantity;

    Product() {}

    Product(String n, int q) {
      name = n;
      quantity = q;
    }
};



// Arreglo de productos
Product inventory[MAX_PRODUCTS];


// Variables temporales
// producto seleccionado
int temp_product = 0;
// tipo de proceso seleccionado | false: egreso | true: ingreso
int temp_process = false;
// cantidad de producto seleccionado
int temp_quantity = 0;

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
