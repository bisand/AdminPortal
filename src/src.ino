#include <Arduino.h>
#include "AdminPortal.h"

AdminPortal *portal;

void setup()
{
  Serial.begin(115200);

  WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255, 255, 255, 0));
  WiFi.softAP("EngMon", "Password123");

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("EngMon");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin("EngMon"))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  portal = new AdminPortal();

  std::map<String, String> config = portal->loadConfig();
  if(config.size() < 1)
  {
    config["flow_mlpp_in"] = "5.5";
    config["flow_mlpp_out"] = "6.5";
    config["flow_moving_avg"] = "32";
    portal->saveConfig(config);
  }

  int movingAvg = config["flow_moving_avg"].toInt();

  portal->setup();
}

unsigned long _currMillis = 0;
unsigned long _interval = 1000;

void loop()
{
  if (millis() - _currMillis > _interval)
  {
    _currMillis = millis();
    portal->log(String(millis()).c_str());
  }

  portal->loop();
}