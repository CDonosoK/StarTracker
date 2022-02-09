#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <ESP32Servo.h>

//OLED Pin SDA - SCK
Adafruit_SH1106 display(13, 14);
TinyGPS gps;
SoftwareSerial ss;

//Threads

TaskHandle_t tarea1;
TaskHandle_t tarea2;

//Para guardar los datos GPS
String gpsData = "";
String latitud = "Latitud: ", longitud = "Longitud: ", fecha = "Fecha: -";
char buff[10];

//Potenciometro
const int pinPotenciometro = 35;
int valorPotenciometro, valorAntiguo = 0;

//Motores
Servo motorVertical, motorHorizontal;

//GPS data
float flat, flon;  
unsigned long age;
int ano, estrellaSeleccionada = 0, accionSeleccionada = 2;
byte mes, dia, hora, minuto, segundo, extra;

//Link base de datos: http://www.stellar-database.com/
//Array de estrellas y planetas
String arrayEstrellas[13][3] ={
  {"MERCURIO", "19#50#19", "-19#3#29"},
  {"VENUS", "18#59#56", "-16#11#15"},
  {"MARTE", "18#46#45", "-23#34#51"},
  {"JUPITER", "22#42#54", "-9#11#22"},
  {"SATURNO", "21#13#49", "-16#55#7"},
  {"URANO", "2#34#42", "+14#45#6"},
  {"NEPTUNO", "23#30#3", "-4#27#11"},
  {"SIRIO", "6#45#9", "-16#42#58"},
  {"BETELGEUSE", "5#55#10", "+7#24#26"},
  {"ALDEBARAN", "4#35#55", "16#30#33"},
  {"ALFA-CENTAURI", "14#39#37", "-60#50#2"},
  {"ANTARES", "16#29#24", "-26#25#55"},
  {"CAPELLA", "5#16#41", "+45#59#53"}
  };

int cantidadEstrellas = (sizeof(arrayEstrellas)/(sizeof(String)*3))-1;
int azimuth, altitud;

//Boton Seleccionador
int pinBoton = 26, botonApretado = LOW;
bool buscar, habilitadoBuscar = false, estrellaEncontrada = false;

void setup(void) {
  Serial.begin(9600);
  buscar = false;
  display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.setRotation(2);

  //Se inicializa el GPS Pin Tx - Rx 
  ss.begin(9600, SWSERIAL_8N1, 33, 32, false);
  if(!ss){
    while (1) {
      delay (1000);
    }
  }

  //Se seleccionan los pines de los motores
  motorVertical.attach(16);
  motorHorizontal.attach(17);

  //Seleccionador de Modos
  pinMode(pinPotenciometro, INPUT);
  pinMode(pinBoton, INPUT);

  //Se muestra el menú inicial
  drawMainScreen();
}

void drawMenu(){
  //Movimiento de la pantalla
  valorPotenciometro = int(map(analogRead(pinPotenciometro), 0, 4095, 0, 200));
  botonApretado = digitalRead(pinBoton);
  
  if ((valorAntiguo - valorPotenciometro)< -2){
    valorAntiguo = valorPotenciometro;
    accionSeleccionada += 1;
  }
  else if ((valorAntiguo - valorPotenciometro) > 2){
    valorAntiguo = valorPotenciometro;
    accionSeleccionada -= 1;
  }
  if (accionSeleccionada < 0){
    accionSeleccionada = 0;
  }
  else if (accionSeleccionada >2){
    accionSeleccionada = 2;
  }
  
  if (estrellaSeleccionada > cantidadEstrellas ){
    //No superamos el array
    estrellaSeleccionada = cantidadEstrellas;
  }
  else if (estrellaSeleccionada <0){
    //No superamos el array
    estrellaSeleccionada = 0;
  }

  if (accionSeleccionada == 0){
    drawData(arrayEstrellas[estrellaSeleccionada][0], "BUSCAR", "<", ">");
    drawData(arrayEstrellas[estrellaSeleccionada][0], "BUSCAR", "", ">"); 
    if (botonApretado == HIGH){
      estrellaSeleccionada -= 1;
    }
  }
  else if (accionSeleccionada == 1){
    drawData(arrayEstrellas[estrellaSeleccionada][0], "BUSCAR", "<", ">"); 
    drawData(arrayEstrellas[estrellaSeleccionada][0], "BUSCAR", "<", ""); 
    if (botonApretado == HIGH){
      estrellaSeleccionada += 1;
    }
  }
  else if (accionSeleccionada == 2){
    drawData(arrayEstrellas[estrellaSeleccionada][0], "BUSCAR", "<", ">"); 
    drawData(arrayEstrellas[estrellaSeleccionada][0], "", "<", ">"); 
    habilitadoBuscar = true;
    if (botonApretado == HIGH and habilitadoBuscar){
      azimuth = 20;
      altitud = 160;
      
      //Threads 1
      xTaskCreatePinnedToCore(
        drawSearchScreen,
        "Modificar pantalla",
        1000,
        NULL,
        1,
        &tarea1,
        1);

      moveMotors(azimuth, altitud);
        
       
    }
  }
}

