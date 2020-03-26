#include "AdminPortal.h"

void hard_esp_restart()
{
  esp_task_wdt_init(1, true);
  esp_task_wdt_add(NULL);
  while (true)
    ;
}

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

// Replaces placeholder with LED state value
String AdminPortal::processor(const String &var)
{
  Serial.println(var);
  if (var == "VERSION")
  {
    return "v1.0";
  }
  if (var == "CONFIG_FORM")
  {
    return this->getConfigForm();
  }
  return String();
}

// void AdminPortal::setTemplateProcessor(AwsTemplateProcessor processorCallback)
// {
//   // AdminPortal::_processorCallback = processorCallback;
// }

// Handles upload of firmware.
void AdminPortal::onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
  uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
  if (!index)
  {
    Serial.println("Update");
    //Update.runAsync(true);
    if (!Update.begin(free_space))
    {
      Update.printError(Serial);
    }
  }

  if (Update.write(data, len) != len)
  {
    Update.printError(Serial);
  }

  if (final)
  {
    if (!Update.end(true))
    {
      Update.printError(Serial);
    }
    else
    {
      Serial.println("Update complete");
      hard_esp_restart();
    }
  }
}

// Read config file.
void AdminPortal::deleteConfig()
{
  if (SPIFFS.begin())
  {
    if (_isDebug)
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
    if (_isDebug)
      Serial.println("mounted file system");
    if (SPIFFS.exists("/cfg.json"))
    {
      //file exists, reading and loading
      if (_isDebug)
        Serial.println("reading config file");
      File configFile = SPIFFS.open("/cfg.json", "r");
      if (configFile)
      {
        if (_isDebug)
          Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        configFile.close();
        if (_isDebug)
          Serial.println(buf.get());

        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, buf.get());
        if (!error)
        {
          if (_isDebug)
            Serial.println("\nparsed json");

          JsonObject root = doc.as<JsonObject>();
          for (JsonPair kv : root)
          {
            result[kv.key().c_str()] = kv.value().as<String>();
          }
        }
        else
        {
          if (_isDebug)
            Serial.println("failed to load json config");
        }
      }
    }
  }
  else
  {
    if (_isDebug)
      Serial.println("failed to mount FS");
  }
  return result;
  //end read
}

// Save config file.
void AdminPortal::saveConfig(std::map<String, String> config)
{
  if (_isDebug)
    Serial.println("saving config");
  DynamicJsonDocument doc(1024);

  // Dynamically map values.
  std::map<String, String>::iterator it;
  for (it = config.begin(); it != config.end(); it++)
  {
    doc[it->first] = it->second;
    if (_isDebug)
    {
      Serial.print(it->first);
      Serial.print(" : ");
      Serial.println(it->second);
    }
  }

  File configFile = SPIFFS.open("/cfg.json", "w");
  if (!configFile)
  {
    if (_isDebug)
      Serial.println("failed to open config file for writing");
  }

  // Serialize JSON to file
  if (serializeJson(doc, configFile) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }
  configFile.close();
  if (_isDebug)
    Serial.println("saving done!");
  //end save
}

bool AdminPortal::formatSPIFFS()
{
  return SPIFFS.format();
}

void AdminPortal::addConfigFormElement(String name, String label, String group, String value, String valueType)
{
  ConfigFormElement element;
  element.name = name;
  element.label = label;
  element.group = group;
  element.value = value;
  element.valueType = valueType;
  Serial.print("> ");
  Serial.print(name);
  Serial.print(" : ");
  Serial.println(value);
  _configFormElements.push_back(element);
}

