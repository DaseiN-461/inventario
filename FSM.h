
#include "touchButtons.h"
#include "defs.h"

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
        Serial.printf("\tProducto seleccionado: %i\r\n",temp_product);
  }
  if(touchUpPressed){
        delay(delay_time);
        button_release();
        if(temp_product<MAX_PRODUCTS){
          temp_product++;
        }
        Serial.printf("\tProducto seleccionado: %i\r\n",temp_product);
  }
}



void temp_processSelection(){
  int delay_time = 100;
  if(touchDownPressed){
        delay(delay_time);
        button_release();
        temp_process = false;
        Serial.printf("\t\ttemp_process: ");
        Serial.println(temp_process ? "true" : "false");
  }
  if(touchUpPressed){
        delay(delay_time);
        button_release();
        temp_process = true;
        Serial.printf("\t\ttemp_process: ");
        Serial.println(temp_process ? "true" : "false");
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

        temp_quantity++;
        
        Serial.printf("\tCantidad seleccionado: %i\r\n",temp_quantity);
  }
}

void FSM_UI_Handler(){
  switch(currentStateUI){
    case sel_product:
      productSelection();
      
      if(touchRightPressed){
        FSM_next_state(sel_process);
        
      }
      
      break;
       
    case sel_process:
      if(touchSelPressed){
        FSM_next_state(sel_quantity);
      }
      if(touchLeftPressed){
        FSM_next_state(sel_product);
      }
      temp_processSelection();
      break;
      
    case sel_quantity:
      if(touchRightPressed){
        FSM_next_state(sel_product);  
        logUpdate(temp_product,temp_process,temp_quantity);
        temp_product = 0;
        temp_process = false;
        temp_quantity = 0;
        viewLog();     
      }
      if(touchLeftPressed){
        FSM_next_state(sel_process);
      }
      quantitySelection();
      
      break;
  }
}
