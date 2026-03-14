#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

/* 
 *  ENVIRONMENT MONITOR & DATA LOGGER (V2.1)
 *  Built for: Panth
 *  UI: Clean Dashboard (No logging counter)
 *  Features: 1-Hour JSON Logging, Multi-View, Serial Export
 */

// --- HARDWARE CONFIG ---
const int BTN_PIN = 3;   
const int LED_PIN = 13;
#define DHTPIN 2         
#define DHTTYPE DHT11    

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- DATA LOGGING CONFIG ---
const int MAX_SAMPLES = 60;        
float tempHistory[MAX_SAMPLES];    
int sampleIndex = 0;               
int totalSamplesFilled = 0;        
unsigned long lastLogTime = 0;
const unsigned long LOG_INTERVAL = 60000; // 1 Minute

// --- SYSTEM STATES ---
enum ViewMode { DASHBOARD, TEMP_FOCUS, HUM_FOCUS, STATS, CREDITS }; 
ViewMode currentView = DASHBOARD;

// --- SENSOR VARIABLES ---
unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 2000;
float t = 0.0, h = 0.0;
float tMin = 99.0, tMax = -99.0;
float hMin = 100.0, hMax = 0.0;
bool sensorError = true;
bool backlightOn = true;

// --- CUSTOM ICONS ---
byte icon_temp[8] = {0b00100, 0b01010, 0b01010, 0b01110, 0b11111, 0b11111, 0b01110, 0b00000}; 
byte icon_drop[8] = {0b00100, 0b00100, 0b01110, 0b11111, 0b11111, 0b11111, 0b01110, 0b00000}; 
byte icon_deg[8]  = {0b01100, 0b10010, 0b10010, 0b01100, 0b00000, 0b00000, 0b00000, 0b00000}; 
byte icon_up[8]   = {0b00100, 0b01110, 0b10101, 0b00100, 0b00100, 0b00100, 0b00000, 0b00000}; 
byte icon_dn[8]   = {0b00000, 0b00000, 0b00100, 0b00100, 0b00100, 0b10101, 0b01110, 0b00100}; 

