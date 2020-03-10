//Script by Dylan Rovito
//Made for "Entity" Escape Room
//FOG RELAY CONTROLLER SCRIPT FOR NODE MCU (ESP8266)
//Controls 1 Relay connected to GPIO pin 2

#include <ESP8266WiFi.h>
#include<PubSubClient.h>

void callback(char* topic,byte* payload,unsigned int length1){    
Serial.print("message arrived[");
Serial.print(topic);
Serial.println("]");

for(int i=0;i<length1;i++){
  Serial.print(payload[i]); 
  
}
if(payload[0]==49) digitalWrite(2,HIGH);    //ASCII VALUE OF '1' IS 49
else if(payload[0]==50)digitalWrite(2,LOW);//ASCII VALUE OF '2' IS 50
Serial.println();
}

const char* mqtt_server="192.168.86.27";
WiFiClient espclient;
PubSubClient client(mqtt_server,1883,callback,espclient);


void setup() {
  
   pinMode(2,OUTPUT);
  Serial.begin(115200);
  Serial.print("connecting");
  WiFi.begin("TAZMANHOME","F68ACRogers");         //SSID,PASSWORD 
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  reconnect();

}





void reconnect(){
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  while(!client.connected()){
  if(client.connect("ESP8266ClientFogMachine")){
    Serial.println("connected");
    client.subscribe("fogrelay");
  }
    else{
      Serial.print("failed,rc=");
      Serial.println(client.state());
      delay(500);
    }
  } 
}
void loop() {
    if(!client.connected()){
      reconnect();
    }
    
    client.loop();

}
