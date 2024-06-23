
#include "touchButtons.h"

#include "log.h"

////////////////////////////////////////////////// FSM Definicion de estados //////////////////////////////////////////////////
enum state_UI{
  
  sel_product,        // seleccionar perno
  sel_process,       // seleccionar si se desea agregar o quitar
  sel_quantity      // seleccionar la cantidad
  
};
state_UI currentStateUI = sel_product;


void FSM_UI_Handler();
void FSM_next_state(state_UI state);

void FSM_next_state(state_UI state){
  delay(1000);
  button_release();
  currentStateUI = state;
  Serial.printf("state -> [%i]\r\n",state);  
}

void productSelection();
void temp_processSelection();
void quantitySelection();



void productSelection(){
  int delay_time = 500;
  if(touchDownPressed){
        delay(delay_time);
        button_release();
        if(temp_product>0){
          temp_product--;
        }
        Serial.printf("\tProducto seleccionado: %s\r\n",inventory[temp_product].name);
  }
  if(touchUpPressed){
        delay(delay_time);
        button_release();
        if(inventory[temp_product+1].name != "No registrado"){
          temp_product++;
        }
        Serial.printf("\tProducto seleccionado: %s\r\n",inventory[temp_product].name);
  }
}



void temp_processSelection(){
  int delay_time = 100;
  if(touchDownPressed){
        delay(delay_time);
        button_release();
        temp_process = false;
        Serial.printf("\t\ttemp_process: ");
        Serial.println(temp_process ? "Añadir" : "Quitar");
  }
  if(touchUpPressed){
        delay(delay_time);
        button_release();
        temp_process = true;
        Serial.printf("\t\ttemp_process: ");
        Serial.println(temp_process ? "Añadir" : "Quitar");
  }
}

void quantitySelection(){
  int delay_time = 100;
  if(touchDownPressed){
        delay(delay_time);
        button_release();
        if(temp_quantity>0){
          temp_quantity--;
        }
        Serial.printf("\tCantidad seleccionado: %i\r\n",temp_quantity);
  }
  if(touchUpPressed){
        delay(delay_time);
        button_release();
        Serial.printf("Name %s, quantity %d", inventory[temp_product].name, inventory[temp_product].quantity);
        if(temp_process == false){
          if(temp_quantity < inventory[temp_product].quantity){
            temp_quantity++;
          }
        
        }
        Serial.printf("\tCantidad seleccionado: %i\r\n",temp_quantity);
  }
}

void FSM_UI_Handler(){
  switch(currentStateUI){
    case sel_product:
      productSelection();
      
      if(touchRightPressed){
        FSM_next_state(sel_process);
        Serial.printf("Product selected: %s\r\n",inventory[temp_product].name);
      }
      
      break;
       
    case sel_process:
      if(touchSelPressed){
        FSM_next_state(sel_quantity);
        Serial.printf("Product selected: %s\r\n",inventory[temp_product].name);
      }
      if(touchLeftPressed){
        FSM_next_state(sel_product);
      }
      temp_processSelection();
      break;
      
    case sel_quantity:
      if(touchRightPressed){
        FSM_next_state(sel_product);  
        modifyInventory(temp_product, temp_process, temp_quantity);

        //save_log_in_eeprom();
        viewLog();

        sendInventory();
        
        button_release();
        currentStateUI = sel_product;
        temp_product = 0;
        temp_process = false;
        temp_quantity = 0;
        
        
             
      }
      if(touchLeftPressed){
        button_release();
        temp_quantity = 0;
        FSM_next_state(sel_process);
      }
      quantitySelection();
      
      break;
  }
}
