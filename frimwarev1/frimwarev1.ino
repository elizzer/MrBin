const int trigPin = 5;
const int echoPin = 18;
const int binHeight=100;
const int smoke=19;
int levelInterval=1;
int smokeInterval=100

#include <WiFi.h>
#include <HTTPClient.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
#define ID 111112

const char* ssid = "Protosem";
const char* password = "proto123";

long duration;
float distanceCm;
float distanceInch;
bool smokeStatus=false;

String serverName = "http://localhost:4040/sample";
 
 void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Starts the serial communication

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(smoke,INPUT);

}

void mesure(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (%): ");
  int disper=((binHeight-distanceCm)/binHeight)*100;
  disper = disper-(disper%10);

    
  HTTPClient http;

  http.begin("http://192.168.57.18:4040/sample");
  http.addHeader("Content-Type", "application/json");

  String Data="";
  Data.concat("{\"ID\":\"");
  Data.concat(ID);
  Data.concat("\",");
  Data.concat("\n\"level\":\"");
  Data.concat(disper);
  Data.concat("\",");
  Data.concat("\n\"interval\":\"");
  Data.concat(interval);
  Data.concat("\",");
  Data.concat("\n\"smoke\":\"");
  Data.concat(smokeStatus);
  Data.concat("\"}");
  Serial.println(Data);
  int httpResponseCode = http.POST(Data);
      
  if (httpResponseCode>0) {
    
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  mesure();
  
  for(int i=0;i<(levelInterval*60*60*1000)/smokeInterval;i++){
    delay(smokeInterval);

    if(digitalRead(smoke)==1){
      smokeStatus=true;  
      break;  
    }
    else{
      smokeStatus=false;
    }
  }  
}
