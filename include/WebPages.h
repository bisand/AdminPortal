#include <Arduino.h>

struct WebPages
{
    public:
        const char *index = 
        #include "index.h"
        ;
        const char *docs = 
        #include "docs.h"
        ;
        const char *config = 
        #include "config.h"
        ;
        const char *monitor = 
        #include "monitor.h"
        ;
        const char *upgrade = 
        #include "upgrade.h"
        ;
};
