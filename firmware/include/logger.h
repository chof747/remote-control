#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>

class Logger 
{
    public:

        static const int NONE  = 0;
        static const int ERROR = 1;
        static const int WARN  = 2;
        static const int INFO  = 3;
        static const int DEBUG = 4;

        Logger(int logLevel = Logger::NONE);

        void info(const char* component, const char* message,  ... ) {
            va_list args;
            va_start(args, message);
            this->log(Logger::INFO, component, message, args);    
            va_end(args);
        }
        void warn(const char* component, const char* message, ... ) {
            va_list args;
            va_start(args, message);
            this->log(Logger::WARN, component, message, args);    
            va_end(args);
        }
        void error(const char* component, const char* message, ... ) {
            va_list args;
            va_start(args, message);
            this->log(Logger::ERROR, component, message, args);    
            va_end(args);
        }
        void debug(const char* component, const char* message, ... ) {
            va_list args;
            va_start(args, message);
            this->log(Logger::DEBUG, component, message, args);    
            va_end(args);
        }

    protected:

        int logLevel;
        void log(int level, const char* component, const char* message, va_list args );
};

extern Logger Log;

#endif //LOGGER_H