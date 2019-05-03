/*
 * OTAWebUpdater.ino Example from ArduinoOTA Library
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

#include "AdminPortal.h"

AdminPortal::AdminPortal()
{
#ifdef ESP8266
  _webServer = new ESP8266WebServer(80);
#else
  _webServer = new AsyncWebServer(80);
#endif
  _apIP = new IPAddress(192, 168, 4, 1);
  _ssid = (char *)"EngineMonitor";
  _host = (char *)"EngineMonitor";
  _password = (char *)"Password123";
  _config = new Config();
}

AdminPortal::~AdminPortal()
{
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

void AdminPortal::onNotFound(AsyncWebServerRequest *request)
{
  request->send(SPIFFS, "/404.html", String(), false, processor);
}

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
void AdminPortal::readConfigFile()
{
  if (SPIFFS.begin())
  {
    if(isDebug) Serial.println("mounted file system");
    if (SPIFFS.exists("/cfg.json"))
    {
      //file exists, reading and loading
      if(isDebug) Serial.println("reading config file");
      File configFile = SPIFFS.open("/cfg.json", "r");
      if (configFile)
      {
        if(isDebug) Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        if(isDebug) Serial.println(buf.get());
        configFile.readBytes(buf.get(), size);

        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, configFile);
        if (!error)
        {
          if(isDebug) Serial.println("\nparsed json");

          _config->flow_mlpp_in = doc["flow_mlpp_in"];
          _config->flow_mlpp_out = doc["flow_mlpp_out"];
          _config->flow_moving_avg = doc["flow_moving_avg"];
        }
        else
        {
          if(isDebug) Serial.println("failed to load json config");
        }
        configFile.close();
      }
    }
  }
  else
  {
    if(isDebug) Serial.println("failed to mount FS");
  }
  //end read
}

// Save config file.
void AdminPortal::writeConfigFile()
{
  if(isDebug) Serial.println("saving config");
  DynamicJsonDocument doc(1024);
  doc["flow_mlpp_in"] = _config->flow_mlpp_in;
  doc["flow_mlpp_out"] = _config->flow_mlpp_out;
  doc["flow_moving_avg"] = _config->flow_moving_avg;

  File configFile = SPIFFS.open("/cfg.json", "w");
  if (!configFile)
  {
    if(isDebug) Serial.println("failed to open config file for writing");
  }

  // Serialize JSON to file
  if (serializeJson(doc, configFile) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  configFile.close();
  //end save
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
  Serial.begin(115200);

  WiFi.softAPConfig(*_apIP, *_apIP, IPAddress(255, 25, 255, 0));
  WiFi.softAP(_ssid, _password);

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(_ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(_host))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  // Display landing page.
  _webServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  _webServer->on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  _webServer->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  _webServer->on("/docs", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/docs.html", String(), false, processor);
  });

  // Display configuration page.
  _webServer->on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(www_username, www_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/config.html", String(), false, processor);
  });

  // Display firmware upgrade page.
  _webServer->on("/upgrade", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!request->authenticate(www_username, www_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/upgrade.html", String(), false, processor);
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

  _webServer->begin();
}

void AdminPortal::loop(void)
{
  if (millis() - _currMillis > _interval)
  {
    _currMillis = millis();
  }
}
