#ifndef WS_JSONMESSAGE_H
#define WS_JSONMESSAGE_H

#include "ws_message.h"

class WebSocketJsonMessage : public WebSocketMessage
//****************************************************************************************
{
    virtual const char* provideSendMessage();
    virtual WSMessageAnswer handleAnswer(const char*);
};


#endif //WS_JSONMESSAGE_H