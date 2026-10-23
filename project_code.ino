#define BLYNK_TEMPLATE_ID "ID"
#define BLYNK_TEMPLATE_NAME "Flame and gas detection"
#define BLYNK_AUTH_TOKEN "Token"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "12345678";          // Replace with your WiFi Name
char pass[] = "12345678";          // Replace with your WiFi Password

#define MQ2_DOUT_PIN 32    
#define FLAME_PIN 35       
#define BUZZER_PIN 25      
#define LED_PIN 26         // Pin 26 controls your external LED

BlynkTimer timer;

// State flags
bool gasAlertSent = false;
bool fireAlertSent = false;

// Alarm & Mobile tracking variables
bool alarmActive = false;
int alarmDurationCounter = 0; // Tracks how long the alarm has been beeping
bool mobileMute = false;      // Tracks if you turned off the alarm from the app
bool buzzerState = false;     // Toggles for the blinking/beeping effect

// Blynk App Sync: This function runs automatically when you press the button on Virtual Pin V1
BLYNK_WRITE(V1) {
  int buttonState = param.asInt(); // Reads the button state (1 = Pressed/ON, 0 = OFF)
  if (buttonState == 1) {
    mobileMute = true;  // User wants to mute the alarm
    digitalWrite(BUZZER_PIN, LOW); // Instantly turn off hardware
    digitalWrite(LED_PIN, LOW);
    Serial.println("📱 Alarm muted via Mobile App!");
  } else {
    mobileMute = false; // Reset mute restriction when button toggled back
  }
}

void checkSensors() {
  int flameValue = digitalRead(FLAME_PIN); 
  int gasValue = digitalRead(MQ2_DOUT_PIN); 
  
  bool currentDanger = false;

  // --- GAS DETECTION ---
  Serial.print("Gas Value: ");
  if (gasValue == HIGH) {    
    Serial.println("Gas Detected");
    currentDanger = true;
    if (!gasAlertSent) { 
      // CRITICAL FIX: Ensure the event code matches exactly what is in your Blynk Console
      Blynk.logEvent("gas_alert", "⚠️ Gas Detected!");
      gasAlertSent = true; 
    }
  } else {
    Serial.println("No Gas Detected");
    gasAlertSent = false; 
  }

  // --- FIRE DETECTION ---
  Serial.print("Flame Sensor: ");
  if (flameValue == LOW) { 
    Serial.println("Flame Detected");
    currentDanger = true;
    if (!fireAlertSent) {
      // CRITICAL FIX: Ensure the event code matches exactly what is in your Blynk Console
      Blynk.logEvent("fire_alert", "🔥 Fire Detected!");
      fireAlertSent = true; 
    }
  } else {
    Serial.println("No Flame");
    fireAlertSent = false; 
  }

  // --- ALARM LOGIC (1-Minute Beeping + Mute Control) ---
  if (currentDanger) {
    if (!alarmActive) {
      alarmActive = true;
      alarmDurationCounter = 0; 
    }

    if (alarmDurationCounter < 30 && !mobileMute) {
      buzzerState = !buzzerState; 
      
      // FIXED: Active Buzzer and LED now both cycle cleanly together
      digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
      digitalWrite(LED_PIN, buzzerState ? HIGH : LOW); 
      
      alarmDurationCounter++; 
      Serial.print("🚨 ALARM ACTIVE! Time elapsed: ");
      Serial.print(alarmDurationCounter * 2);
      Serial.println(" seconds.");
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      if (mobileMute) {
        Serial.println("🔇 Alarm running but silenced by phone.");
      } else {
        Serial.println("⏱️ 1-Minute Alarm timeout reached. Silenced.");
      }
    }
  } else {
    alarmActive = false;
    alarmDurationCounter = 0;
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    
    if (mobileMute) {
      mobileMute = false;
      Blynk.virtualWrite(V1, 0); 
    }
  }
  
  Serial.println("-----------------------");
}

void setup() {
  Serial.begin(115200);

  pinMode(MQ2_DOUT_PIN, INPUT);  
  pinMode(FLAME_PIN, INPUT);     
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // FIXED: Using the standard parameter structure for Blynk connection stability
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, checkSensors);
}

void loop() {
  Blynk.run();    
  timer.run();    
}
