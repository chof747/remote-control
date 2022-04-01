#ifndef WS_MESSAGE_H
#define WS_MESSAGE_H

enum WSMessageAnswer { NOT_RELEVANT = 0, ACCEPTED = 1, DENIED = 9};

class WebSocketMessage 
{
    public:

    WebSocketMessage(): messageId(0) {} 

    WebSocketMessage* setId(unsigned int);

    virtual const char* provideSendMessage() = 0;
    virtual WSMessageAnswer handleAnswer(const char*) = 0;

    protected:

    unsigned int messageId;
};

#endif //W_MESSAGE_H