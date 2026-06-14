#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// Настройки Wi-Fi
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

ESP8266WebServer server(80);
Servo finger1, finger2, finger3, finger4, finger5;

// Пины сервоприводов и углы (0 — открыто, 150 — сжато)
const int servoPins[5] = {D1, D2, D3, D5, D6}; 
const int ANGLE_OPEN = 0;    
const int ANGLE_CLOSE = 150; 

String currentStatus = "NONE";

void handleRoot() {
  // Проверяем параметр ?status= в URL
  if (server.hasArg("status")) {
    String statusArg = server.arg("status");
    
    if (statusArg == "knopka_on") {
      currentStatus = "ON";
      // Сжимаем руку (тянем тросы)
      finger1.write(ANGLE_CLOSE); finger2.write(ANGLE_CLOSE);
      finger3.write(ANGLE_CLOSE); finger4.write(ANGLE_CLOSE); finger5.write(ANGLE_CLOSE);
    } 
    else if (statusArg == "knopka_off") {
      currentStatus = "OFF";
      // Расслабляем тросы (разгибание за счет пружин)
      finger1.write(ANGLE_OPEN); finger2.write(ANGLE_OPEN);
      finger3.write(ANGLE_OPEN); finger4.write(ANGLE_OPEN); finger5.write(ANGLE_OPEN);
    }
  }
  
  // Отправляем в браузер простой текст с текущим статусом
  server.send(200, "text/plain", "STATUS: " + currentStatus);
}

void setup() {
  Serial.begin(115200);
  
  // Инициализация сервоприводов
  finger1.attach(servoPins[0]); finger2.attach(servoPins[1]);
  finger3.attach(servoPins[2]); finger4.attach(servoPins[3]); finger5.attach(servoPins[4]);
  
  // Стартовое положение — рука открыта
  finger1.write(ANGLE_OPEN); finger2.write(ANGLE_OPEN);
  finger3.write(ANGLE_OPEN); finger4.write(ANGLE_OPEN); finger5.write(ANGLE_OPEN);

  // Подключение к сети
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  
  Serial.print("Адрес для управления: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}