#pragma once
#include "application.h"
#include "Particle.h"
#include "JsonParser.h"
#include "GsmModule.h"

class MqttClient {
private:
     int id;
public:
   int counter = 0;
   GsmModule gsm;

   JsonParser json  = JsonParser();
   String publish   = "/connected/";
   String subscribe = "/openDevice/1111";
   String payload   = "device connected";

   MqttClient();
   void setID(int id);
   void setupMqtt();
   void readSerial();
   void publishData(String data, String topic);

};
