#include <Arduino.h>

struct WebPages
{
    public:
        const char *index = "<!DOCTYPE html><html><head><title>Engine Monitor</title></head><body><div><h2>Welcome to EngineMonitor</h2><p>The purpose of this product is to be able to monitor a boat engine</p><p>For information on how to customize the admin interface, please see <a href='https://github.com/bisand/AdminPortal/'>AdminPortal</a> on GitHub</p><p><a href='/docs'>Documentation</a></p><p><a href='/config'>Configuration</a></p><p><a href='/upgrade'>Firmware Upgrade</a></p><p><a href='/monitor'>Debug monitor</a></p></div></body></html>";
        const char *docs = "<!DOCTYPE html><html><head><title>Engine Monitor</title></head><body><div><h2>Documentation</h2><p>Please see <a href='https://github.com/bisand/AdminPortal/'>AdminPortal</a> on GitHub</p></div></body></html>";
        const char *config = "<!DOCTYPE html><html><head><title>Engine Monitor</title></head><body><div><h2>Configuration</h2><form action='/config' method='POST'>%CONFIG_FORM%</form></div></body></html>";
        const char *monitor = "<!DOCTYPE html><html><head><title>Engine Monitor</title></head><body><h2>Debug Monitor</h2><p><button id='clrButton'>Clear List</button></p><p><ol id='debugList'></ol></p><script>var eventList = document.querySelector('#debugList');var button = document.querySelector('#clrButton');button.onclick = function () {eventList.innerHTML = "";};if (!!window.EventSource) {var source = new EventSource('/log_events');source.addEventListener('log_event', function (e) {var newElement = document.createElement('li');newElement.textContent = e.data;eventList.appendChild(newElement);}, false);}</script></body></html>";
        const char *upgrade = "<!DOCTYPE html><html><head><title>Engine Monitor</title></head><body><div><h2>Upgrade firmware</h2><p>Please see <a href='https://github.com/bisand/AdminPortal/'>AdminPortal</a> on GitHub</p></div></body></html>";
};
