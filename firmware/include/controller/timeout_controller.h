#ifndef TIMEOUT_CONTROLLER_H
#define TIMEOUT_CONTROLLER_H

class TimeOutController
{
    public:

        TimeOutController(unsigned long timeout);
        void restartTimeout();
        bool checkTimeForTimeOut();

        void startTimer(unsigned long time);
        bool checkTimeForTimer();


    private:
        unsigned long referenceTimeOut;
        unsigned long referenceTimer;

        /**
         * Timeout in milliseconds
         */
        unsigned long timeout;
        unsigned long timerTime;

};


#endif //TIMEOUT_CONTROLLER_H