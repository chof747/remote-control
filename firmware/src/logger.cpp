#include <Arduino.h>
#include "logger.h"

const char *LEVEL_LABELS[5] = { "NONE", "ERROR", "WARNING", "INFO", "DEBUG"};

Logger::Logger(int logLevel)
//****************************************************************************************
{
    this->logLevel = logLevel;
    if (Logger::NONE != logLevel)
    {
        Serial.begin(115200); 
    }
}

void Logger::log(int level, const char* component, const char* message, va_list args)
//****************************************************************************************
{
    if (Logger::NONE != this->logLevel)
    {   
        //Serial.printf("Log-level: %d, given: %d\n", this->logLevel, level);
        if (this->logLevel >= level)
        {
            char buffer[255];
            vsnprintf(buffer,254,message, args);
            Serial.printf("[%-7s][%-10s] %s\n", LEVEL_LABELS[level], component, buffer);
        }
    }
}

#ifdef LOG_LEVEL
  Logger Log(LOG_LEVEL);
 #else
  Logger Log(Logger::NONE);
#endif
