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
digitalWrite(4, LOW);
}
else if(payload[0]==50){ //2 ON
digitalWrite(4, HIGH);
}
Serial.println();
}

void setup() {
  delay( 1000 ); // power-up safety delay
  Serial.begin(115200);
  Serial.print("connecting");
  pinMode(2, INPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  WiFi.begin("WIFI-SSID-REDACTED","WIFI-PASSWORD-REDACTED");         //SSID,PASSWORD 
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  reconnect();

}



const char* mqtt_server="MQTT-SERVER-IP-REDACTED"; //MQTT Server IP Address
WiFiClient espclient;
PubSubClient client(mqtt_server,1883,callback,espclient);

void reconnect(){
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  while(!client.connected()){
  if(client.connect("controlpanelclient")){
    Serial.println("connected");
    client.subscribe("controlIn");
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
    client.publish("controlOut","2");
    DoorOpen = true;
    Serial.println("Recieved Control Signal");
      }
    }
    else{
      if (DoorOpen == true) {
        client.publish("controlOut","1");
        DoorOpen = false;
      }
    }
    client.loop();
    
    }
