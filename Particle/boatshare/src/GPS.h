#pragma once
#include "application.h"
#include "Particle.h"
#include "GsmModule.h"
#include "MqttClient.h"
#include "JsonParser.h"

class GPS {
private:
  String lat = "58.999210";
  String lng = "10.041967";
  int id  = 0;
public:

   GsmModule gsm;
   MqttClient mqtt;
   GPS();
   JsonParser json = JsonParser();
   void setID(int id);
   String getLat();
   String getLng();
   void setLat(String lat);
   void setLng(String lng);
   bool initializeGPS();
   bool GPSPositioning();
   String getPosition();
};
