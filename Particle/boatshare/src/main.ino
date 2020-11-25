#include "MqttClient.h"
#include "GPS.h"
#include "JsonParser.h"

String     id    = System.deviceID();       //Use particle photon embedded id
int        relay = D6;
MqttClient mqtt;
GPS        gps;
JsonParser json;
int        openOrClosed  = false;
bool       setupComplete = false;
long       previous;
void setup()
{
   Serial.begin(115200);
   Serial1.begin(115200);
   pinMode(relay, OUTPUT);
   previous = millis();
}

void loop()
{
   //in the current version of the photon firmware EEPROM can not be manipulated in the setup function and thats why I moved most of the setup here
   if(!setupComplete){
      gps  = GPS();
      json = JsonParser();
      gps.initializeGPS();//Reads and writes to EEPROM
      mqtt = MqttClient();
      mqtt.setID(id);
      gps.setID(id);
      mqtt.setupMqtt();
      setupComplete = true;
      }

   //checks mqtt broker for retained messages sent from nodejs server
   String retainedMessage = mqtt.checkTopicForRetainedMessages("/openDevice/" + id); //

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



   //check for position every ten seconds
   if((millis() - previous) > 20000){

      if(gps.getCurrentPosition()){
         mqtt.publishData(json.stringifyJsonLocation(gps.getLat(), gps.getLng(), id, "online"), "/clientStatus");
         }
      else{
          mqtt.publishData("sattelites not found for device: " + id, "/clientStatus");
          }
          previous = millis();
      }

}
