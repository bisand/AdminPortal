#include <Arduino.h>
#include "AdminPortal.h"

AdminPortal *portal;

void setup()
{
  Config *config = new Config();
  config->AddValue("Test1", 0);
  config->AddValue("Test2", (float)0.0);
  portal = new AdminPortal(*config);

  portal->setup();
}

void loop()
{
  portal->loop();
}