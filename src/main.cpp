/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-class.rst
 *       https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-examples.rst
 *       https://github.com/esp8266/Arduino/issues/504
 *  Works with: station_bare_01.ino
 */ 


#include <ESP8266WiFi.h>

WiFiServer server(80);
IPAddress IP(192,168,4,15);
IPAddress mask(255, 255, 255, 0);

byte ledPin = 2;


void wait_and_answer_to_pc(String wait_str, String answer){
  if (Serial.available() > 0){
    String str = Serial.readString();

    if (str == wait_str){
      Serial.println(answer);
    }
  }
}


void forwarding_sensors_to_pc(String str){
  if (str.length() > 1){
    //if (str[0] == 'r' or str[0] == 'l'){
    //  if (isDigit(str[1])){
        Serial.println(str);
    //  }
    //}
  }
}


void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Game-Event", "Event-Game");
  WiFi.softAPConfig(IP, IP, mask);
  server.begin();
  pinMode(ledPin, OUTPUT);
  Serial.println();
  Serial.println("accesspoint_bare_01.ino");
  Serial.println("Server started.");
  Serial.print("IP: ");     Serial.println(WiFi.softAPIP());
  Serial.print("MAC:");     Serial.println(WiFi.softAPmacAddress());
}

void loop() {
  wait_and_answer_to_pc("Game-Event\r", "Event-Game-base-1.0");

  WiFiClient client = server.available();
  
  if (!client) {
    return;
  }

  digitalWrite(ledPin, LOW);
  
  String request = client.readStringUntil('\r');
  
  forwarding_sensors_to_pc(request);
 
  client.flush();
  
  digitalWrite(ledPin, HIGH);
}