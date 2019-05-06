#include <WiFi.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ESPAsyncWebServer.h>
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include <list>
#include <map>

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
  unsigned long _currMillis = 0;
  unsigned long _interval = 1;
  IPAddress *_apIP;
  char *_host;
  char *_ssid;
  char *_password;
  bool isDebug;

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

  void addConfigFormElement(String name, String label, String group, String value);

  void setup();
  void loop();
};