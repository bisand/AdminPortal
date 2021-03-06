#include <Arduino.h>

class WebPages
{
    public:
        const char *index_html = 
        #include "index.h"
        ;
        const char *docs_html = 
        #include "docs.h"
        ;
        const char *config_html = 
        #include "config.h"
        ;
        const char *monitor_html = 
        #include "monitor.h"
        ;
        const char *upgrade_html = 
        #include "upgrade.h"
        ;
        const char *style_css = 
        #include "style.h"
        ;
        const char *p404_html = 
        #include "404.h"
        ;
};
