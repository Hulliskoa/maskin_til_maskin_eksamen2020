#include "MqttClient.h"
#include "GPS.h"


String     id    = System.deviceID();       //Use particle photon embedded id
int        relay = D6;
MqttClient mqtt;
GPS        gps;
int        openOrClosed  = false;
bool       setupComplete = false;

void setup()
{
   Serial.begin(115200);
   Serial1.begin(115200);
   pinMode(relay, OUTPUT);
}

void loop()
{
   //in the current version of the photon firmware EEPROM can not be manipulated in the setup function and thats why I moved most of the setup here
   if(!setupComplete){
      gps = GPS();
      gps.initializeGPS();//Reads and writes to EEPROM
      mqtt = MqttClient();
      mqtt.setID(id);
      gps.setID(id);
      mqtt.setupMqtt();
      setupComplete = true;
      }

   //checks mqtt broker for retained messages sent from nodejs server
   String retainedMessage = mqtt.checkTopicForRetainedMessages("/openDevice/" + id);

   if(retainedMessage == "1"){
      Serial.println("open");
      digitalWrite(relay, 1);//Unlocks relay
      mqtt.publishData("open", "/deviceStatus/" + id);
      openOrClosed = 1;
      }
   else if(retainedMessage == "0"){
           Serial.println("close");
           openOrClosed = 0;
           digitalWrite(relay, 0);//locks relay
           mqtt.publishData("closed", "/deviceStatus/" + id);
           }
   else{
       Serial.println("no change");
       }

   delay(1000);

   gps.getCurrentPosition();
}
