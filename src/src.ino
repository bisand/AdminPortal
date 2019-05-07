#include <Arduino.h>
#include "AdminPortal.h"

AdminPortal *portal;

void setup()
{
  portal = new AdminPortal("EngMon", "Password123");

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