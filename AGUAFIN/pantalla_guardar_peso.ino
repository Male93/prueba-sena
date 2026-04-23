void guardarPeso(float pesoAhorrar) {
  prefs.begin(nombreActual, false);
  prefs.putFloat("peso", pesoAhorrar); // Guardamos el valor bajo la clave "peso"
  prefs.end();
  Serial.print("Peso guardado: ");
  Serial.println(pesoAhorrar);
}


void touchPantallaCambioPeso(int x, int y) {
  // --- BOTÓN VERDE: "IGUAL" (Lado izquierdo) ---
  if (x > 20 && x < 150 && y > 190 && y < 235) {
    Serial.println("Peso confirmado. Yendo a llenado...");
    
    // recalcular la meta antes de ir
       litrosMeta = peso * 0.035; 

    pantallaLlenado(); // 💧 Te envía directo a servir agua
  }

  // --- BOTÓN ROJO: "CAMBIAR" (Lado derecho) ---
  if (x > 170 && x < 300 && y > 190 && y < 235) {
    Serial.println("Cambiando peso...");
    pantallaPeso();    // ⚖️ Te envía a la pantalla de ajuste (+ / -)
  }
}

// dibujar pantalla

void dibujarConfirmarPeso(float p) {
  pantalla = 6; 
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  
  tft.setCursor(20, 50);
  tft.print("Hola "); tft.print(nombreActual);
  
  tft.setCursor(20, 80);
  tft.print("Tu peso actual registrado:");
  
  tft.setTextSize(4);
  tft.setCursor(80, 120);
  tft.print(p, 1); tft.print(" kg");

  // Botones: SI (Verde) / NO (Rojo)
  tft.fillRect(20, 190, 130, 45, 0x07E0);  // Verde
  tft.fillRect(170, 190, 130, 45, 0xF800); // Rojo
  
  tft.setTextSize(2);
  tft.setCursor(60, 205); tft.print("IGUAL");
  tft.setCursor(195, 205); tft.print("CAMBIAR");
}