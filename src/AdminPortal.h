#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#else
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#endif
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include <iostream>
#include <memory>
#include <map>

class ConfigElement
{
  public:
    String label;
};

class ConfigString : public ConfigElement
{
  public:
    String value;
};

class ConfigInt : public ConfigElement
{
  public:
    int value;
};

class ConfigFloat : public ConfigElement
{
  public:
    float value;
};

class ConfigDouble : public ConfigElement
{
  public:
    double value;
};

class ConfigLong : public ConfigElement
{
  public:
    long value;
};

class AdminPortal
{
private:
#ifdef ESP8266
  ESP8266WebServer *_webServer;
#else
  AsyncWebServer *_webServer;
#endif
  unsigned long _currMillis = 0;
  unsigned long _interval = 1;
  IPAddress *_apIP;
  char *_host;
  char *_ssid;
  char *_password;
  bool isDebug;

  static void onNotFound(AsyncWebServerRequest *request);
  static void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

public:
  AdminPortal();
  ~AdminPortal();

  std::map<String, ConfigElement*> loadConfig();
  void saveConfig(std::map<String, ConfigElement*> config);
  void deleteConfig();

  void setup();
  void loop();
};