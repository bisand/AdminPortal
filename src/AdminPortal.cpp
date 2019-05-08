#include "AdminPortal.h"

AdminPortal::AdminPortal()
{
  _webServer = new AsyncWebServer(80);
  _events = new AsyncEventSource("/log_events");
  _wp = new WebPages();
}

AdminPortal::~AdminPortal()
{
  delete _webServer;
  delete _events;
  delete _wp;
}

/*
 * Login page
 */
const char *loginIndex = "";

// Replaces placeholder with LED state value
String processor(const String &var)
{
  Serial.println(var);
  if (var == "VERSION")
  {
    return "v1.0";
  }
  return String();
}

// Handle 404 not found responses.
void AdminPortal::onNotFound(AsyncWebServerRequest *request)
{
  request->send(SPIFFS, "/404.html", String(), false, processor);
}

// Handles upload of firmware.
void AdminPortal::onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if (!index)
  {
    Serial.printf("Update: %s\n", filename.c_str());
    if (!Update.begin(UPDATE_SIZE_UNKNOWN))
    { //start with max available size
      Update.printError(Serial);
    }
  }
  else if (!final)
  {
    if (Update.write(data, len) != len)
    {
      Update.printError(Serial);
    }
  }
  else if (final)
  {
    if (Update.end(true))
    { //true to set the size to the current progress
      Serial.printf("Update Success: %u\nRebooting...\n", index);
    }
    else
    {
      Update.printError(Serial);
    }
  }
}

// Read config file.
void AdminPortal::deleteConfig()
{
  std::map<String, String> result;
  if (SPIFFS.begin())
  {
    if (isDebug)
      Serial.println("mounted file system");
    if (SPIFFS.exists("/cfg.json"))
    {
      SPIFFS.remove("/cfg.json");
    }
  }
}

