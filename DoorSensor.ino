//Script by Dylan Rovito
//Made for "Entity" Escape Room
//DOOR REED SWITCH (MAGNETIC CONTACT SENSOR)
//Publishes a message when door is opened or closed

#include <ESP8266WiFi.h>
#include<PubSubClient.h>

int ReadState = 1;
bool DoorOpen = false;

void callback(char* topic,byte* payload,unsigned int length1){    
Serial.print("message arrived[");
Serial.print(topic);
Serial.println("]");

for(int i=0;i<length1;i++){
  Serial.print(payload[i]); 
}


if(payload[0]==49) {  //1 OFF
////////////////////////////////////////////////////////////
}
else if(payload[0]==50){ //2 ON
////////////////////////////////////////////////////////////
}
Serial.println();
}

void setup() {
  delay( 3000 ); // power-up safety delay
  Serial.begin(115200);
  Serial.print("connecting");
  pinMode(2, INPUT);
  WiFi.begin("TAZMANHOME","F68ACRogers");         //SSID,PASSWORD 
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  reconnect();

}



const char* mqtt_server="192.168.86.27";
WiFiClient espclient;
PubSubClient client(mqtt_server,1883,callback,espclient);

void reconnect(){
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  while(!client.connected()){
  if(client.connect("doorclient")){
    Serial.println("connected");
    client.subscribe("doorIn");
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
    ReadState = digitalRead(2);
    if (ReadState == LOW) {
      if (DoorOpen == false) {
    client.publish("doorOut","2");
    DoorOpen = true;
    Serial.println("Recieved Control Signal");
      }
    }
    else{
      if (DoorOpen == true) {
        client.publish("doorOut","1");
        DoorOpen = false;
      }
    }
    client.loop();
    
    }
