#include "Adafruit_MCP23X17.h"

#include "component/button_controls.h"
#include "controller/button_controller.h"
#include "logger.h"

#define MODULE "BUTTONS"

#define DEBOUNCE_TIME 20
#define LONG_PRESS_TIME 500
#define DBL_CLICK_TIME 150

Adafruit_MCP23X17 mcp;

bool ButtonControls::setup()
//*********************************************************************************
{
  controller = NULL;

  mcp.begin_I2C();
  mcp.pinMode(WAKE_UP_WATCH_PIN, OUTPUT);
  mcp.digitalWrite(WAKE_UP_WATCH_PIN, LOW);

  Log.info(MODULE, "Initializing Button Controls ...");

  setupButtons();

  state = 0;  
  eventState = 0;
  eventTime = 0;
  clicks = 0;

  return true;
}

void ButtonControls::loop()
//*********************************************************************************
{
  uint16_t newState = readButtons();
  long t = millis();
  
  if ((newState != state) && (DEBOUNCE_TIME < (t - eventTime)))
  {
    Log.debug(MODULE, "New State detected: %04X (old was %04X) at %d", newState, state, t);
    if (newState > state)
    {
      //button has been pressed start 
      eventTime = t;
      state = newState;
      eventState = state;
    }  
    else if (newState == 0)
    {
      //release
      unsigned long pressTime = t - eventTime;
      Log.debug(MODULE,"Click counter before = %d", clicks);
      clicks += 1;

      if (LONG_PRESS_TIME <= pressTime)
      {
        eventTime = 0;
        longPress = true;
      }
      else
      {
        longPress = false;
        eventTime = (clicks > 1) ? 0 : t;
      }

      state = newState;
      Log.debug(MODULE,"Click counter after = %d", clicks);
    }
    else
    {
      eventTime = t;
    }

  }

  //Log.debug(MODULE, "Time passed since last %02X click = %d", eventState, (millis() - eventTime));

  if ((DBL_CLICK_TIME < (t - eventTime)) && (clicks > 0))
  {
      if (longPress)
      {
        Log.info(MODULE, "\033[32mLong press of button state x%04X\033[39m", eventState);
        if (controller != NULL)
        {
          controller->onLongPress(buttonStates(eventState));
        }
      }
      else if (2 == clicks)
      {
        Log.info(MODULE, "\033[32mDouble Click of button state x%04X\033[39m", eventState);
        if (controller != NULL)
        {
          controller->onDblClick(buttonStates(eventState));
        }
      }
      else 
      {
        Log.info(MODULE, "\033[32mClick of button state x%04X\033[39m", eventState);
        if (controller != NULL)
        {
          controller->onClick(buttonStates(eventState));
        }

      }

      clicks = 0;
      eventTime = 0;
      eventState = 0;
  }
}

void ButtonControls::setController(ButtonController* controller)
//*********************************************************************************
{
  this->controller = controller;
}

void ButtonControls::activateWakeUp()
//*********************************************************************************
{
  mcp.digitalWrite(WAKE_UP_WATCH_PIN, HIGH);
}


uint16_t ButtonControls::readButtons()
//*********************************************************************************
{
  uint16_t btnStates = mcp.readGPIOAB();
  uint16_t value = gButtonMask & ~btnStates; //>> FIRST_BUTTON;
  return value;
}

btnStateType ButtonControls::buttonStates(uint16_t state)
//*********************************************************************************
{
  btnStateType status;
  status.modeButton = state & 1 << MODE_BUTTON;

  for (int i = 0; i< BUTTON_COUNT - 1; i++)
  {
    status.pushButtons[i] = state & 1 << gButtons[i];
  }

  return status;
}

void ButtonControls::setupButtons() 
//*********************************************************************************
{
  for (int i = 0; i < BUTTON_COUNT - 1; i++)
  {
    gButtons[i] = FIRST_BUTTON + i;    
  }

  //mode button comes last:
  gButtons[BUTTON_COUNT - 1] = MODE_BUTTON;

  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    uint16_t b = gButtons[i];
    gButtonMask += 1 << b;
    mcp.pinMode(b, INPUT_PULLUP);
    //mcp.digitalWrite(b, HIGH);
  }

  Log.debug(MODULE, "gButtonMask = %d, %04X", gButtonMask, gButtonMask);
}

ButtonControls buttonControls;
