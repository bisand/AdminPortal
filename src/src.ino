#include <Arduino.h>
#include "AdminPortal.h"

AdminPortal *portal;

void setup()
{
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

void loop()
{
  portal->loop();
}