// Read config file.
std::map<String, String> AdminPortal::loadConfig()
{
  std::map<String, String> result;
  if (SPIFFS.begin())
  {
    if (isDebug)
      Serial.println("mounted file system");
    if (SPIFFS.exists("/cfg.json"))
    {
      //file exists, reading and loading
      if (isDebug)
        Serial.println("reading config file");
      File configFile = SPIFFS.open("/cfg.json", "r");
      if (configFile)
      {
        if (isDebug)
          Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        if (isDebug)
          Serial.println(buf.get());
        configFile.readBytes(buf.get(), size);

        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, configFile);
        if (!error)
        {
          if (isDebug)
            Serial.println("\nparsed json");

          JsonObject root = doc.as<JsonObject>();
          for (JsonPair kv : root)
          {
            result[kv.key().c_str()] = kv.value().as<String>();
          }
        }
        else
        {
          if (isDebug)
            Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  }
  else
  {
    if (isDebug)
      Serial.println("failed to mount FS");
  }
  return result;
  //end read
}

// Save config file.
void AdminPortal::saveConfig(std::map<String, String> config)
{
  if (isDebug)
    Serial.println("saving config");
  DynamicJsonDocument doc(1024);

  // Dynamically map values.
  std::map<String, String>::iterator it;
  for (it = config.begin(); it != config.end(); it++)
  {
    doc[it->first] = it->second;
  }

  File configFile = SPIFFS.open("/cfg.json", "w");
  if (!configFile)
  {
    if (isDebug)
      Serial.println("failed to open config file for writing");
  }

  // Serialize JSON to file
  if (serializeJson(doc, configFile) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }
  configFile.close();
  //end save
}

bool AdminPortal::formatSPIFFS()
{
  return SPIFFS.format();
}

void AdminPortal::addConfigFormElement(String name, String label, String group, String value)
{
  if (!_configFormElements)
    _configFormElements = new std::list<ConfigFormElement *>();

  ConfigFormElement *configFormElement = new ConfigFormElement(name, label, group, value);
  _configFormElements->push_back(configFormElement);
}

String AdminPortal::getConfigForm()
{
  String result;
  if (!_configFormElements)
    return result;

  std::list<ConfigFormElement *>::iterator it;
  String lastGroup = "";
  for (it = _configFormElements->begin(); it != _configFormElements->end(); ++it)
  {
    String tmp = "";
    if (lastGroup != (*it)->group)
    {
      tmp += "<div class=\"row\"><div class=\"col c12\"><h3>" + (*it)->group + "</h3></div></div>";
      lastGroup = (*it)->group;
    }
    tmp += "<div class=\"row\">";
    tmp += "<div class=\"col c2\"><label for=\"" + (*it)->name + "\"></label></div>";
    tmp += "<div class=\"col c10\"><input type=\"number\" name=\"" + (*it)->name + "\" value=\"%" + (*it)->name + "%\" class=\"smooth\" /></div>";
    tmp += "</div>";
  }

  return result;
}

void AdminPortal::log(const char *topic, const char *text)
{
  _events->send(text, topic, millis());
}

void AdminPortal::log(const char *text)
{
  _events->send(text, "log_event", millis());
}

const char *www_username = "admin";
const char *www_password = "esp32";
const char *www_realm = "Custom Auth Realm";
String authFailResponse = "Authentication Failed";

/*
 * setup function
 */
void AdminPortal::setup(void)
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Display landing page.
  _webServer->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (SPIFFS.exists("/index.html"))
      request->send(SPIFFS, "/index.html", String(), false, processor);
    else
      request->send_P(200, "text/html", _wp->index_html, processor );
  });

  _webServer->on("/index.html", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (SPIFFS.exists("/index.html"))
      request->send(SPIFFS, "/index.html", String(), false, processor);
    else
      request->send_P(200, "text/html", _wp->index_html, processor );
  });

  _webServer->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  _webServer->on("/docs", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (SPIFFS.exists("/docs.html"))
      request->send(SPIFFS, "/docs.html", String(), false, processor);
    else
      request->send_P(200, "text/html", _wp->docs_html, processor );
  });

  // Display configuration page.
  _webServer->on("/config", HTTP_GET, [&](AsyncWebServerRequest *request) {
    // if (!request->authenticate(www_username, www_password))
    //   return request->requestAuthentication();
    if (SPIFFS.exists("/config.html"))
      request->send(SPIFFS, "/config.html", String(), false, processor);
    else
      request->send_P(200, "text/html", _wp->config_html, processor );
  });

  // Display configuration page.
  _webServer->on("/monitor", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (SPIFFS.exists("/monitor.html"))
      request->send(SPIFFS, "/monitor.html", String(), false, processor);
    else
      request->send_P(200, "text/html", _wp->monitor_html, processor );
  });

  // Display firmware upgrade page.
  _webServer->on("/upgrade", HTTP_GET, [&](AsyncWebServerRequest *request) {
    // if (!request->authenticate(www_username, www_password))
    //   return request->requestAuthentication();
    if (SPIFFS.exists("/upgrade.html"))
      request->send(SPIFFS, "/upgrade.html", String(), false, processor);
    else
      request->send_P(200, "text/html", _wp->upgrade_html, processor );
  });

  /*handling uploading firmware file */
  _webServer->on("/uploadfw", HTTP_POST, [](AsyncWebServerRequest *request) { request->send(200); }, onUpload);

  // Display landing page.
  _webServer->on("/logout", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->authenticate(www_username, www_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Display 404 if no pages was found.
  _webServer->onNotFound(onNotFound);

  _events->onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got was: %u\n", client->lastId());
    }
    client->send("hello!","log_event",millis(),1000);
  });
  // _events->setAuthentication(www_username, www_password);
  _webServer->addHandler(_events);

  _webServer->begin();
}

void AdminPortal::loop(void)
{
  if (millis() - _currMillis > _interval)
  {
    _currMillis = millis();
  }
}
