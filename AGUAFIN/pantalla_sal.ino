void pantallaSal() {
  pantalla = 8; // Asegúrate de definir SAL como 8 o usar el número directamente
  tft.fillScreen(BLACK);
  dibujarBotonHome();

  // TÍTULO (Tamaño 2 para que quepa en una línea)
  tft.setTextColor(AGUA);
  tft.setTextSize(2);
  tft.setCursor(15, 20);
  tft.print("PROCESO DE MINERALIZACION");

  // PRIMER PÁRRAFO (Tamaño 1 para texto largo o 2 con saltos manuales)
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  
  tft.setCursor(10, 50);
  tft.print("Anadir una pizca de sal");
  tft.setCursor(10, 70);
  tft.print("marina restaura los");
  tft.setCursor(10, 90);
  tft.print("electrolitos esenciales.");

  // SEGUNDO PÁRRAFO (Motivación)
  tft.setCursor(10, 130);
  tft.print("Tu cuerpo absorbera el");
  tft.setCursor(10, 150);
  tft.print("agua de forma efectiva.");

  // INVITACIÓN FINAL (En color Verde para que resalte)
  tft.setTextColor(GREEN);
  tft.setCursor(10, 190);
  tft.print("¡Usa la sal que tienes");
  tft.setCursor(10, 210);
  tft.print("aqui al lado! =)");
}
