#include <Arduino.h>
#include "AdminPortal.h"

AdminPortal *portal = new AdminPortal();

void setup()
{
  portal->setup();
}

void loop()
{
  portal->loop();
}