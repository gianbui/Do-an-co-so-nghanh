#include "wifiConfig.h"
#define BLYNK_TEMPLATE_ID "TMPL6X1jXpNRh"
#define BLYNK_TEMPLATE_NAME "Doancs"
#define BLYNK_AUTH_TOKEN "DZ9Rbk9G9QTIWdanOijh17FoZxmsGe83"

#include <Wire.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h> 
#include <HTTPClient.h>

#define DHTTYPE DHT11
#define DHTPIN 26    
#define BUZZER_PIN 18
#define quat 17 //bao do am
#define led 16   // dieu khien bylink
#define antoan 5  // led bao an toan
#define MQ2_ANALOG_PIN 34
#define SERVO_PIN 13  
int lastTS=0;

String apiKey = "F96IDREWWWHBQ2CU";  
const char* server = "http://api.thingspeak.com/update";

void sendToThingSpeak(float t, float h, int gasAnalog) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Field1 = nhiệt độ, Field2 = độ ẩm, Field3 = khí gas
    String url = String(server) + "?api_key=" + apiKey +
                 "&field1=" + String(t) +
                 "&field2=" + String(h) +
                 "&field3=" + String(gasAnalog);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.print("ThingSpeak HTTP: ");
      Serial.print(httpCode);
      Serial.print(" | Response: ");
      Serial.println(payload);
    } else {
      Serial.print("Loi gui ThingSpeak: ");
      Serial.println(http.errorToString(httpCode));
    }

    http.end();
  } else {
    Serial.println("WiFi chua ket noi, khong gui ThingSpeak");
  }
}


DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool blynkConnect = 0;

Servo servo;
int servoState = 0;
bool servoManual = false;  
unsigned long lastSensorCheck = 0; 
bool sensorError = false;  
int lastGasValue = -1;
float lastTemp = -1000, lastHumi = -1000;

BLYNK_WRITE(V4) {
  int ledState = param.asInt(); 
  digitalWrite(led, ledState);
  Serial.print("Trạng thái LED từ Blynk: ");
  Serial.println(ledState);
}

BLYNK_WRITE(V5) {
  int servoCmd = param.asInt(); 
  servoManual = true; 
  if (servoCmd == 1) {
    servo.write(90); 
    servoState = 1;
    Serial.println("Servo mở");
  } else {
    servo.write(0); 
    servoState = 0;
    Serial.println("Servo đóng");
  }
}

void setup() {
  Serial.begin(9600);
  wifiConfig.begin();

  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Welcome nhom 3");
  Serial.println("Welcome nhom 3");
  delay(2000);
  lcd.clear();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(quat, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(antoan, OUTPUT);
  pinMode(MQ2_ANALOG_PIN, INPUT);

  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);

  servo.attach(SERVO_PIN);
  servo.write(0); 
}

void loop() {
  wifiConfig.run();

  if (WiFi.status() == WL_CONNECTED) {
    if (blynkConnect == 0) {
      Serial.println("Connecting to blynk cloud...!");
      if (Blynk.connect(5000)) {
        Serial.println("Connected to blynk cloud!");
        blynkConnect = 1;
      } else {
        Serial.println("Connection failed. Try again later.");
      }
    }
    if (!Blynk.connected()) blynkConnect = 0;
    Blynk.run();
  }

  
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasAnalog = analogRead(MQ2_ANALOG_PIN);

  Blynk.virtualWrite(V3, gasAnalog); 

  if (isnan(h) || isnan(t) || gasAnalog <10 || gasAnalog > 4095) {
    sensorError = true;
    Serial.println("Co Loi SENSOR !!");
  }

  if (sensorError) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LOI SENSOR");
    lcd.setCursor(0, 1);
    lcd.print("Kiem tra DHT/MQ2");
    Blynk.virtualWrite(V2, 1);

    
    for (int i = 0; i < 6; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(150);
      digitalWrite(BUZZER_PIN, LOW);
      delay(100);
    }
    return;
  }

  if (!isnan(h) && !isnan(t)) {
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);

    if (t >= 42 || gasAnalog >= 1000) { 
      // digitalWrite(BUZZER_PIN, 1);
      
      digitalWrite(antoan, 0);
      digitalWrite(quat, 0);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("!!! CANH BAO !!!");
      lcd.setCursor(0, 1);
      lcd.print("CO KHI GAS/KHOI");
      Serial.println("nhiet do >42 hoac gas >1000");
      Blynk.virtualWrite(V2, 1);

      servo.write(180);
      servoState = 1;
      servoManual = false; 
      
      for (int i = 0; i < 10; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(150);
      digitalWrite(BUZZER_PIN, LOW);
      delay(50);
    }
    } 
    else {
      if (h < 60) {
        digitalWrite(quat, 1);
        digitalWrite(antoan, 0);
        Blynk.virtualWrite(V6, 1);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("T:"); lcd.print(t); lcd.print("C ");
        lcd.print("Gas:");lcd.print(gasAnalog);
        lcd.setCursor(0, 1);
        lcd.print("H:"); lcd.print(h); lcd.print("% ");lcd.print("cap am");
      } else {
        digitalWrite(quat, 0);
        digitalWrite(antoan, 1);
        Blynk.virtualWrite(V6, 0);

        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("T:"); lcd.print(t); lcd.print("C ");
        lcd.print("Gas:");lcd.print(gasAnalog);
        lcd.setCursor(0, 1);
        lcd.print("H:"); lcd.print(h); lcd.print("% ");lcd.print("an toan");

       
      }

      Blynk.virtualWrite(V2, 0);
      digitalWrite(BUZZER_PIN, 0);

      if (servoState == 1 && !servoManual) {
         servo.write(0);
         servoState = 0;
      }
    }
    delay(2000);
  }

        Serial.print("Nhiet do: "); Serial.print(t);
        Serial.print(" C, Do am: "); Serial.print(h);
        Serial.print(" %, Gas ADC: ");
        Serial.println(gasAnalog);

        if (millis() - lastTS >= 15000) {
        sendToThingSpeak(t, h, gasAnalog);
         lastTS = millis();};
  
}
