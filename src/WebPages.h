#include <Arduino.h>

struct WebPages
{
    public:
        String index = "<!DOCTYPE html><html><head><title>Engine Monitor</title></head><body><div><h2>Welcome to EngineMonitor</h2><p>The purpose of this product is to be able to monitor a boat engine</p><p>For information on how to customize the admin interface, please see <a href='https://github.com/bisand/AdminPortal/'>AdminPortal</a> on GitHub</p><p><a href='/docs'>Documentation</a></p><p><a href='/config'>Configuration</a></p><p><a href='/upgrade'>Firmware Upgrade</a></p><p><a href='/monitor'>Debug monitor</a></p></div></body></html>";
        String docs = "<!DOCTYPE html><html><head><title>Engine Monitor</title></head><body><div><h2>Documentation</h2><p>Please see <a href='https://github.com/bisand/AdminPortal/'>AdminPortal</a> on GitHub</p></div></body></html>";
        String config = "<!DOCTYPE html><html><head><title>Engine Monitor</title></head><body><div><h2>Configuration</h2><form action='/config' method='POST'>%CONFIG_FORM%</form></div></body></html>";
        String monitor = "<!DOCTYPE html><html><head><title>Engine Monitor</title></head><body><div><h2>Welcome to EngineMonitor</h2><p>The purpose of this product is to be able to monitor a boat engine</p><p>For information on how to customize the admin interface, please see <a href='https://github.com/bisand/AdminPortal/'>AdminPortal</a> on GitHub</p><p><a href='/docs'>Documentation</a></p><p><a href='/config'>Configuration</a></p><p><a href='/upgrade'>Firmware Upgrade</a></p><p><a href='/monitor'>Debug monitor</a></p></div></body></html>";
        String upgrade = "<!DOCTYPE html><html><head><title>Engine Monitor</title></head><body><div><h2>Welcome to EngineMonitor</h2><p>The purpose of this product is to be able to monitor a boat engine</p><p>For information on how to customize the admin interface, please see <a href='https://github.com/bisand/AdminPortal/'>AdminPortal</a> on GitHub</p><p><a href='/docs'>Documentation</a></p><p><a href='/config'>Configuration</a></p><p><a href='/upgrade'>Firmware Upgrade</a></p><p><a href='/monitor'>Debug monitor</a></p></div></body></html>";
};
