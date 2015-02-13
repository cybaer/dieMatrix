/*
 * ui.h
 *
 *  Created on: 27.01.2015
 *      Author: cybaer
 */

#include "avrlib/spi.h"
#include "avrlib/gpio.h"
#include "devices.h"

using namespace avrlib;

typedef SpiMaster<NumberedGpio<4>, MSB_FIRST, 4> spi_master;
typedef MCP23S17<spi_master, 3> portExtender1;
typedef MCP23S17<spi_master, 1> portExtender2;

static const int8_t INPUT_COUNT = 8;

LED<portExtender1, PORT_B, 7> LED_In1;
LED<portExtender1, PORT_B, 6> LED_In2;
LED<portExtender1, PORT_B, 5> LED_In3;
LED<portExtender1, PORT_B, 4> LED_In4;
LED<portExtender1, PORT_B, 3> LED_In5;
LED<portExtender1, PORT_B, 2> LED_In6;
LED<portExtender1, PORT_B, 1> LED_In7;
LED<portExtender1, PORT_B, 0> LED_In8;
static LEDArray InputLEDs(INPUT_COUNT, &LED_In1, &LED_In2, &LED_In3, &LED_In4, &LED_In5, &LED_In6, &LED_In7, &LED_In8);

Switch<portExtender1, PORT_A, 0> SW_In1;
Switch<portExtender1, PORT_A, 1> SW_In2;
Switch<portExtender1, PORT_A, 2> SW_In3;
Switch<portExtender1, PORT_A, 3> SW_In4;
Switch<portExtender1, PORT_A, 4> SW_In5;
Switch<portExtender1, PORT_A, 5> SW_In6;
Switch<portExtender1, PORT_A, 6> SW_In7;
Switch<portExtender1, PORT_A, 7> SW_In8;
static SwitchArray InputSwitches(INPUT_COUNT, &SW_In1, &SW_In2, &SW_In3, &SW_In4, &SW_In5, &SW_In6, &SW_In7, &SW_In8);

LED<portExtender2, PORT_B, 0> LED_M1;
LED<portExtender2, PORT_B, 1> LED_M2;
LED<portExtender2, PORT_B, 2> LED_M3;
LED<portExtender2, PORT_B, 3> LED_M4;
static LEDArray _4LEDs(4, &LED_M1, &LED_M2, &LED_M3, &LED_M4);

Switch<portExtender2, PORT_A, 7> SW_M1;
Switch<portExtender2, PORT_A, 6> SW_M2;
Switch<portExtender2, PORT_A, 5> SW_M3;
Switch<portExtender2, PORT_A, 4> SW_M4;

static SwitchArray _4Switches(4, &SW_M1, &SW_M2, &SW_M3, &SW_M4);

class Ui
{
public:
  void Init(void)
  {
    InputSwitches.init();
    InputLEDs.init();
    _4Switches.init();
    _4LEDs.init();
    portExtender1::Init();
    portExtender2::Init();
  }

  void Poll(void)
  {
    portExtender1::ReadIO();
    portExtender2::ReadIO();
    InputSwitches.refresh();
    _4Switches.refresh();
  }

  void Do(void)
  {
    for(int8_t i=0; i<INPUT_COUNT; i++)
    {
      if(InputSwitches.m_SwArray[i]->active())
        InputLEDs.m_LedArray[i]->set();
      else
        InputLEDs.m_LedArray[i]->clear();
    }
    if(SW_M1.active())
      LED_M1.set();
    else
      LED_M1.clear();

    if(SW_M4.active())
      LED_M2.set();
    else
      LED_M2.clear();

    portExtender1::WriteIO();
    portExtender2::WriteIO();
  }

private:

};

static Ui ui;