void loop(void) {
  drawMenu();
}

void moveMotors(int azimuth, int altitud){
  if (azimuth >= 90){
    for (int i = 90; i <= azimuth; i++){
    motorHorizontal.write(i);
    smartdelay(25);
    } 
  }
  else{
    for (int i = 90; i >= azimuth; i--){
    motorHorizontal.write(i);
    smartdelay(25);
    } 
  }

  if (altitud >= 90){
    for (int i = 90; i <= altitud; i++){
    motorVertical.write(i);
    smartdelay(25);
    } 
  }
  else{
    for (int i = 90; i >= altitud; i--){
    motorVertical.write(i);
    smartdelay(25);
    } 
  }

  estrellaEncontrada = true;
  display.clearDisplay();
  display.display(); 
}

int *obtenerCoordenada(String value){
  static int coordenadas[3] = {0,0,0};

  return coordenadas;
}

void definirCoordenadas(double ){
  
}

void cambiarCoordenadas(){
  
}

void drawSearchScreen(void * pvParameters){
  //Pantalla de transición mientras se busca la estrella

  for (;;){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(30,25);
    display.println("BUSCANDO");
    display.display();
    smartdelay(200);
    display.clearDisplay();
    display.setCursor(30,25);
    display.println("BUSCANDO.");
    display.display();
    smartdelay(200);
    display.clearDisplay();
    display.setCursor(30,25);
    display.println("BUSCANDO..");
    display.display();
    smartdelay(200);
    display.clearDisplay();
    display.setCursor(30,25);
    display.println("BUSCANDO...");
    display.display();

    if (estrellaEncontrada){
      break;
    }
    display.clearDisplay();
    display.display(); 
  }
  
}

void drawMainScreen(){
  //Pantalla Inicial
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(display.width()/2-20,5);
  display.println("Star"); 
  display.setCursor(display.width()/2-40,30);
  display.println("Tracker"); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 52);
  display.println("by InnovacionesDoMa");
  display.display();
  smartdelay(5000);
  display.clearDisplay();
}

void drawData(String star, String botonBuscar, String botonIzq, String botonDer){
    //Obtener data del GPS y mostrarla por pantalla
    gps.f_get_position(&flat, &flon, &age);
    latitud += floatToString(flat,TinyGPS::GPS_INVALID_F_ANGLE, 6, 3);
    longitud += floatToString(flon,TinyGPS::GPS_INVALID_F_ANGLE, 6, 3);
    display.setTextSize(0);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(latitud);
    display.setCursor(0,10);
    display.println(longitud);
    display.setCursor(0,20);
    
    //Se obtienen los datos de la fecha
    gps.crack_datetime(&ano, &mes, &dia, &hora, &minuto, &segundo, &extra, &age);
    fecha += String(static_cast<int>(dia));
    fecha += "/";
    fecha += String(static_cast<int>(mes));
    fecha += "/";
    fecha += String(ano);

    //En caso de no estar conectados a la red, se establece la fecha por defecto
    if (fecha == "Fecha: 0/0/2000"){
      fecha = "Fecha: -";
      habilitadoBuscar = false;
    }
    else{
      habilitadoBuscar = true;
    }
    display.println(fecha);

    //Se agrega el divisor
    display.drawLine(0,30,display.width(),30, WHITE);
    
    //Se muestra la estrella que se desea buscar
    display.setCursor(display.width()/2 - 3*star.length(),37);
    display.println(star);
    display.setCursor(0,37);
    display.println(botonIzq);
    display.setCursor(display.width()-6,40);
    display.println(botonDer);
    display.display();
    display.clearDisplay();

    //Boton buscar
    display.setCursor(display.width()/2 - 3*botonBuscar.length(),52);
    display.println(botonBuscar);
    
    smartdelay(150);

    //Se vuelven a setear las variables originales
    latitud = "Latitud: ";
    longitud = "Longitud: ";
    fecha = "Fecha: ";
}

//Funcion para pasar de float a string
String floatToString(float val, float invalid, int len, int prec) {
  String out = "";
  if (val == invalid) {
    while (len-- > 1){
      return "-" ;
    }
  }
  else{
    for (int i = 0; i < 10; i++) {
       dtostrf(val, len, prec, buff);  
       out += buff;
       return out;
    }
  }
}

//Funcion auxiliar
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
