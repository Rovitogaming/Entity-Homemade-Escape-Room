//Script by Dylan Rovito
//Made for "Entity" Escape Room
//ROOM RELAY SCRIPT MADE FOR ESP8266 
//Controls 3 Relays Connected to "Digital" Pins 9, 4, 3 Over Wifi

#include <ESP8266WiFi.h>
#include<PubSubClient.h>

void callback(char* topic,byte* payload,unsigned int length1){    
Serial.print("message arrived[");
Serial.print(topic);
Serial.println("]");

for(int i=0;i<length1;i++){
  Serial.print(payload[i]); 
}

const char* mqtt_server="192.168.86.27";
WiFiClient espclient;
PubSubClient client(mqtt_server,1883,callback,espclient);

if(payload[0]==49) {
  digitalWrite(2,HIGH); //1 OFF
  client.publish("roomout","1OFF"); 
}
else if(payload[0]==50)digitalWrite(2,LOW); //2 ON
if(payload[0]==51) digitalWrite(5,HIGH); //3 OFF   
else if(payload[0]==52)digitalWrite(5,LOW); //4 ON
if(payload[0]==53) digitalWrite(4,HIGH); //5 OFF
else if(payload[0]==54)digitalWrite(4,LOW); //6 ON
Serial.println();
}

const char* mqtt_server="192.168.86.27";
WiFiClient espclient;
PubSubClient client(mqtt_server,1883,callback,espclient);


void setup() {
  
   pinMode(2,OUTPUT);
   pinMode(5, OUTPUT);
   pinMode(4, OUTPUT);
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
  if(client.connect("ESP8266Client123456789")){
    Serial.println("connected");
    client.subscribe("roomrelay");
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
