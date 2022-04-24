#include "a.h"
#include "inout.h"
#include <TimerOne.h>

mode_e mode;
switch_e actionOpenClose;
bool resetMode;
uint32_t count = 0;
Button buttonMode = Button(BUTTON_MODE, LOW);
Button buttonSwitch = Button(BUTTON_SWITCH, LOW);
Button buttonReset = Button(BUTTON_RESET, LOW);
int myDoubleClickDelay = 500; // 400 is the default
int a;
void setup()
{
  // put your setup code here, to run once:
  pinMode(cs_Pin, OUTPUT);
  pinMode(data_Pin, OUTPUT);
  pinMode(clock_Pin, OUTPUT);

  initInOut();
  // buttonReset.setDoubleClickDelay(myDoubleClickDelay);
  Timer1.initialize(5000000);
  Timer1.attachInterrupt(blink);
  max_init(0X03);

  Serial.begin(9600);

  write_char('D', 4);
  write_char('o', 3);
  write_char('o', 2);
  write_char('r', 1);
  delay(1000);
  max_clear();
}

void loop()
{
  // put your main code here, to run repeatedly:
  // buttonMode.listen();
  mode            = getMode(&buttonMode);
  actionOpenClose = getStatusButton(&buttonSwitch);
  resetMode       = getResetState(&buttonReset);

  switch (mode)
  {
  case AUTO:
    setOutRelay(RELAY_CLOSE,OFF);
    setOutRelay(RELAY_OPEN,OFF);
    break;
  case MANUAL:
    switch (actionOpenClose)
    {
    case OPEN:
      setOutRelay(RELAY_CLOSE,ON);
      setOutRelay(RELAY_OPEN,OFF);
      //scroll_string((uint8_t *)" OPEN " ,1,left);
      break;
    case CLOSE:
    // max_clear();
      setOutRelay(RELAY_CLOSE,OFF);
      setOutRelay(RELAY_OPEN,ON);
      //scroll_string((uint8_t *)" CLOSE " ,1,left);
      break;
    }
    break;

  default:
    break;
  }
}

void blink()
{
  // scroll_string((uint8_t *)"12345" ,1,left);
}
