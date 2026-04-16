/*
 * esp32_thingspeak.ino
 *
 * T -> Sıcaklık
 * H -> Nem
 * P -> Basınç
 * L -> Işık seviyesi
 * G -> Hava kalitesi (MQ135)
 * 
 * Author: MSI
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"

HardwareSerial NanoPort(2); // UART2
String line = ""; // uart verisi -> satir

// congif.h
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASS;
const char* apiKey = THINGSPEAK_KEY;

// WiFi baglantisi koparsa diye
void ensureWifi(){
  if (WiFi.status() == WL_CONNECTED) return; // bagli

  Serial.println("WiFi yeniden baglaniyor...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);

  int sayac = 0;
  while (WiFi.status() != WL_CONNECTED && sayac < 20){// yaklasik 10s bekle
    delay(500);
    Serial.print(".");
    sayac++;
  }
  Serial.println();

  // baglanti durumu
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("WiFi tekrar baglandi");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi hala yok");
  }
}

// HTTP GET istegi: veri atma
void sendToThingSpeak(int t, int h, int p, int l, int g){
  ensureWifi();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi yok");
    return;
  }

  HTTPClient http;

  String url = "http://api.thingspeak.com/update?api_key=";
  url += apiKey;
  url += "&field1=" + String(t); // temp.
  url += "&field2=" + String(h); // hum.
  url += "&field3=" + String(p); // press.
  url += "&field4=" + String(l); // light
  url += "&field5=" + String(g); // air-qua.

  http.begin(url);
  // HHTP GET istegi gonderme
  int code = http.GET();

  Serial.print("HTTP kod: "); // 200: bagli, 0: baglanti yok, -1: WiFi yok
  Serial.println(code);

  http.end();
}

// string'i ayristirma
void parseAndSend(String s){
  int t = 0, h = 0, p = 0, l = 0, g = 0;

  int tPos = s.indexOf("T:");
  int hPos = s.indexOf(",H:");
  int pPos = s.indexOf(",P:");
  int lPos = s.indexOf(",L:");
  int gPos = s.indexOf(",G:");

  if (tPos == -1 || hPos == -1 || pPos == -1 || lPos == -1 || gPos == -1) return; // eksik veri kontrolu

  t = s.substring(tPos + 2, hPos).toInt();
  h = s.substring(hPos + 3, pPos).toInt();
  p = s.substring(pPos + 3, lPos).toInt();
  l = s.substring(lPos + 3, gPos).toInt();
  g = s.substring(gPos + 3).toInt();

  Serial.println("Veri alindi:");
  Serial.println(s);

  sendToThingSpeak(t, h, p, l, g);
}

void setup() {
  Serial.begin(115200); // seri port icin
  NanoPort.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("WiFi baglaniyor...");
  WiFi.begin(ssid, password); // wigi baglantisi baslatma

  while (WiFi.status() != WL_CONNECTED){ // baglanana kadar bekle
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi baglandi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // veri geldikce oku
  while (NanoPort.available()){
    char c = NanoPort.read();

    if (c == '\n'){
      
      line.trim();
      if (line.length() > 0){
        parseAndSend(line);
      }
      line = "";
      
    } else if (c != '\r'){
      line += c;
    }
    
  }
}
