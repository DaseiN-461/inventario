typedef struct{
  int id;
  String product_name;
  int quantity;
}Product;


const int MAX_PRODUCTS = 5;
// Arreglo de productos
Product products[MAX_PRODUCTS];


// Variables temporales
// producto seleccionado
int temp_product = 0;
// tipo de proceso seleccionado | false: egreso | true: ingreso
int temp_process = false;
// cantidad de producto seleccionado
int temp_quantity = 0;



// WiFi
const char* ssid = "Copérnico 5G";
const char* password = "MMM...copito";

// MQTT Broker
const char* mqtt_server = "mqtt.eclipseprojects.io";
const int mqtt_port = 1883;
const char* mqtt_topic = "Mycodo/inventario/2";  // Tema en el que Mycodo espera recibir los datos
