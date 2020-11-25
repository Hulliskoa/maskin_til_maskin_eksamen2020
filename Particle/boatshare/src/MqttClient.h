#pragma once
#include "application.h"
#include "Particle.h"
#include "JsonParser.h"
#include "GsmModule.h"
#include "GPS.h"

/**
 * \brief Handles all mqtt based communication between module and broker
 */
class MqttClient {
private:
   String id;/*!< device id of the particle photon */
public:
   GsmModule gsm;
   GPS gps;
   JsonParser json = JsonParser();
   String publish  = "/connected/";

   MqttClient();

   /**
    * Sets the member value id to the specified string value
    */
   void setID(String id /**< [in] device id to be set */);
   /**
    * Sets up communication with the mqttbroker by fetching network access and the connection to the mqtt broker
    * Setup is done with the help of: https://gist.github.com/jenschr/2d248362a4c3c5651145b5f009586513
    */
   void setupMqtt();
   /**
    * Publishes mqtt message to mqtt broker
    */
   void publishData(String data /**< [in] data to be published as mqtt message */, String topic  /**< [in] the mqtt topic used to publish at */);
   /**
    * Subscribes to the specified topic and checks for any retained messages from the broker
    * This is a very modified version of sendATcommand() written by: https://gist.github.com/IdrisCytron/7ed2defe995cec8109efc5561fb00886
    * Modifications made with the help of simcom 7600 documentation: https://simcom.ee/documents/SIM7X00/SIM7500_SIM7600_SIM7800%20Series_MQTT_AT%20Command%20Manual_V1.00.pdf
    */
   String checkTopicForRetainedMessages(String subTopic /**< [in] data to be published as mqtt message */);
};
