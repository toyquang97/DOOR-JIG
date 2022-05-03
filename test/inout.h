//#include "main.h"

#ifndef _INOUT_H
#define _INOUT_H

#include "stdbool.h"
#include "stdint.h"
#include "Arduino.h"
#include <Button.h>

// change the max7219 PORT and Pins below

#define RELAY_CLOSE 7
#define RELAY_OPEN 6

#define BUTTON_MODE A0
#define BUTTON_SWITCH 5
#define BUTTON_RESET 2

#define ON true
#define OFF false


typedef enum
{
  OPEN,CLOSE
}switch_e;

typedef enum
{
  AUTO,MANUAL
}mode_e;

typedef enum
{
  OPENING,CLOSING,OPENED,CLOSED
}state_e;

// #define bitRead(value, bit) (((value) >> (bit)) & 0x01)
// #define bitSet(value, bit) ((value) |= (1UL << (bit)))
// #define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
// #define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit_select(B, i) ((B >> i) & 1)


void initInOut();

void setOutRelay(uint8_t relay, bool level);  

switch_e getStatusButton(Button *pin);
mode_e getMode(Button *pin);
bool getResetState(Button *pin);

#endif
