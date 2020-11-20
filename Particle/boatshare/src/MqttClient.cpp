#include "MqttClient.h"

MqttClient::MqttClient()
{
   gsm = GsmModule();
};

// Setup is done with the help of:
// https://gist.github.com/jenschr/2d248362a4c3c5651145b5f009586513
void MqttClient::setupMqtt()
{
   pinMode(D7, OUTPUT);
   Serial.begin(115200);
   Serial1.begin(115200);

   for(int i = 8; i > 0; i--){
       Serial.println("Wait " + String(i) + " boring seconds for the green LED to turn on");
       delay(1000);
       }

   // Creates random clientID
   String clientId = "ParticleClient-";

   clientId += String(random(0xffff), HEX);

   // Pin deactivated for sim-card!!
   gsm.sendAndReadResponse("AT");
   gsm.sendAndReadResponse("AT+CFUN=1");
   gsm.sendAndReadResponse("AT+CSQ");
   gsm.sendAndReadResponse("AT+CGACT=1,1");
   gsm.sendAndReadResponse("AT+CREG?");
   gsm.sendAndReadResponse("AT+CGREG?");
   gsm.sendAndReadResponse("AT+CGDCONT=1,\"IP\",\"telia\"");
   gsm.sendAndReadResponse("AT+CGPADDR");
   gsm.sendAndReadResponse("AT+CMQTTSTART");
   gsm.sendAndReadResponse("AT+CMQTTACCQ=0,\"" + clientId + "\"");
   gsm.sendAndReadResponse("AT+CMQTTCONNECT=0,\"tcp://hairdresser.cloudmqtt.com:15488\",60, 0,\"hwxtryvm\",\"Bga5mnvq1sxC\"");

   publishData(payload, publish);
   publishData(json.stringifyJsonLocation("58.999210", "10.041967", this->id), "/addClient");

 String subLength = String(strlen(subscribe));

   gsm.sendAndReadResponse("AT+CMQTTSUB=0,"+ subLength + ",1");
   Serial1.print(subscribe);
}

void MqttClient::publishData(String data, String topic)
{
   String topicLength = String(strlen(topic));

   gsm.sendAndReadResponse("AT+CMQTTTOPIC=0," + topicLength);
   Serial1.print(topic);
   delay(1000);

   String payloadLength = String(strlen(data));

   gsm.sendAndReadResponse("AT+CMQTTPAYLOAD=0," + payloadLength);
   Serial1.println(data);
   delay(1000);

   gsm.sendAndReadResponse("AT+CMQTTPUB=0,1,60");
   delay(1000);
}

// In the main loop, you can keep entering AT commands from your Serial Monitor
void MqttClient::readSerial()
{
   /* send everything received from the SIM7600
    * to usb serial & vice versa */
   if(Serial.available() > 0){
      Serial.print(">");

      while(Serial.available()){
            char ch = Serial.read();
            Serial.print(ch);
            Serial1.print(ch);
            }
      }

   if(Serial1.available() > 0){
      Serial.print(":");

      while(Serial1.available()){
            char ch = Serial1.read();

            if(ch){
               Serial.print(ch);
               }
            }
      }

   // blink the LED, just to show we are alive
   if(counter % 1000 == 0){
      digitalWrite(D7, !digitalRead(D7));
      }
   counter++;
}

void MqttClient::setID(int id)
{
   this->id = id;
}
