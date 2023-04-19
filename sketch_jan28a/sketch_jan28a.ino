int LDR = A0;            // LDR input at A0 pin.
int LED = D5;             // LED is connected to PWM Pin 3.
int LDRReading = 0;      // to store input value of LDR
int lEDBrightness = 0;   // to store the value of LED Brightness
int threshold_val = 800; // Check your threshold and modify it.

void setup(){
  Serial.begin(115200);     // initializing serail communication.
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
  // Defining LED pin as output.
}

void loop(){
  analogRead(LDR,HIGH);
  digitalWrite(LED,HIGH);
  LDRReading = analogRead(LDR);    // Reading LDR Input.
  Serial.println(LDRReading);      // Printing LDR input value.
}
