#include "component/websocket/ws_message.h"


WebSocketMessage* WebSocketMessage::setId(unsigned int id)
//****************************************************************************************
{
    messageId = id;
    return this;
}