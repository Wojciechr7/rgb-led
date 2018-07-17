#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <SocketIoClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;



#define czerwona 13
#define zielona 12
#define niebieska 14

void changeColor(const char * payload, size_t length) {

  String s = String((char*)payload);

  if (s == "red") {
    analogWrite(czerwona, 500);
    digitalWrite(zielona, LOW);
    digitalWrite(niebieska, LOW);
    
  }
  if (s == "green") {
    digitalWrite(czerwona, LOW);
    digitalWrite(zielona, HIGH);
    digitalWrite(niebieska, LOW);
    
  }
  if (s == "blue") {
    digitalWrite(czerwona, LOW);
    digitalWrite(zielona, LOW);
    digitalWrite(niebieska, HIGH);
    
  }
  
  
  USE_SERIAL.printf("%s\n", payload);

}

void changeAnalog(const char * payload, size_t length) {
  DynamicJsonBuffer jsonBuffer(length);
  JsonArray& root = jsonBuffer.parseArray(payload);

    analogWrite(czerwona, root[0]);
    analogWrite(zielona, root[1]);
    analogWrite(niebieska, root[2]);

  USE_SERIAL.printf("%s\n", root[0]);

}
 
void setup() {
  pinMode(czerwona, OUTPUT); // Piny, podłączone do diody jako wyjścia
  pinMode(zielona, OUTPUT);
  pinMode(niebieska, OUTPUT);
  
    USE_SERIAL.begin(115200);

    WiFi.begin("Hemoglobina", "zxasqw12");

    while (WiFi.status() != WL_CONNECTED) {

    delay(1000);
    Serial.print("Connecting..");
    }

    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    WiFiMulti.addAP("ssid", "pass");

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }




    webSocket.on("color", changeColor);
    webSocket.on("analog", changeAnalog);
    webSocket.begin("192.168.0.102", 3000);
}

 
 
void loop() {
webSocket.loop();
}

