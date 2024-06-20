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



void createLog();
void logUpdate(int product, bool process, int cantity);
void viewLog();

void createLog(){
  for(int i=0; i<MAX_PRODUCTS; i++){
    products[i] = {i, "Unknown", 0};
  }
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

void viewLog(){
  Serial.println("------LOG------");
  for(int i=0; i<MAX_PRODUCTS; i++){
    
    Serial.printf("ID: %i\r\n",products[i].id);
    Serial.println("Name: " + products[i].product_name);
    Serial.printf("Cantidad: %i\r\n",products[i].quantity);
  }
}
