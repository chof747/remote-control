#include "component/websocket/ws_jsonmessage.h"
#include <cstddef>

const char* WebSocketJsonMessage::provideSendMessage()
//****************************************************************************************
{
    return NULL;
}

WSMessageAnswer WebSocketJsonMessage::handleAnswer(const char*)
//****************************************************************************************
{
    return WSMessageAnswer::DENIED;
}