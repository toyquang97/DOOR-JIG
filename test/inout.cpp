
#include "inout.h"

void initInOut()
{
  pinMode(RELAY_CLOSE,OUTPUT);
  pinMode(RELAY_OPEN,OUTPUT);
//   pinMode(BUTTON_MODE,INPUT);
//   pinMode(BUTTON_RESET,INPUT);
//   pinMode(BUTTON_SWITCH,INPUT);
}

void setOutRelay(uint8_t relay, bool level)
{
  digitalWrite(relay,level);
}


switch_e getStatusButton(Button *pin)
{
  bool status;
  pin->listen();
  status = pin->onPressAsToggle();

  if (status)
  {
    return OPEN;
  }
  else
    return CLOSE;
}

mode_e getMode(Button *pin)
{
  static bool mode;
  pin->listen();

  mode = pin->onPressAsToggle();
  if (mode)
  {
    return AUTO;
  }
  else
    return MANUAL;

}

bool getResetState(Button *pin)
{
  bool mode;
  pin->listen();

  mode = pin->onDoubleClick();
  if (mode)
  {
    return 1;
  }
  else
    return 0;

}
