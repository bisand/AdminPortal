#include <Arduino.h>
#include "AdminPortal.h"

AdminPortal *portal;

void setup()
{
  portal = new AdminPortal();

  portal->setup();
}

void loop()
{
  portal->loop();
}