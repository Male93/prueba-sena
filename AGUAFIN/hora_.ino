void {dibujarHora() 
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return; // Si no hay internet/hora, no dibuja nada
  }

  // hora (ejemplo: 12:45)
  char horaTexto[10];
  sprintf(horaTexto, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);

  // fondo negro 
  tft.fillRect(98, 195, 130, 35, BLACK); 
  
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(118, 200);
  tft.print(horaTexto);
}

//icono vaso
void dibujarIconoVaso(int x, int y, uint16_t color) {
  // Cuerpo del vaso
  tft.drawRect(x, y, 20, 25, WHITE); // Contorno
  tft.fillRect(x + 2, y + 5, 16, 18, color); // Contenido (agua)
  // Brillo del cristal
  tft.drawLine(x + 4, y + 7, x + 4, y + 12, WHITE);
}
// vasos meta
void dibujarVasosMeta() {
  int numVasos = litrosMeta / 0.25; // Cada vaso son 250ml
  if (numVasos > 15) numVasos = 15; // Limitamos para que no se salga de la pantalla

  int startX = 40;
  int startY = 100;
  int vasosPorFila = 5;

  for (int i = 0; i < numVasos; i++) {
    int fila = i / vasosPorFila;
    int columna = i % vasosPorFila;

    int x = startX + (columna * 50); // Espaciado horizontal
    int y = startY + (fila * 40);   // Espaciado vertical

    dibujarIconoVaso(x, y, AGUA);
  }
}