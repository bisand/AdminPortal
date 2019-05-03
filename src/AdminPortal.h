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

class Config
{
  private:
    class ConfigElement
    {
      public:
        int *valInt;
        float *valFloat;
        String *valString;
    };
    std::map<String, ConfigElement*> _config;
  public:
    void AddValue(String key, int value){
      ConfigElement *ce = new ConfigElement();
      ce->valInt = new int(value);
      _config[key] = ce;
    }
    void AddValue(String key, float value){
      ConfigElement *ce = new ConfigElement();
      ce->valFloat = new float(value);
      _config[key] = ce;
    }
    void AddValue(String key, String value){
      ConfigElement *ce = new ConfigElement();
      ce->valString = new String(value);
      _config[key] = ce;
    }
    String GetStringValue(String key)
    {
      ConfigElement *ce = _config[key];
      return *ce->valString;
    }
    int GetIntValue(String key)
    {
      ConfigElement *ce = _config[key];
      return *ce->valInt;
    }
    float GetFloatValue(String key)
    {
      ConfigElement *ce = _config[key];
      return *ce->valFloat;
    }
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

  Config _config;

  void readConfigFile();
  void writeConfigFile();

  static void onNotFound(AsyncWebServerRequest *request);
  static void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

public:
  AdminPortal(Config config);
  ~AdminPortal();

  void setup();
  void loop();
};