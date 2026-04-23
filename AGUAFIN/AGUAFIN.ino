#include <SPI.h>

#include <Adafruit_GFX.h>

#include <Adafruit_ILI9341.h>

#include <XPT2046_Touchscreen.h>

#include <Preferences.h>
Preferences prefs;

/* TFT */

#define TFT_CS 15

#define TFT_DC 2

#define TFT_RST 4



Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);



/* TOUCH */

#define TOUCH_CS 5

XPT2046_Touchscreen ts(TOUCH_CS);



/* SENSOR + RELE */

#define TRIG 32

#define ECHO 35

#define RELE 14



/* TOUCH CALIB */

#define TS_MINX 450

#define TS_MAXX 3800

#define TS_MINY 350

#define TS_MAXY 3700



/* COLORES */

#define BLACK 0x0000

#define WHITE 0xFFFF

#define BLUE 0x001F

#define RED 0xF800

#define GREEN 0x07E0

#define AGUA 0x5D9B



/* VARIABLES */

int pantalla = 1;

int peso = 60;



float litrosMeta = 0;

bool esRegistro = true; // true = Registro, false = Entrar

/* BOTON HOME */

#define BTN_HOME_X 25

#define BTN_HOME_Y 200

#define BTN_HOME_W 70

#define BTN_HOME_H 30

/* BOTONES INICIO */
#define BTN_REG_X 40
#define BTN_REG_Y 120
#define BTN_REG_W 110
#define BTN_REG_H 50
/* BOTONES ENTRAR */
#define BTN_ENT_X 170
#define BTN_ENT_Y 120
#define BTN_ENT_W 110
#define BTN_ENT_H 50

/* FLUJO */

float caudal = 0.037;

unsigned long tiempoInicio = 0;

bool llenando = false;



float volumenActual = 0;// Volumen de inicio

float volumenTotal = 0;

unsigned long ultimoUpdate = 0;

float volumenFinal = volumenTotal; 



/* TECLADO */

String teclas[4][7] = {

{"Q","W","E","R","T","Y","U"},

{"I","O","P","A","S","D","F"},

{"G","H","J","K","L","Z","X"},

{"C","V","B","N","M","DEL","OK"}

};



char nombreActual[12] = "";

int nombreIndex = 0;



/* ================= SENSOR ================= */



float medirDistancia() {

  digitalWrite(TRIG, LOW);

  delayMicroseconds(2);



  digitalWrite(TRIG, HIGH);

  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);



  long duracion = pulseIn(ECHO, HIGH, 30000);

  if (duracion == 0) return -1;



  return duracion * 0.034 / 2;

}



/* ================= UI BOTON HOME ================= */



void dibujarBotonHome(){

  tft.fillRect(BTN_HOME_X, BTN_HOME_Y, BTN_HOME_W, BTN_HOME_H, RED);

  tft.setTextColor(WHITE);

  tft.setTextSize(2);

  tft.setCursor(BTN_HOME_X + 10, BTN_HOME_Y + 8);

  tft.print("HOME");

}



bool tocarHome(int x, int y){

  if(x > BTN_HOME_X && x < BTN_HOME_X + BTN_HOME_W &&

     y > BTN_HOME_Y && y < BTN_HOME_Y + BTN_HOME_H){

    digitalWrite(RELE, LOW);

    pantallaBienvenida();

    return true;

  }

  return false;



  

}



/* ================= TECLADO ================= */
void dibujarPantallaEntrar() {
  pantalla = 7; // Identificador único para esta pantalla
  tft.fillScreen(BLACK);
  
  // Título 
  tft.setTextColor(GREEN); // Usamos verde para diferenciarlo del azul de registro
  tft.setTextSize(2);
  tft.setCursor(60, 10);
  tft.print("INGRESAR USUARIO");

  // Recuadro donde aparece el nombre que escribes
  tft.drawRect(40, 40, 240, 30, WHITE);
  tft.setCursor(50, 50);
  tft.setTextColor(WHITE);
  tft.print(nombreActual);

  // --- DIBUJO DE LAS TECLAS ---
  int x = 10;
  int y = 90;

  for(int f = 0; f < 4; f++) {
    for(int c = 0; c < 7; c++) {
      String tecla = teclas[f][c];

      // Colores de las teclas especiales
      if(tecla == "DEL") tft.fillRect(x, y, 40, 30, RED);
      else if(tecla == "OK") tft.fillRect(x, y, 40, 30, GREEN);
      else tft.fillRect(x, y, 40, 30, 0x0410); // Un azul más oscuro 

      tft.setCursor(x + 8, y + 8);
      tft.setTextColor(WHITE);
      tft.print(tecla);

      x += 45;
    }
    x = 10;
    y += 35;
  }
}


