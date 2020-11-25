#include "MqttClient.h"


MqttClient::MqttClient()
{
   gsm = GsmModule();
   gps = GPS();
};


void MqttClient::setID(String id)
{
   this->id = id;
}

void MqttClient::setupMqtt()
{
   for(int i = 8; i > 0; i--){
       Serial.println(String(i) + " seconds -  waiting for lte module to activate");
       delay(1000);
       }

   // Creates random clientID for each connection
   String clientId = "BoatDevice-";

   clientId += String(random(0xffff), HEX);

   //Setup to connect to network and mqttbroker
   // Deactivate pin on simcard for this code to work!----------------
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


   publishData(this->id, publish);
   publishData(json.stringifyJsonLocation(gps.getLat(), gps.getLng(), this->id, "online"), "/clientStatus");

   //set will message to give the server notice when the device lose connection
   String willTopic       = "/clientStatus";
   String willTopicLength = String(strlen(willTopic));

   gsm.sendAndReadResponse("AT+CMQTTWILLTOPIC=0," + willTopicLength);
   delay(1000);
   gsm.sendAndReadResponse(willTopic);
   delay(1000);
   gsm.sendAndReadResponse("AT+CMQTTWILLMSG=?");
   delay(1000);

   String willMessage = json.stringifyJsonLocation(gps.getLat(), gps.getLng(), this->id, "offline");
   String willLength  = String(strlen(willMessage));

   gsm.sendAndReadResponse("AT+CMQTTWILLMSG=0," + willLength + ",1");
   delay(600);
   gsm.sendAndReadResponse(willMessage, 2000);
   delay(1000);
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

String MqttClient::checkTopicForRetainedMessages(String subTopic)
{
   int    x      = 0;
   bool   answer = false;
   char   responseBuffer[300];//increased buffersize because of trouble with buffer overflowing
   long   previous;
   char   payload[100];
   String expected_answer = "+CMQTTRXPAYLOAD: 0,"; //the response from the module that tells us the mqtt message payload is incoming

   memset(responseBuffer, '\0', 300);
   memset(payload, '\0', 100);

   while(Serial1.available() > 0){                             // Clean the input buffer
         Serial1.read();
         }

   String subLength = String(strlen(subTopic));

   Serial1.println("AT+CMQTTSUB=0," + subLength + ",1"); // Send length of topic to subscribe to
   delay(1000);
   Serial1.println(subTopic);                            //send actual topic

   previous = millis();


   do {
      if(Serial1.available() != 0){
         //reset response index and set all to null to avoid memory leak and crash if large amount of data comes through buffer
         if(x > 299){
            memset(responseBuffer, '\0', 300);
            x = 0;
            }

         responseBuffer[x] = Serial1.read();
         Serial.print(responseBuffer[x]);
         x++;

         //if the loop detects an ERROR in the buffer it will restart and resubscribe to teh topic
         if(strstr(responseBuffer, "ERROR") != NULL){
            while(Serial1.available() > 0){                            // Clean the input buffer
                  Serial1.read();
                  }

            Serial1.println("AT+CMQTTUNSUBTOPIC=0," + subLength);
            delay(1000);
            Serial1.println(subTopic);
            delay(600);
            Serial1.println("AT+CMQTTUNSUB=0,0");
            delay(600);
            Serial1.println("AT+CMQTTSUB=0," + subLength + ",1"); // Send length of topic to subscribe to
            delay(1000);
            Serial1.println(subTopic);
            memset(responseBuffer, '\n', 300);
            previous = millis();
            x        = 0;
            }

         //when the desired answer is found we know the next characters in line by reading the documentation: https://simcom.ee/documents/SIM7X00/SIM7500_SIM7600_SIM7800%20Series_MQTT_AT%20Command%20Manual_V1.00.pdf
         if(strstr(responseBuffer, expected_answer) != NULL){
            Serial.println(responseBuffer);
            while(Serial1.available() > 0 && !answer){
                  char ch = Serial1.read();
                  char payLoadLengthAscii[4];
                  memset(payLoadLengthAscii, '\0', 4);
                  int z = 0;
                  while(ch != '\r'){
                        payLoadLengthAscii[z] = ch;
                        ch = Serial1.read();
                        z++;
                        }
                  Serial1.read();                               //reads '\n' from buffer
                  int payLoadLength = atoi(payLoadLengthAscii); //http://www.cplusplus.com/reference/cstdlib/atoi/
                  //next characters will be the actual mqtt payload
                  //source: 2.3.2.2 Receive publish message from MQTT server.
                  for(int i = 0; i <= payLoadLength; i++){
                      if(i == payLoadLength || (i > 99)){
                         answer = true;
                         break;
                         }
                      else{
                          ch         = Serial1.read();
                          payload[i] = ch;
                          }
                      }
                  }
            }
         }
      } while (!answer && ((millis() - previous) < 1000));

   //unsubscribe to topic to be able to do other things without overloading the buffer
   Serial1.println("AT+CMQTTUNSUBTOPIC=0," + subLength);
   delay(1000);
   Serial1.println(subTopic);
   delay(600);
   Serial1.println("AT+CMQTTUNSUB=0,0");
   delay(600);
   return(payload);
}
