#include "a.h"
#include "inout.h"
#include <TimerOne.h>
#include <string.h>
mode_e mode;
switch_e actionOpenClose;
static state_e stateActive;
bool resetMode;
bool time5s;
const char openMes[9] = " OPEN ";
const char closeMes[9] = "CLOSE";
const char closingMes[9] = "CLOSING";
const char openingMes[9] = "OPENING";
const char autoMes[9] = " AUTO ";

uint32_t count = 0;
char showMess[10] = "";
 static uint32_t timeOut;
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
  Timer1.initialize(5000);
  Timer1.attachInterrupt(blink);
  max_init(0X07);
  stateActive = OPENING;
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

  switch (mode)
  {
  case AUTO:
    
    max_clear();
    setOutRelay(RELAY_CLOSE, OFF);
    setOutRelay(RELAY_OPEN, OFF);
    
    Serial.println("hihi");
     switch (stateActive)
     {
     case OPENING:
       setOutRelay(RELAY_CLOSE, OFF);
       setOutRelay(RELAY_OPEN, ON);
       memcpy(showMess, openingMes, 9);
       scroll_string((uint8_t *)showMess, 1, left);
       if (time5s)
       {
         stateActive = OPENED;
         time5s = 0;
       }
       break;
     case OPENED:
       
       count++;
       sprintf(showMess, "%ld", count);
       scroll_string((uint8_t *)showMess, 1, left);
       delay(1000);
       stateActive = CLOSING;
       break;
     case CLOSING:
     Serial.println("closeing");
       setOutRelay(RELAY_CLOSE, ON);
       setOutRelay(RELAY_OPEN, OFF);
       memcpy(showMess, closingMes, 9);
       scroll_string((uint8_t *)showMess, 1, left);
       if (time5s)
       {
         stateActive = CLOSED;
         time5s = 0;
       }
       break;
     case CLOSED:
      Serial.println("cloesd");
       
       sprintf(showMess, "%ld", count);
       scroll_string((uint8_t *)showMess, 1, left);
       delay(1000);
       stateActive = OPENING;
       break;
     default:
       break;
     }

    break;
  case MANUAL:

    switch (actionOpenClose)
    {
    case OPEN:
      max_clear();
      memcpy(showMess, openMes, 9);
      setOutRelay(RELAY_CLOSE, OFF);
      setOutRelay(RELAY_OPEN, ON);
      scroll_string((uint8_t *)showMess, 1, left);
      break;
    case CLOSE:
      max_clear();
       memcpy(showMess, closeMes, 9);
       setOutRelay(RELAY_CLOSE,ON);
       setOutRelay(RELAY_OPEN,OFF);
       scroll_string((uint8_t *)showMess ,1,left);
      break;
    }
    break;

  default:
    break;
  }
}

void blink()
{

  timeOut++;
  if ((timeOut % 1000) == 0) // 5S
  {
    time5s = 1;
  }

  mode = getMode(&buttonMode);
  actionOpenClose = getStatusButton(&buttonSwitch);
  resetMode = getResetState(&buttonReset);

  if(resetMode)
    count = 0;
}
