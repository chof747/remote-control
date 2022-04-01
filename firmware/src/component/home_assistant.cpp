#include "component/home_assistant.h"

#include "logger.h"
#include "config.h"

#define MODULE "WEBSOCKET"

WebSocketsClient webSocket;

#define HASS_AUTH_REQUEST_NEEDLE "\"type\": \"auth_required\""
#define HASS_AUTHORIZED_NEEDLE "\"type\": \"auth_ok\""
#define HASS_AUTHORIZATION_MSG "{ \"type\" : \"auth\", \"access_token\" : \"" HASS_API_KEY "\" }"

bool HomeAssistantClient::setup()
//****************************************************************************************
{
    webSocket.begin(HASS_SERVER, HASS_PORT, "/api/websocket");
    webSocket.onEvent(std::bind(&HomeAssistantClient::handleEvent,
                                this,
                                std::placeholders::_1,
                                std::placeholders::_2,
                                std::placeholders::_3));
    return true;
}

void HomeAssistantClient::afterSetup()
//****************************************************************************************
{
}

void HomeAssistantClient::loop()
//****************************************************************************************
{
    webSocket.loop();
}

void HomeAssistantClient::handleEvent(WStype_t type, uint8_t *payload, size_t length)
//****************************************************************************************
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Log.warn(MODULE, "Disconnected!");
        break;

    case WStype_CONNECTED:
        Log.info(MODULE, "Connected to url: %s", payload);
        break;

    case WStype_TEXT:
        Log.debug(MODULE, "Message Received payload: '%s'", payload);
        if (!checkForSpecialPayloads((const char*) payload))
        {
            parseMessage((const char*) payload);
        }
        break;

    case WStype_BIN:
        Log.info(MODULE, "Binary Message Received with length: %u", length);
        hexdump(payload, length);
        break;

    case WStype_PING:
        Log.info(MODULE, "Got ping");
        break;
    case WStype_PONG:
        Log.info(MODULE, "Got pong");
        break;

    default:
        Log.warn(MODULE, "Unknown message type received: %d -  '%s'", type, payload);
    }
}

bool HomeAssistantClient::checkForSpecialPayloads(const char* payload)
//****************************************************************************************
{
    if (strstr(payload, HASS_AUTH_REQUEST_NEEDLE) != NULL)
    {
        authenticated = false;
        authenticate();
        return true;
    }
    else if (strstr(payload, HASS_AUTHORIZED_NEEDLE) != NULL)
    {
        authenticated = true;
        Log.info(MODULE, "Authenticated successfully at Home Assistant Server");
        return true;
    }

    return false;
}

bool HomeAssistantClient::parseMessage(const char* payload)
//****************************************************************************************
{
    return true;
}

void HomeAssistantClient::authenticate()
//****************************************************************************************
{
    Log.debug(MODULE, "Received authorization message");
    webSocket.sendTXT(HASS_AUTHORIZATION_MSG);
}

HomeAssistantClient homeAssistantClient;