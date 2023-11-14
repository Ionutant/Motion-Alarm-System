int LED_Pin = 13;
int vibr_Pin = 23;
int buzzer_Pin = 12;

void activateBuzzer() {
  digitalWrite(buzzer_Pin, HIGH);
}

void deactivateBuzzer() {
  digitalWrite(buzzer_Pin, LOW);
}

void setup() {
  pinMode(LED_Pin, OUTPUT);
  pinMode(vibr_Pin, INPUT);
  pinMode(buzzer_Pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  long measurement = pulseIn(vibr_Pin, HIGH);
  Serial.println(measurement);

  if (measurement > 1000) {
    digitalWrite(LED_Pin, HIGH);

    activateBuzzer();
    delay(2000); // Buzzer-ul va rămâne activ timp de 2 secunde
    deactivateBuzzer();

    digitalWrite(LED_Pin, LOW);
  } else {
    digitalWrite(LED_Pin, LOW);
  }

  delay(50);
}