void dibujarTeclado(){



pantalla = 1;

tft.fillScreen(BLACK);

tft.setTextColor(WHITE);

tft.setTextSize(2);

tft.setCursor(70,10);

// Título dinámico
  if(esRegistro) {
    tft.print("Nuevo Registro");
  } else {
    tft.print("Ingrese Usuario"); // dar "Entrar"
  }



tft.drawRect(40,40,240,30,WHITE);

tft.setCursor(50,50);

tft.print(nombreActual);



int x = 10;

int y = 90;



for(int f=0; f<4; f++){

  for(int c=0; c<7; c++){



    String tecla = teclas[f][c];



    if(tecla == "DEL") tft.fillRect(x,y,40,30,RED);

    else if(tecla == "OK") tft.fillRect(x,y,40,30,GREEN);

    else tft.fillRect(x,y,40,30,BLUE);



    tft.setCursor(x+8,y+8);

    tft.print(tecla);



    x += 45;

  }

  x = 10;

  y += 35;

}

}



/* ================= PANTALLAS ================= */
void pantallaBienvenida() {
  pantalla = 0;
  tft.fillScreen(BLACK);
  
  //hora
  dibujarHora();

  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(50, 50);
  tft.print("Bienvenidos!");
  
  // Botón Registro
  tft.fillRect(BTN_REG_X, BTN_REG_Y, BTN_REG_W, BTN_REG_H, BLUE);
  tft.setTextSize(2);
  tft.setCursor(BTN_REG_X + 10, BTN_REG_Y + 15);
  tft.print("REGISTRO");
  
  // Botón Entrar
  tft.fillRect(BTN_ENT_X, BTN_ENT_Y, BTN_ENT_W, BTN_ENT_H, GREEN);
  tft.setCursor(BTN_ENT_X + 15, BTN_ENT_Y + 15);
  tft.print("ENTRAR");
}


void pantallaPeso(){



pantalla = 2;

tft.fillScreen(BLACK);

dibujarBotonHome();



tft.setTextColor(WHITE);

tft.setTextSize(3);

tft.setCursor(90,20);

tft.print("TU PESO");



tft.fillRect(120,60,80,35,GREEN);

tft.setTextSize(2);

tft.setCursor(145,70);

tft.print("OK");



tft.fillRect(40,130,60,50,RED);

tft.setTextSize(4);

tft.setCursor(60,145);

tft.print("-");



tft.fillRect(220,130,60,50,GREEN);

tft.setCursor(240,145);

tft.print("+");



tft.setCursor(115,140);

tft.print(peso);

tft.print("kg");

}



void pantallaMeta(){



pantalla = 3;

tft.fillScreen(BLACK);

dibujarBotonHome();



litrosMeta = peso * 0.035;



tft.setTextColor(WHITE);

tft.setTextSize(2);



tft.setCursor(80,20);

tft.print("META DIARIA");



tft.setCursor(100,60);

tft.print(litrosMeta,2);

tft.print(" L");

dibujarVasosMeta();// vasitos

tft.fillRect(110,200,100,30,GREEN);

tft.setCursor(150,208);

tft.print("OK");

}



void pantallaLlenado(){



pantalla = 4;

tft.fillScreen(BLACK);

dibujarBotonHome();



tft.setTextColor(WHITE);

tft.setTextSize(2.5);

tft.setCursor(60,20);

tft.print("LLENANDO");



tft.setTextSize(2);

tft.setCursor(60,60);

tft.print(nombreActual);



tft.setCursor(60,100);

tft.print("0 L");



tft.drawRect(40,135,240,20,WHITE);



tft.fillRect(245,200,70,30,GREEN);

tft.setCursor(262,208);

tft.print("SIG");



volumenActual = 0;

}



void PantallaFin(){

  

pantalla = 5;



tft.fillScreen(BLACK);

dibujarBotonHome();



tft.setTextColor(WHITE);

tft.setTextSize(3);

tft.setCursor(70,20);

tft.print("META DIARIA");



tft.setTextSize(2);

tft.setCursor(70,60);

tft.print(nombreActual);









volumenActual = 0;

}

/* ================= ANIMACION ================= */



void actualizarLlenado(float volumen){



int ancho = (volumen / 0.5) * 240;

if(ancho > 240) ancho = 240;



tft.fillRect(60,100,120,20,BLACK);

tft.setCursor(60,100);

tft.print(volumen , 2);

tft.print(" L");



tft.fillRect(40,135,ancho,20,AGUA);// Animacion del agua 

}



/* ================= TOUCH ================= */

