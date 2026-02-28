#include <BluetoothSerial.h>

BluetoothSerial ESP_BT;

const int ledPins[8] = {32, 33, 25, 26, 27, 14, 12, 13};
const int LED_COUNT = 8;
const int TEAM_NUMBER = 223; // Numărul echipei

// Funcție pentru calculul șirului lui Fibonacci
int fibonacci(int n) {
  if (n <= 1)
    return n;
  else
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  ESP_BT.begin("Echipa 223"); // Setează numele dispozitivului pentru Bluetooth
  
  Serial.println("Așteptare conexiune Bluetooth...");
}

void displayOnLEDs(int number, int duration) {
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(ledPins[i], (number >> (7 - i)) & 1);
  }
  delay(duration); // Așteaptă pentru durata specificată
  // Oprirea LED-urilor
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  if (ESP_BT.available()) {
    // Citirea lui X și Y
    int X = ESP_BT.parseInt(); // Citirea lui X
    int Y = ESP_BT.parseInt(); // Citirea lui Y

    // Afișare textul "OK NNN" pe Bluetooth
    ESP_BT.print("OK ");
    ESP_BT.println(TEAM_NUMBER);
    
    // Afișare numere X și Y pe terminalul serial
    Serial.print("Echipa=");
    Serial.print(TEAM_NUMBER);
    Serial.print("  X=");
    Serial.print(X);
    Serial.print(", Y=");
    Serial.println(Y);

    // Afișare pe LED-uri
    displayOnLEDs(X, (TEAM_NUMBER / 15) % 3 + 1 * 1000); // Durată în milisecunde
    delay(500); // Pauză de 0.5 secunde
    
    // Afișare numărul Y pe LED-uri
    displayOnLEDs(Y, (TEAM_NUMBER / 15) % 3 + 1 * 1000); // Durată în milisecunde
    delay(500); // Pauză de 0.5 secunde
    
    // Calculul și afișarea al (X+Y)-lea element al șirului lui Fibonacci
    int ZZZ = fibonacci(X + Y);
    Serial.print("ZZZ=");
    Serial.println(ZZZ);
    
    ESP_BT.print("ZZZ=");
    ESP_BT.println(ZZZ);
    
    // Așteptare pentru o nouă operație
    Serial.println("Doriți o nouă operație?  da/NU");
    while(true){
      if(Serial.available()){
        String response = Serial.readString();
        if(response == "da"){
          break;
        } else {
          for (int k = 0; k < 3; k++) {
            for (int i = 0; i < LED_COUNT; i++) {
              digitalWrite(ledPins[i], (TEAM_NUMBER >> (7 - i)) & 1);
            }
            delay(500);
            for (int i = 0; i < LED_COUNT; i++) {
              digitalWrite(ledPins[i], LOW);
            }
            delay(500);
          }
          return;
        }
      }
    }
  }
}
