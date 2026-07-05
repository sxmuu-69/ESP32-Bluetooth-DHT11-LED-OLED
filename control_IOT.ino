#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 124
#define SCREEN_HIGH 64

Adafruit_SSD1306 display(SCREEN_WIDTH , SCREEN_HIGH , &Wire , -1);

class Sensor{
  private:
    int pin;
    DHT dht;

  public:
    Sensor(int p): dht(p , DHT11){
      pin = p;
    } 

    void iniciar(){
      dht.begin();
    } 

    float temperatura(){
      float temp = dht.readTemperature();
      return temp;
    }

    float humedad(){
      float hume = dht.readHumidity();
      return hume;
    }
};

class LED{
  private:
    int pin;
    int frecuencia; 
    int bits;
    int brillo;
    int direccion;
    unsigned long tiempoAnterior;

  public:
    LED(int p , int f , int b){
      pin = p;
      frecuencia = f;
      bits = b;
      brillo = 0;
      direccion = 1;
      tiempoAnterior = 0;
    }

    void iniciar(){
      ledcAttach(pin , frecuencia , bits);
    }

    void setBrillo(int valor){
      brillo = valor;
      ledcWrite(pin , valor);
    }

    int getBrillo(){
      return brillo;
    }

    void fade(){
      unsigned long actual = millis();
      if(actual - tiempoAnterior >= 10){
        tiempoAnterior = actual;

        brillo += direccion;

        if(brillo >= 255){
          direccion = -1;
        }
        if(brillo <= 0){
          direccion = 1;
        }
        ledcWrite(pin , brillo);
      }
    }
};

LED led(5 , 5000 , 8);
Sensor sen(4);
volatile bool cambiarModo = false;

void tareaSensor(void* parameter){
while(true){
  float temp = sen.temperatura();
  float hume = sen.humedad();

  Serial1.print("TEMPERATURA: ");
  Serial1.print(temp);
  Serial1.print(" C || HUMEDAD: ");
  Serial1.print(hume);
  Serial1.println("%");
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void tareaLED(void* parameter){
  led.iniciar();
  while(true){
    if(Serial1.available()){
      String mensaje = "";

      while(Serial1.available()){
        char c = Serial1.read();
        mensaje += c;
        delay(5);
      }

      mensaje.trim();
       float brightness = mensaje.toInt();

      if(mensaje == "ON"){
        cambiarModo = false;
        led.setBrillo(255);
      }

      if(mensaje == "OFF"){
        cambiarModo = false;
        led.setBrillo(0);
      }

      if(mensaje == "AUTO"){
        cambiarModo = true;
        }

      if(mensaje != "ON" && mensaje != "OFF" && mensaje != "AUTO"){
        cambiarModo = false;
        led.setBrillo(brightness);
      }
    }

     if(cambiarModo){
        led.fade();
      }
    }
}

void tareaOLED(void* parameter){
  while(true){
    
     int brillo = led.getBrillo();

    if(cambiarModo){
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0 , 0);
      display.print("Mode: AUTOMATICO");
      display.setCursor(0 , 25);
      display.print("BRIGHTNESS: ");
      display.print(map(brillo , 0 , 255 , 0 , 100));
      display.print("%");
      display.display();
    }
   
    if(!cambiarModo){
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0 , 0);
      display.print("Mode: MANUAL");
      display.setCursor(0 , 25);
      display.print("BRIGHTNESS: ");
      display.print(map(brillo , 0 , 255 , 0 , 100));
      display.print("%");
      display.display();
    }
  }
}

void setup(){
  Serial1.begin(9600 , SERIAL_8N1 , 16 , 17);
  sen.iniciar();
  xTaskCreatePinnedToCore(tareaSensor , "SENSOR" , 8192 , NULL , 1 , NULL , 0);
  xTaskCreatePinnedToCore(tareaLED , "LED" , 8192 , NULL , 1 , NULL , 1);
  xTaskCreatePinnedToCore(tareaOLED , "OLED" , 8192 , NULL , 1 , NULL , 1);

  if(!display.begin(SSD1306_SWITCHCAPVCC , 0x3c)){
    Serial.print("OLED no encontrada");
    while(true);
    }
    display.setTextColor(WHITE);
}

void loop(){

}