#define TOUCH_PIN_UP  T9  // Pin táctil T4 pin13
#define TOUCH_PIN_DOWN  T4  // Pin táctil T9 pin32
#define TOUCH_PIN_SEL  T7  // Pin táctil T7 pin27
#define TOUCH_PIN_LEFT  T0  // Pin táctil T0 pin04
#define TOUCH_PIN_RIGHT  T3  // Pin táctil T3 pin15

int umbral = 8;

volatile bool touchUpPressed = false;
volatile bool touchDownPressed = false;
volatile bool touchSelPressed = false;
volatile bool touchLeftPressed = false;
volatile bool touchRightPressed = false;


void touchButtons_init();
void button_release();
void IRAM_ATTR touchUpISR();
void IRAM_ATTR touchDownISR();
void IRAM_ATTR touchSelISR();
void IRAM_ATTR touchLeftISR();
void IRAM_ATTR touchRightISR();


void touchButtons_init(){
  // Configurar los pines táctiles como entradas
  touchAttachInterrupt(TOUCH_PIN_UP, touchUpISR, umbral); // Umbral de sensibilidad táctil (ajustar según necesidades)
  touchAttachInterrupt(TOUCH_PIN_DOWN, touchDownISR, umbral); // Umbral de sensibilidad táctil (ajustar según necesidades)
  touchAttachInterrupt(TOUCH_PIN_SEL, touchSelISR, umbral); // Umbral de sensibilidad táctil (ajustar según necesidades)
  touchAttachInterrupt(TOUCH_PIN_LEFT, touchLeftISR, umbral); // Umbral de sensibilidad táctil (ajustar según necesidades)
  touchAttachInterrupt(TOUCH_PIN_RIGHT, touchRightISR, umbral); // Umbral de sensibilidad táctil (ajustar según necesidades)
}

void button_release(){
  touchSelPressed = false;
  touchLeftPressed = false;
  touchRightPressed = false;
  touchUpPressed = false;
  touchDownPressed = false;
}

void IRAM_ATTR touchUpISR() {
  touchUpPressed = true;
}

void IRAM_ATTR touchDownISR() {

  touchDownPressed = true;
}

void IRAM_ATTR touchSelISR() {
  touchSelPressed = true;
}
void IRAM_ATTR touchLeftISR() {
  touchLeftPressed = true;
}
void IRAM_ATTR touchRightISR() {
  touchRightPressed = true;
}
