#include "MqttClient.h"
#include "GPS.h"

int id = 1111;

MqttClient mqtt = MqttClient();
GPS        gps  = GPS();

void setup()
{
   mqtt.setID(id);
   gps.setID(id);
   mqtt.setupMqtt();
   gps.initializeGPS();
}

void loop()
{
   mqtt.checkForNewMessages("/openDevice/1111");
   //sendPosition();
   //mqtt.readSerial();
}

void sendPosition()
{
   gps.GPSPositioning();
   delay(1000);
};