void touchBienvenida(int x, int y) {
  // BOTÓN REGISTRO (Azul)
  // X va de 40 a (40 + 110 = 150)
  // Y va de 120 a (120 + 50 = 170)
  if(x > BTN_REG_X && x < (BTN_REG_X + BTN_REG_W) && 
     y > BTN_REG_Y && y < (BTN_REG_Y + BTN_REG_H)) {
    
    esRegistro = true;
    nombreIndex = 0;      
    nombreActual[0] = '\0';
    dibujarTeclado(); // Pasa a pantalla 1
  }
  
  // BOTÓN ENTRAR (Verde)
  // X va de 170 a (170 + 110 = 280)
  // Y va de 120 a (120 + 50 = 170)
  if(x > BTN_ENT_X && x < (BTN_ENT_X + BTN_ENT_W) && 
     y > BTN_ENT_Y && y < (BTN_ENT_Y + BTN_ENT_H)) {
    
    esRegistro = false;
    nombreIndex = 0;      
    nombreActual[0] = '\0';
    dibujarPantallaEntrar(); // Pasa a pantalla 7
  }
}

void tecladoTouch(int x, int y) {
  if (tocarHome(x, y)) return;

  // Limitar el área de toque solo a las teclas
  if (y < 90 || y > 230) return;

  int f = (y - 90) / 35;
  int c = (x - 10) / 45;

  if (f < 0 || f > 3 || c < 0 || c > 6) return;

  String tecla = teclas[f][c];

  if (tecla == "DEL") {
    if (nombreIndex > 0) {
      nombreIndex--;
      nombreActual[nombreIndex] = '\0';
    }
  } 
  else if (tecla == "OK") {
    if (nombreIndex > 0) {
      prefs.begin(nombreActual, false); 
      bool existe = prefs.isKey("consumo");

      // --- ESCENARIO A: VIENE DE LA PANTALLA DE REGISTRO (1) ---
      if (pantalla == 1) { 
        if (existe) {
          //mostrar error
          tft.fillRect(40, 40, 240, 30, RED);
          tft.setCursor(55, 50);
          tft.setTextColor(WHITE);
          tft.print("YA REGISTRADO");
          // Espera para que el usuario alcance a leer
          delay(1200); 
       

          nombreIndex = 0;
          nombreActual[0] = '\0';
          prefs.end();

         dibujarPantallaEntrar(); 
          return; // Salimos de la función para que no intente redibujar el teclado
        } else {
          volumenTotal = 0;
          prefs.putFloat("consumo", 0.0);
          prefs.end();
          pantallaPeso();
          return; 
        }
      } 
      
      // --- ESCENARIO B: VIENE DE LA PANTALLA DE ENTRAR (7) ---
      else if (pantalla == 7) { 
        if (!existe) {
          // Mostramos el error en rojo
          tft.fillRect(40, 40, 240, 30, RED);
          tft.setCursor(55, 50);
          tft.setTextColor(WHITE);
          tft.print("NO EXISTE");
          
          // Espera para que el usuario alcance a leer
          delay(1200); 
          
          // Limpiar las variables del nombre
          nombreIndex = 0;
          nombreActual[0] = '\0';
          prefs.end();
       
          //  Volver a la pantalla 0 (Bienvenida)
          pantallaBienvenida(); 
          return; // Salimos de la función para que no intente redibujar el teclado
        } else {
          // Si sí existe, flujo normal a confirmar peso...
          volumenTotal = prefs.getFloat("consumo", 0.0);
          float pesoGuardado = prefs.getFloat("peso", 60.0); 
          peso = pesoGuardado;
          prefs.end();
          dibujarConfirmarPeso(pesoGuardado);
          return;
        }
      }
    }
  } 
  else {
    // Escribir letras (Q, W, E, etc.)
    if (nombreIndex < 11) {
      nombreActual[nombreIndex] = tecla[0];
      nombreIndex++;
      nombreActual[nombreIndex] = '\0';
    }
  }

  if (pantalla == 7) dibujarPantallaEntrar();
  else dibujarTeclado();
}
void touchPeso(int x,int y){

if(tocarHome(x,y)) return;



if(x>220 && x<280 && y>130 && y<180){ peso++; pantallaPeso(); }

if(x>40 && x<100 && y>130 && y<180){ if(peso>1)peso--; pantallaPeso(); }

if(x>120 && x<200 && y>60 && y<95){ 
  
  
guardarPeso(peso); // funcion guardar peso 

pantallaMeta(); }


}



void touchMeta(int x,int y){

if(tocarHome(x,y)) return;

if(x>110 && x<210 && y>200 && y<230){ pantallaLlenado(); }

}



void touchLlenado(int x,int y){

if(tocarHome(x,y)) return;

if(x>245 && x<290 && y>200 && y<310){ PantallaFin(); }



}


void touchFinal(int x,int y){

  if(tocarHome(x,y)) return;

}





