#include <WiFi.h>
#include <HTTPClient.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
#define ID 111111

#include <WiFi.h>
#include <HTTPClient.h>



const int trigPin1 = 5;
const int echoPin1 = 18;
const int trigPin2= 26;
const int echoPin2=25;

const int binHeight=25.5;
const int smoke=19;
int smokeout=21;
float levelInterval=0.15;

int smokeInterval=1000;

const char* ssid = "Protosem";
const char* password = "proto123";

long duration1;
float distanceCm1;

long duration2;
float distanceCm2;


bool sentSmokeStatus=false;
bool smokeStatus=false;


int distancePercentage;
String serverName = "http://192.168.57.202:4040/sample";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(trigPin1,OUTPUT);
  pinMode(echoPin1,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);
  pinMode(smoke,INPUT);
  pinMode(smokeout,OUTPUT);
  digitalWrite(smokeout,HIGH);

}

void measure1(){
digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
   digitalWrite(trigPin1, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin1, LOW);
   duration1 = pulseIn(echoPin1, HIGH);
   distanceCm1 = duration1 * SOUND_SPEED/2;
}


void measure2(){
digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
   digitalWrite(trigPin2, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin2, LOW);
   duration2 = pulseIn(echoPin2, HIGH);
   distanceCm2 = duration2 * SOUND_SPEED/2;
}

void mesureLevel(){
  measure1();
 // measure2();

  
  //int distanceCm=(distanceCm1+distanceCm2)/2;
  int distanceCm=distanceCm1;
  
 
  distancePercentage=(((binHeight-distanceCm)*100)/binHeight);
  distancePercentage = distancePercentage-(distancePercentage%10);
  Serial.println(distancePercentage);
}

void httpSend(int dataType){

  HTTPClient http;

  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  
  String Data="";

  Data.concat("{\"ID\":\"");
  Data.concat(ID);
  Data.concat("\",");
  
  Data.concat("\n\"dataType\":\"");
  Data.concat(dataType);
  Data.concat("\",");

  switch (dataType)
  {
  case 0:
      Data.concat("\n\"level\":\"");
      Data.concat(distancePercentage);
      Data.concat("\",");
      Data.concat("\n\"interval\":\"");
      Data.concat(levelInterval);
      
    break;
  case 1:
      Data.concat("\n\"smoke\":\"");
      Data.concat(smokeStatus);
      
    break;

  default:
    break;
  }

  Data.concat("\"}");
  
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

void detectSmoke(){

  if(digitalRead(smoke)==1){
      smokeStatus=true;    
    }
    else{
      smokeStatus=false;
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  mesureLevel();
  httpSend(0);
  for(int i=0;i<(levelInterval*60*1000)/smokeInterval;i++){
    delay(smokeInterval);
    detectSmoke();
    
    if(smokeStatus!=sentSmokeStatus){
      httpSend(1);
      sentSmokeStatus=smokeStatus;
    }
    
  }

}
