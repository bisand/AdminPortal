#include <Arduino.h>
#include "AdminPortal.h"

AdminPortal *portal;

void setup()
{
  portal = new AdminPortal();

  std::map<String, ConfigElement*> config = portal->loadConfig();
  if(config.size() < 1)
  {
    config["flow_mlpp_in"] = new ConfigFloat();
    config["flow_mlpp_out"] = new ConfigFloat();
    config["flow_moving_avg"] = new ConfigFloat();
    portal->saveConfig(config);
  }

  portal->setup();
}

void loop()
{
  portal->loop();
}