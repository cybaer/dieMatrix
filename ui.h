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

typedef MCP23S17<spi_master, 1> portExtender2;

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
    _4Switches.init();
    _4LEDs.init();
    portExtender2::Init();
  }

  void Poll(void)
  {
    portExtender2::ReadIO();
    _4Switches.refresh();
  }

  void Do(void)
  {
    if(SW_M1.active())
      LED_M1.set();
    else
      LED_M1.clear();

    if(SW_M4.active())
      LED_M2.set();
    else
      LED_M2.clear();

    portExtender2::WriteIO();
  }

private:

};

static Ui ui;

