#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ESPAsyncWebServer.h>
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include <list>
#include <map>
#include "WebPages.h"

class ConfigFormElement
{
private:
public:
  String name;
  String label;
  String group;
  String value;

  ConfigFormElement(String name, String label, String group, String value);
  ~ConfigFormElement();
};

class AdminPortal
{
private:
  AsyncWebServer *_webServer;
  AsyncEventSource *_events;

  unsigned long _currMillis = 0;
  unsigned long _interval = 500;
  IPAddress *_apIP;
  const char *_host;
  const char *_ssid;
  const char *_password;
  bool isDebug;
  WebPages *_wp;

  std::list<ConfigFormElement *> *_configFormElements;

  static void onNotFound(AsyncWebServerRequest *request);
  static void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

  String getConfigForm();

public:
  AdminPortal();
  ~AdminPortal();

  std::map<String, String> loadConfig();
  void saveConfig(std::map<String, String> config);
  void deleteConfig();
  bool formatSPIFFS();

  void addConfigFormElement(String name, String label, String group, String value);

  void log(const char *topic, const char *text);
  void log(const char *text);

  void setup();
  void loop();
};