String AdminPortal::getConfigForm()
{
  String result = "";

  std::list<ConfigFormElement>::iterator it;
  String lastGroup = "";
  Serial.println("getConfigForm()");

  for (it = _configFormElements.begin(); it != _configFormElements.end(); ++it)
  {
    Serial.print("> ");
    Serial.print(it->name);
    Serial.print(" : ");
    Serial.println(it->value);
    Serial.flush();
    String tmp = "";
    if (lastGroup != it->group)
    {
      tmp += "<h3>" + String(it->group) + "</h3>";
      lastGroup = String(it->group);
    }
    tmp += "<label for=\"" + String(it->name) + "\">" + String(it->label) + "</label>";
    if (it->valueType.equalsIgnoreCase("checkbox"))
    {
      std::stringstream chk(it->value.c_str());
      bool isChecked;
      chk >> std::boolalpha >> isChecked;
      String checked = isChecked ? "checked=\"checked\"" : "";
      tmp += "<input type=\"hidden\" name=\"" + String(it->name) + "\" value=\"false\" />";
      tmp += "<input type=\"checkbox\" id=\"" + String(it->name) + "\" name=\"" + String(it->name) + "\"  value=\"true\" " + checked + "\" class=\"smooth\" />";
    }
    else
    {
      tmp += "<input type=\"" + String(it->valueType) + "\" id=\"" + String(it->name) + "\" name=\"" + String(it->name) + "\" value=\"" + String(it->value) + "\" class=\"smooth\" />";
    }
    result += tmp;
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

void AdminPortal::setfillConfigElementsCallback(void (*fillConfigElementsCallback)())
{
  _fillConfigElementsCallback = fillConfigElementsCallback;
}

void AdminPortal::clearConfigElements()
{
  _configFormElements.clear();
}

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
      request->send(SPIFFS, "/index.html", String(), false, [this](const String &var) -> String { return this->processor(var); });
    else
      request->send_P(200, "text/html", _wp->index_html, [this](const String &var) -> String { return this->processor(var); });
  });

  _webServer->on("/index.html", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (SPIFFS.exists("/index.html"))
      request->send(SPIFFS, "/index.html", String(), false, [this](const String &var) -> String { return this->processor(var); });
    else
      request->send_P(200, "text/html", _wp->index_html, [this](const String &var) -> String { return this->processor(var); });
  });

  _webServer->on("/style.css", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (SPIFFS.exists("/style.css"))
      request->send(SPIFFS, "/style.css", "text/css");
    else
      request->send_P(200, "text/css", _wp->index_html, [this](const String &var) -> String { return this->processor(var); });
  });

  _webServer->on("/docs", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (SPIFFS.exists("/docs.html"))
      request->send(SPIFFS, "/docs.html", String(), false, [this](const String &var) -> String { return this->processor(var); });
    else
      request->send_P(200, "text/html", _wp->docs_html, [this](const String &var) -> String { return this->processor(var); });
  });

  // Display configuration page.
  _webServer->on("/config", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (!request->authenticate(www_username, www_password))
      return request->requestAuthentication();

    if (_fillConfigElementsCallback != NULL)
      _fillConfigElementsCallback();

    if (SPIFFS.exists("/config.html"))
      request->send(SPIFFS, "/config.html", String(), false, [this](const String &var) -> String { return this->processor(var); });
    else
      request->send_P(200, "text/html", _wp->config_html, [this](const String &var) -> String { return this->processor(var); });
  });

  _webServer->on("/config", HTTP_POST, [&](AsyncWebServerRequest *request) {
    if (!request->authenticate(www_username, www_password))
      return request->requestAuthentication();

    //List all parameters
    std::map<String, String> config = loadConfig();
    int params = request->params();
    for (int i = 0; i < params; i++)
    {
      AsyncWebParameter *p = request->getParam(i);
      if (p->isFile())
      { //p->isPost() is also true
        Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      }
      else if (p->isPost())
      {
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        config[String(p->name())] = String(p->value());
      }
      else
      {
        Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
    saveConfig(config);


    if (_fillConfigElementsCallback != NULL)
      _fillConfigElementsCallback();

    if (SPIFFS.exists("/config.html"))
      request->send(SPIFFS, "/config.html", String(), false, [this](const String &var) -> String { return this->processor(var); });
    else
      request->send_P(200, "text/html", _wp->config_html, [this](const String &var) -> String { return this->processor(var); });
  });

  // Display configuration page.
  _webServer->on("/monitor", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (!request->authenticate(www_username, www_password))
      return request->requestAuthentication();
    if (SPIFFS.exists("/monitor.html"))
      request->send(SPIFFS, "/monitor.html", String(), false, [this](const String &var) -> String { return this->processor(var); });
    else
      request->send_P(200, "text/html", _wp->monitor_html, [this](const String &var) -> String { return this->processor(var); });
  });

  // Display firmware upgrade page.
  _webServer->on("/upgrade", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (!request->authenticate(www_username, www_password))
      return request->requestAuthentication();
    if (SPIFFS.exists("/upgrade.html"))
      request->send(SPIFFS, "/upgrade.html", String(), false, [this](const String &var) -> String { return this->processor(var); });
    else
      request->send_P(200, "text/html", _wp->upgrade_html, [this](const String &var) -> String { return this->processor(var); });
  });

  /*handling uploading firmware file */
  _webServer->on(
      "/uploadfw", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200);
      },
      onUpload);

  // Display landing page.
  _webServer->on("/logout", HTTP_GET, [&](AsyncWebServerRequest *request) {
    if (request->authenticate(www_username, www_password))
      return request->requestAuthentication();
    if (SPIFFS.exists("/index.html"))
      request->send(SPIFFS, "/index.html", String(), false, [this](const String &var) -> String { return this->processor(var); });
    else
      request->send_P(200, "text/html", _wp->index_html, [this](const String &var) -> String { return this->processor(var); });
  });

  // Display 404 if no pages was found.
  _webServer->onNotFound([&](AsyncWebServerRequest *request) {
    if (SPIFFS.exists("/404.html"))
      request->send(SPIFFS, "/404.html", String(), false, [this](const String &var) -> String { return this->processor(var); });
    else
      request->send_P(200, "text/html", _wp->p404_html, [this](const String &var) -> String { return this->processor(var); });
  });

  _events->onConnect([](AsyncEventSourceClient *client) {
    if (client->lastId())
    {
      Serial.printf("Client reconnected! Last message ID that it got was: %u\n", client->lastId());
    }
    client->send("hello!", "log_event", millis(), 1000);
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
