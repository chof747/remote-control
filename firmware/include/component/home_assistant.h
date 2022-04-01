#ifndef HOME_ASSISTANT_H
#define HOME_ASSISTANT_H

#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <LinkedList.h>

#include "component.h"
#include "websocket/ws_jsonmessage.h"

//short defining web socket message base class

class HomeAssistantClient : public Component
{
    public:

    HomeAssistantClient() : authenticated(false), messageQueue() {}

    bool setup();
    void afterSetup();
    void loop();

    private:

    bool authenticated;
    LinkedList<WebSocketJsonMessage> messageQueue;

    void handleEvent(WStype_t type, uint8_t * payload, size_t length);
    void authenticate();
    bool checkForSpecialPayloads(const char* payload);
    bool parseMessage(const char* payload);
};

extern HomeAssistantClient homeAssistantClient;


#endif //HOME_ASSISTANT_H