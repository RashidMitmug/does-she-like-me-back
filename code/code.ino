#include <Servo.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address of Ethernet shield
IPAddress ip(192, 168, 1, 177); // IP address of Arduino
EthernetServer server(80); // Port to listen on

// Define the pins for the soil moisture sensor and the servo motor
const int moistureSensorPin = A0;
const int servoPin = 9;

// Define the threshold moisture level for watering
const int moistureThreshold = 500;

Servo servoMotor;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  servoMotor.attach(servoPin);
  servoMotor.write(0); // Set servo to closed position initially
}

void loop() {
  // Handle incoming HTTP request
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    String interval = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/plain");
          client.println("Connection: close");
          client.println();
          client.println("Interval set successfully.");
          // Here, you can read the interval value from the request and set it accordingly
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
          interval += c;
        }
      }
    }
    delay(1);
    client.stop();
  }
  
  // Read moisture level from sensor
  int moistureLevel = analogRead(moistureSensorPin);
  
  // Parse watering interval string
  int seconds, minutes, hours, days;
  sscanf(interval.c_str(), "%d,%d,%d,%d", &seconds, &minutes, &hours, &days);
  
  // Calculate watering interval in milliseconds
  long wateringInterval = ((days * 24L * 60L * 60L) + (hours * 60L * 60L) + (minutes * 60L) + seconds) * 1000L;
  
  // Check if soil is dry enough to water
  if (moistureLevel < moistureThreshold) {
    Serial.println("Soil is dry. Watering plant...");
    waterPlant();
    delay(5000); // Delay for watering
  } else {
    Serial.println("Soil moisture level is sufficient. No watering needed.");
  }
  
  // Delay before next iteration
  delay(wateringInterval);
}

void waterPlant() {
  // Open the servo to release water
  servoMotor.write(90);
  delay(1000); // Delay to allow water to flow
  // Close the servo after watering
  servoMotor.write(0);
}