/* ================= SETUP ================= */



void setup(){



Serial.begin(115200);


//SENSOR ULTRASONICO 
pinMode(TRIG, OUTPUT);

pinMode(ECHO, INPUT);

pinMode(RELE, OUTPUT);


//INICIO DEL RELE
digitalWrite(RELE, LOW);



SPI.begin(18,19,23);



tft.begin();

tft.setRotation(1);



ts.begin();

ts.setRotation(1);



pantallaBienvenida();

configurarTiempo();//llama función de tiempo WIFINTP.INO 

}

/* ================= Cambio de día ================= */

void verificarCambioDia() {
  // Solo se verifica la hora si hay un usuario logueado
  if (strlen(nombreActual) > 0) { 
    if (obtenerHora() == 0) { // Si es medianoche
      prefs.begin(nombreActual, false);
      prefs.putFloat("consumo", 0.0);
      prefs.end();
      volumenTotal = 0;
      Serial.println("Día reiniciado automáticamente");
    }
  }
}

/* ================= LOOP ================= */

void loop(){


  if(ts.touched()){



    TS_Point p = ts.getPoint();



    int x = map(p.x, TS_MAXX, TS_MINX, 0, 320);

    int y = map(p.y, TS_MAXY, TS_MINY, 0, 240);


    if(pantalla==0) touchBienvenida(x,y);
    if(pantalla==1) tecladoTouch(x,y);

    if(pantalla==2) touchPeso(x,y);

    if(pantalla==3) touchMeta(x,y);

    if(pantalla==4) touchLlenado(x,y);

    if(pantalla==5) touchFinal(x,y);
    
    if (pantalla == 6) {
      touchPantallaCambioPeso(x,y);
    }

    if(pantalla == 7) tecladoTouch(x, y);


    Serial.print("Pantalla: "); Serial.print(pantalla);
    Serial.print(" | Touch X: "); Serial.print(x);
    Serial.print(" | Y: "); Serial.println(y);

    

    delay(150);

  }



  if(pantalla == 4){



    float distancia = medirDistancia();



    if(distancia > 0){



      // 🟢 VASO PRESENTE

      if(distancia <= 5){



        if(!llenando){

          tiempoInicio = millis();

          llenando = true;

          volumenActual = 0;

        }



        digitalWrite(RELE, HIGH);



        if(millis() - ultimoUpdate > 200){



          float segundos = (millis() - tiempoInicio) / 1000.0;

          volumenActual = segundos * caudal;



          // 🔥 Mostrar llenado actual

          actualizarLlenado(volumenActual);



          // 🔥 Mostrar TOTAL EN TIEMPO REAL

          float totalEnCurso = volumenTotal + volumenActual;



          tft.fillRect(40,165,240,30,BLACK);

          tft.setCursor(40,165);

          tft.setTextSize(2);

          tft.setTextColor(GREEN);



          tft.print("Total: ");

          tft.print(totalEnCurso , 2);

          tft.print(" L");



          ultimoUpdate = millis();

        }

      }



      // 🔴 VASO RETIRADO

      else{



        if(llenando){



          // 🔥 ACUMULAR DE VERDAD

          volumenTotal += volumenActual;

        // --- Aqui se llama la funcion de void guardar progreso ---
               guardarProgreso();

          tft.fillRect(40,165,240,30,BLACK);

          tft.setCursor(40,165);

          tft.setTextSize(2);

          tft.setTextColor(GREEN);



          tft.print("Total: ");

          tft.print(volumenTotal , 2);

          tft.print(" L");



          llenando = false;

        }



        digitalWrite(RELE, LOW);

      }

    }

  }



if(pantalla == 5){



  float faltaTomar = litrosMeta - volumenTotal;



  // 🔥 Evitar negativos

  if(faltaTomar < 0){

    faltaTomar = 0;

  }





  tft.setTextSize(2);



  tft.setCursor(70,100);

  tft.print("HAS TOMADO:");

  tft.print(volumenTotal, 2);

  tft.print(" L");



  tft.setCursor(70,140);

  tft.print("TE FALTA:");

  tft.print(faltaTomar, 2);

  tft.print(" L");

}
verificarCambioDia();// funcion cambio de día 

// Actualizar la hora cada segundo solo si estamos en la bienvenida
  static unsigned long ultimoReloj = 0;
  if (pantalla == 0 && millis() - ultimoReloj > 1000) {
    dibujarHora();
    ultimoReloj = millis();
  }
  
}

void guardarProgreso() {
  if (strlen(nombreActual) > 0) {
    prefs.begin(nombreActual, false);
    prefs.putFloat("consumo", volumenTotal);
    prefs.end();
    Serial.println("Progreso guardado para: " + String(nombreActual));
  }
}