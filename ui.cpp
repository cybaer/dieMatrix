/*
 * ui.cpp
 *
 *  Created on: 31.03.2015
 *      Author: cybaer
 */
#include "hardware.h"
#include "ui.h"


Ui::Ui(void)
: m_InputLEDs(INPUT_COUNT, &LED_In1, &LED_In2, &LED_In3, &LED_In4, &LED_In5, &LED_In6, &LED_In7, &LED_In8, &LED_In9, &LED_In10)
, m_InputSwitches(INPUT_COUNT, &SW_In1, &SW_In2, &SW_In3, &SW_In4, &SW_In5, &SW_In6, &SW_In7, &SW_In8, &SW_In9, &SW_In10)
, m_OutputLEDs(OUTPUT_COUNT, &LED_Out1, &LED_Out2, &LED_Out3, &LED_Out4, &LED_Out5, &LED_Out6, &LED_Out7, &LED_Out8, &LED_Out9, &LED_Out10)
, m_OutputSwitches(OUTPUT_COUNT, &SW_Out1, &SW_Out2, &SW_Out3, &SW_Out4, &SW_Out5, &SW_Out6, &SW_Out7, &SW_Out8, &SW_Out9, &SW_Out10)
, m_ModeLEDs(4, &LED_M1, &LED_M2, &LED_M3, &LED_M4)
, m_ModeSwitches(4, &SW_M1, &SW_M2, &SW_M3, &SW_M4)
, m_State(&Ui::CPlayState::getInstance())
{

}

void Ui::Init(void)
{
  m_InputSwitches.init();
  m_InputLEDs.init();
  m_OutputSwitches.init();
  m_OutputLEDs.init();
  m_ModeSwitches.init();
  m_ModeLEDs.init();
  portExtender1::Init();
  portExtender2::Init();
  portExtender3::Init();
}

void Ui::Poll(void)
{
  portExtender1::ReadIO();
  portExtender2::ReadIO();
  portExtender3::ReadIO();
  m_InputSwitches.refresh();
  m_OutputSwitches.refresh();
  m_ModeSwitches.refresh();
}
void Ui::Do(void)
{
  //testSwitchLED();
  int8_t modeIndex = 0;
  if(m_ModeSwitches.getPressed(modeIndex))
  {
    m_State->onModeClick(*this, modeIndex);
  }
  portExtender1::WriteIO();
  portExtender2::WriteIO();
  portExtender3::WriteIO();
}


/***** StateMachine *****/

void Ui::CPlayState::onEntry(Ui& context) const
{
  context.m_ModeLEDs.clear();
}
void Ui::CPlayState::onModeClick(Ui& context, int8_t index) const
{
  if(index == 0)
    context.setState(Ui::CScanState::getInstance());
}
void Ui::CScanState::onEntry(Ui& context) const
{
  context.m_ModeLEDs.clear();
  context.m_ModeLEDs.m_LedArray[0]->set();
}
void Ui::CScanState::onModeClick(Ui& context, int8_t index) const
{
  if(index == 0)
      context.setState(Ui::CPlayState::getInstance());
}




void Ui::testSwitchLED(void)
{
  for (int8_t i = 0; i < m_InputSwitches.m_count; i++)
  {
    if (m_InputSwitches.m_SwArray[i]->active())
      m_InputLEDs.m_LedArray[i]->clear();
    else
      m_InputLEDs.m_LedArray[i]->set();
  }
  for (int8_t i = 0; i < m_OutputSwitches.m_count; i++)
  {
    if (m_OutputSwitches.m_SwArray[i]->active())
      m_OutputLEDs.m_LedArray[i]->clear();
    else
      m_OutputLEDs.m_LedArray[i]->set();
  }
  for (int8_t i = 0; i < m_ModeSwitches.m_count; i++)
  {
    if (m_ModeSwitches.m_SwArray[i]->active())
      m_ModeLEDs.m_LedArray[i]->clear();
    else
      m_ModeLEDs.m_LedArray[i]->set();
  }
}