void setup() {
  Serial.begin(9600); 
  pinMode(BTN_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  
  lcd.createChar(0, icon_temp);
  lcd.createChar(1, icon_drop);
  lcd.createChar(2, icon_deg);
  lcd.createChar(3, icon_up);
  lcd.createChar(4, icon_dn);
  
  dht.begin();
  
  lcd.setCursor(0, 0); lcd.print(" PANTH OS V2.1 ");
  lcd.setCursor(0, 1); lcd.print(" SYSTEM ONLINE ");
  delay(2000); 
  lcd.clear();
}

void loop() {
  handleSensor();   
  handleLogging();  
  handleButton();   
  handleLED();      
  updateDisplay();  
}

// --- SENSOR LOGIC ---
void handleSensor() {
  if (millis() - lastReadTime >= READ_INTERVAL) {
    lastReadTime = millis();
    float newH = dht.readHumidity();
    float newT = dht.readTemperature();
    
    if (isnan(newH) || isnan(newT)) {
      sensorError = true;
    } else {
      sensorError = false;
      t = newT; h = newH;
      if (t < tMin) tMin = t; if (t > tMax) tMax = t;
      if (h < hMin) hMin = h; if (h > hMax) hMax = h;
    }
  }
}

// --- DATA LOGGING ENGINE ---
void handleLogging() {
  if (millis() - lastLogTime >= LOG_INTERVAL) {
    lastLogTime = millis();
    if (!sensorError) {
      tempHistory[sampleIndex] = t;
      sampleIndex = (sampleIndex + 1) % MAX_SAMPLES;
      if (totalSamplesFilled < MAX_SAMPLES) totalSamplesFilled++;
    }
  }
}

// --- JSON EXPORT ENGINE ---
void exportDataAsJSON() {
  lcd.clear();
  lcd.setCursor(0,0); lcd.print(">> EXPORTING <<");
  lcd.setCursor(0,1); lcd.print(" CHECK LAPTOP  ");
  
  Serial.println("\n--- DATA START ---");
  Serial.print("{\"owner\": \"Panth\", \"unit\": \"Celsius\", \"count\": ");
  Serial.print(totalSamplesFilled);
  Serial.print(", \"data\": [");
  
  for (int i = 0; i < totalSamplesFilled; i++) {
    int idx = (sampleIndex - totalSamplesFilled + i + MAX_SAMPLES) % MAX_SAMPLES;
    Serial.print(tempHistory[idx], 1);
    if (i < totalSamplesFilled - 1) Serial.print(",");
  }
  
  Serial.println("]}");
  Serial.println("--- DATA END ---");
  
  delay(2000);
  lcd.clear();
}

// --- BUTTON ENGINE ---
void handleButton() {
  static unsigned long lastRel = 0;
  static int clicks = 0;
  static bool waiting = false;
  static bool lastS = LOW;
  
  bool p = digitalRead(BTN_PIN);
  unsigned long now = millis();

  if (p != lastS) {
    if (p) waiting = true; 
    else { clicks++; lastRel = now; }
    lastS = p; delay(25); 
  }
  
  if (waiting && (now - lastRel > 350) && !p) {
    if (clicks == 1) { 
      currentView = static_cast<ViewMode>((currentView + 1) % 5);
      lcd.clear();
    } 
    else if (clicks == 2) { 
      backlightOn = !backlightOn;
      if (backlightOn) lcd.backlight(); else lcd.noBacklight();
    }
    else if (clicks == 3) { 
      currentView = CREDITS;
      lcd.clear();
    }
    else if (clicks == 4) { 
      exportDataAsJSON();
      currentView = DASHBOARD;
    }
    clicks = 0; waiting = false;
  }
}

// --- LED LOGIC ---
void handleLED() {
  unsigned long t_ms = millis();
  if (sensorError) {
    digitalWrite(LED_PIN, (t_ms / 150) % 2); 
  } else {
    unsigned long cycle = t_ms % 2000;
    if (cycle < 40 || (cycle > 140 && cycle < 180)) digitalWrite(LED_PIN, HIGH);
    else digitalWrite(LED_PIN, LOW);
  }
}

// --- DISPLAY RENDERER ---
void updateDisplay() {
  static unsigned long lastUIUpdate = 0;
  if (millis() - lastUIUpdate > 300) {
    lastUIUpdate = millis();

    if (sensorError && t == 0.0) {
      lcd.setCursor(0, 0); lcd.print(" SENSOR OFFLINE ");
      lcd.setCursor(0, 1); lcd.print(" CHECK PIN 2    ");
      return;
    }

    switch (currentView) {
      case DASHBOARD:
        lcd.setCursor(0, 0);
        lcd.write(0); lcd.print(" TEMP: "); lcd.print(t, 1); 
        lcd.write(2); lcd.print("C    "); 
        lcd.setCursor(0, 1);
        lcd.write(1); lcd.print(" HUMID: "); lcd.print(h, 1); 
        lcd.print(" %     ");
        break;
        
      case TEMP_FOCUS:
        lcd.setCursor(0, 0); lcd.print("  TEMPERATURE   ");
        lcd.setCursor(0, 1); lcd.print("     "); lcd.print(t, 1); lcd.write(2); lcd.print("C    ");
        break;
        
      case HUM_FOCUS:
        lcd.setCursor(0, 0); lcd.print("    HUMIDITY    ");
        lcd.setCursor(0, 1); lcd.print("     "); lcd.print(h, 1); lcd.print(" %     ");
        break;
        
      case STATS:
        lcd.setCursor(0, 0);
        lcd.print("T "); lcd.write(3); lcd.print((int)tMax); lcd.print(" "); lcd.write(4); lcd.print((int)tMin);
        lcd.print("  4-CLK"); 
        lcd.setCursor(0, 1);
        lcd.print("H "); lcd.write(3); lcd.print((int)hMax); lcd.print(" "); lcd.write(4); lcd.print((int)hMin);
        lcd.print(" EXPORT"); 
        break;
        
      case CREDITS:
        lcd.setCursor(0, 0); lcd.print("  SYSTEM BUILT  ");
        lcd.setCursor(0, 1);
        if ((millis() / 500) % 2 == 0) lcd.print(" >> BY PANTH << ");
        else lcd.print(" -- BY PANTH -- ");
        break;
    }
  }
}
