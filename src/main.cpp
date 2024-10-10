#include <Arduino.h>

#define GREEN 2
#define RED 4
#define BUZZER 7

#define COUNT_OF_CHARS 36

#define DOT_DURATION 200
#define DASH_DURATION 600
#define SPACE_DURATION 1400

String morseCode[COUNT_OF_CHARS][2] = {
  {"A", ".-"},
  {"B", "-..."},
  {"C", "-.-."},
  {"D", "-.."},
  {"E", "."},
  {"F", "..-."},
  {"G", "--."},
  {"H", "...."},
  {"I", ".."},
  {"J", ".---"},
  {"K", "-.-"},
  {"L", ".-.."},
  {"M", "--"},
  {"N", "-."},
  {"O", "---"},
  {"P", ".--."},
  {"Q", "--.-"},
  {"R", ".-."},
  {"S", "..."},
  {"T", "-"},
  {"U", "..-"},
  {"V", "...-"},
  {"W", ".--"},
  {"X", "-..-"},
  {"Y", "-.--"},
  {"Z", "--.."},
  {"1", ".----"},
  {"2", "..---"},
  {"3", "...--"},
  {"4", "....-"},
  {"5", "....."},
  {"6", "-...."},
  {"7", "--..."},
  {"8", "---.."},
  {"9", "----."},
  {"0", "-----"}
};

String encodeMorse(String input) {
  input.toUpperCase();
  String morse = "";
  for (int i = 0; i < input.length(); i++) {
    char c = input[i];
    if(c == ' '){
      morse += "/";
    }
    else if (c == '.' || c == '?' || c == '!') {
      morse += "//";
    } else{
      for (int j = 0; j < COUNT_OF_CHARS; j++) {
        if (morseCode[j][0] == String(c)) {
          morse += morseCode[j][1];
          morse += "/";
        }
      }
    }
  }
  if(morse[morse.length()-2] != '/'){
      morse += "/";
    }
  return morse;
}

String getInput() {
  String input = "";
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read(); // Přečte jeden znak
      if (c == '\n') { // Pokud uživatel stiskne Enter, ukončí čtení
        break;
      } else if (c == '\b' || c == 127) { // Detekce Backspace (127 je standardní hodnota pro Delete)
        if (input.length() > 0) {
          input.remove(input.length() - 1); // Smaže poslední znak
          Serial.print("\b \b"); // Posune kurzor zpět, smaže znak na terminálu
        }
      } else {
        input += c; // Přidá znak do vstupu
        Serial.print(c); // Vypíše znak zpět na terminál
      }
    }
  }
  input.trim(); // Odstraní mezery a nové řádky na začátku a konci
  return input;
}

void blinkMorse(String morse) {
  for (int i = 0; i < morse.length(); i++) {
    char c = morse[i];
    if (c == '.') {
      digitalWrite(GREEN, HIGH);
      digitalWrite(BUZZER, HIGH);
      delay(DOT_DURATION);
      digitalWrite(GREEN, LOW);
      digitalWrite(BUZZER, LOW);
      delay(DOT_DURATION);
    } else if (c == '-') {
      digitalWrite(GREEN, HIGH);
      digitalWrite(BUZZER, HIGH);
      delay(DASH_DURATION);
      digitalWrite(GREEN, LOW);
      digitalWrite(BUZZER, LOW);
      delay(DOT_DURATION);
    } else if (c == '/') {
      digitalWrite(RED, HIGH);
      delay(SPACE_DURATION);
      digitalWrite(RED, LOW);
    }
  }
}

void setup() {
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);  
  Serial.begin(9600);
}

void loop() {
  Serial.print("Zadej vstup pro zakódování do morseovy abecedy: ");
  String input = getInput(); // Zavolá funkci pro získání vstupu
  Serial.println("\nZadaný vstup: " + input);
  String morse = encodeMorse(input); // Zavolá funkci pro zakódování vstupu do morseovy abecedy
  Serial.println("Morseovka: " + morse);
  blinkMorse(morse); // Zavolá funkci pro přehrání morseovy abecedy
}