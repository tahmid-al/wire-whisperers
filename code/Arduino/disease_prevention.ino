#include <DHT.h>
#include <SoftwareSerial.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <NewPing.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define PIRPIN 3
#define TRIG_PIN 6
#define ECHO_PIN 7
#define MAX_DISTANCE 200

char auth[] = "Your_Blynk_Auth_Token";
char ssid[] = "Your_WiFi_SSID";
char pass[] = "Your_WiFi_Password";

SoftwareSerial EspSerial(10, 11); // RX, TX
#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial);

DHT dht(DHTPIN, DHTTYPE);
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

BlynkTimer timer;

void sendSensor() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  bool motion = digitalRead(PIRPIN);
  int distance = sonar.ping_cm();

  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V3, motion);
  Blynk.virtualWrite(V4, distance);
}

void setup() {
  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  Blynk.begin(auth, wifi, ssid, pass);

  dht.begin();
  pinMode(PIRPIN, INPUT);
  timer.setInterval(2000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
