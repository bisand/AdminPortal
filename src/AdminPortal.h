#include <Arduino.h>
#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ESPAsyncWebServer.h>
#include "ArduinoJson.h"
#include "SPIFFS.h"
#include <list>
#include <map>
#include "WebPages.h"

struct ConfigFormElement
{
  String name;
  String label;
  String group;
  String valueType;
  String value;
};

class AdminPortal
{
private:
  AsyncWebServer *_webServer;
  AsyncEventSource *_events;

  unsigned long _currMillis = 0;
  unsigned long _interval = 500;
  IPAddress *_apIP;
  bool _isDebug;
  WebPages *_wp;

  std::list<ConfigFormElement> _configFormElements;

  static void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
  String getConfigForm();

public:
  AdminPortal();
  ~AdminPortal();

  String processor(const String &var);

  std::map<String, String> loadConfig();
  void saveConfig(std::map<String, String> config);
  void deleteConfig();
  bool formatSPIFFS();

  void addConfigFormElement(String name, String label, String group, String value, String valueType = "text");

  void log(const char *topic, const char *text);
  void log(const char *text);

  void setup();
  void loop();
};