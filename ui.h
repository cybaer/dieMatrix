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

static const uint8_t SPI_Speed = 4;
typedef SpiMaster<NumberedGpio<4>, MSB_FIRST, SPI_Speed> spi_master;
typedef MCP23S17<spi_master, 1> portExtender1;
typedef MCP23S17<spi_master, 3> portExtender2;
typedef MCP23S17<spi_master, 7> portExtender3;


static const int8_t INPUT_COUNT = 10;

LED<portExtender1, PORT_B, 7> LED_In1;
LED<portExtender1, PORT_B, 6> LED_In2;
LED<portExtender1, PORT_B, 5> LED_In3;
LED<portExtender1, PORT_B, 4> LED_In4;
LED<portExtender1, PORT_B, 3> LED_In5;
LED<portExtender1, PORT_B, 2> LED_In6;
LED<portExtender1, PORT_B, 1> LED_In7;
LED<portExtender1, PORT_B, 0> LED_In8;
LED<portExtender2, PORT_B, 7> LED_In9;
LED<portExtender2, PORT_B, 6> LED_In10;
static LEDArray InputLEDs(INPUT_COUNT, &LED_In1, &LED_In2, &LED_In3, &LED_In4, &LED_In5, &LED_In6, &LED_In7, &LED_In8, &LED_In9, &LED_In10);

Switch<portExtender1, PORT_A, 0> SW_In1;
Switch<portExtender1, PORT_A, 1> SW_In2;
Switch<portExtender1, PORT_A, 2> SW_In3;
Switch<portExtender1, PORT_A, 3> SW_In4;
Switch<portExtender1, PORT_A, 4> SW_In5;
Switch<portExtender1, PORT_A, 5> SW_In6;
Switch<portExtender1, PORT_A, 6> SW_In7;
Switch<portExtender1, PORT_A, 7> SW_In8;
Switch<portExtender2, PORT_A, 0> SW_In9;
Switch<portExtender2, PORT_A, 1> SW_In10;
static SwitchArray InputSwitches(INPUT_COUNT, &SW_In1, &SW_In2, &SW_In3, &SW_In4, &SW_In5, &SW_In6, &SW_In7, &SW_In8, &SW_In9, &SW_In10);

static const int8_t OUTPUT_COUNT = 10;

LED<portExtender2, PORT_B, 5> LED_Out1;
LED<portExtender2, PORT_B, 4> LED_Out2;
LED<portExtender2, PORT_B, 3> LED_Out3;
LED<portExtender2, PORT_B, 2> LED_Out4;
LED<portExtender2, PORT_B, 1> LED_Out5;
LED<portExtender2, PORT_B, 0> LED_Out6;
LED<portExtender3, PORT_B, 7> LED_Out7;
LED<portExtender3, PORT_B, 6> LED_Out8;
LED<portExtender3, PORT_B, 5> LED_Out9;
LED<portExtender3, PORT_B, 4> LED_Out10;
static LEDArray OutputLEDs(OUTPUT_COUNT, &LED_Out1, &LED_Out2, &LED_Out3, &LED_Out4, &LED_Out5, &LED_Out6, &LED_Out7, &LED_Out8, &LED_Out9, &LED_Out10);

Switch<portExtender2, PORT_A, 2> SW_Out1;
Switch<portExtender2, PORT_A, 3> SW_Out2;
Switch<portExtender2, PORT_A, 4> SW_Out3;
Switch<portExtender2, PORT_A, 5> SW_Out4;
Switch<portExtender2, PORT_A, 6> SW_Out5;
Switch<portExtender2, PORT_A, 7> SW_Out6;
Switch<portExtender3, PORT_A, 0> SW_Out7;
Switch<portExtender3, PORT_A, 1> SW_Out8;
Switch<portExtender3, PORT_A, 2> SW_Out9;
Switch<portExtender3, PORT_A, 3> SW_Out10;
static SwitchArray OutputSwitches(OUTPUT_COUNT, &SW_Out1, &SW_Out2, &SW_Out3, &SW_Out4, &SW_Out5, &SW_Out6, &SW_Out7, &SW_Out8, &SW_Out9, &SW_Out10);


LED<portExtender3, PORT_B, 3> LED_M1;
LED<portExtender3, PORT_B, 2> LED_M2;
LED<portExtender3, PORT_B, 1> LED_M3;
LED<portExtender3, PORT_B, 0> LED_M4;
static LEDArray ModeLEDs(4, &LED_M1, &LED_M2, &LED_M3, &LED_M4);

Switch<portExtender3, PORT_A, 4> SW_M1;
Switch<portExtender3, PORT_A, 5> SW_M2;
Switch<portExtender3, PORT_A, 6> SW_M3;
Switch<portExtender3, PORT_A, 7> SW_M4;

static SwitchArray ModeSwitches(4, &SW_M1, &SW_M2, &SW_M3, &SW_M4);

class Ui
{
public:
  void Init(void)
  {
    InputSwitches.init();
    InputLEDs.init();
    OutputSwitches.init();
    OutputLEDs.init();
    ModeSwitches.init();
    ModeLEDs.init();
    portExtender1::Init();
    portExtender2::Init();
    portExtender3::Init();
  }

  void Poll(void)
  {
    portExtender1::ReadIO();
    portExtender2::ReadIO();
    portExtender3::ReadIO();
    InputSwitches.refresh();
    OutputSwitches.refresh();
    ModeSwitches.refresh();
  }

  void Do(void)
  {
    for(int8_t i=0; i<InputSwitches.m_count; i++)
    {
      if(InputSwitches.m_SwArray[i]->active())
        InputLEDs.m_LedArray[i]->clear();
      else
        InputLEDs.m_LedArray[i]->set();
    }
    for(int8_t i=0; i<OutputSwitches.m_count; i++)
    {
      if(OutputSwitches.m_SwArray[i]->active())
        OutputLEDs.m_LedArray[i]->clear();
      else
        OutputLEDs.m_LedArray[i]->set();
    }
    for(int8_t i=0; i<ModeSwitches.m_count; i++)
    {
      if(ModeSwitches.m_SwArray[i]->active())
        ModeLEDs.m_LedArray[i]->clear();
      else
        ModeLEDs.m_LedArray[i]->set();
    }
    portExtender1::WriteIO();
    portExtender2::WriteIO();
    portExtender3::WriteIO();
  }

private:

};

static Ui ui;

