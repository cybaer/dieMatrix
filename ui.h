/*
 * ui.h
 *
 *  Created on: 27.01.2015
 *      Author: cybaer
 */

#include "devices.h"

using namespace avrlib;

static const int8_t INPUT_COUNT = 10;
static const int8_t OUTPUT_COUNT = 10;

class Ui
{
public:
  Ui(void);
  void Init(void);
  void Poll(void);
  void Do(void);

private:
  void testSwitchLED(void);

  LEDArray m_InputLEDs;
  SwitchArray m_InputSwitches;
  LEDArray m_OutputLEDs;
  SwitchArray m_OutputSwitches;
  LEDArray m_ModeLEDs;
  SwitchArray m_ModeSwitches;
};

static Ui ui;

