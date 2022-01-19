#include <Arduino.h>
#include "EspMQTTClient.h"
#include <ArduinoJson.h>


EspMQTTClient client(
  "netis_3C8FA0",
  "t.yusupov@alif.mobi",
  "broker.emqx.io",  // MQTT Broker server ip
  "emqx",   // Can be omitted if not needed
  "public",   // Can be omitted if not needed
  "mqttid_ff2a3c44",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

DynamicJsonDocument doc(1024);
void setup()
{
  Serial.begin(115200);
  

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
  client.setMaxPacketSize(512);
  client.enableDebuggingMessages(false);
  pinMode(13, OUTPUT);
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("Navoiy1", [](const String & topic) {
    Serial.println(topic);

  });

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  client.subscribe("Navoiy", [](const String & topic, const String & payload) {
     deserializeJson(doc, payload);
     JsonObject obj = doc.as<JsonObject>();
     String name = obj[F("name")];
     Serial.println(name);
     String ontime2 = obj[F("ontime2")];
     bool switch1 = obj[F("switch1")];
     digitalWrite(13,switch1);
     Serial.println(switch1 );

  });

  // Publish a message to "mytopic/test"
  client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("mytopic/wildcardtest/test123", "This is a message sent 5 seconds later");
  });
}

void loop()
{
  client.loop();
}