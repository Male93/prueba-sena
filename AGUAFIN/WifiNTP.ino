#include <WiFi.h>
#include "time.h"

const char* ssid     = "Redmi Note 13 Pro";
const char* password = "123456789";
const char* ntpServer = "co.pool.ntp.org"; // Servidor de Colombia

void configurarTiempo() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  
  // Configura la hora para Bogotá (-5 GMT)
  configTime(-18000, 0, ntpServer); 
}

int obtenerHora() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) return -1;
  return timeinfo.tm_hour; // Devuelve la hora (0-23)
}