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
, m_modeSwitchIndex(NIL)
, m_selectedInput(NIL)
, m_selectedOutput(NIL)
{
  m_ModeStates[PlayMode] = &Ui::CPlayState::getInstance();
  m_ModeStates[ScanMode] = &Ui::CScanState::getInstance();
  m_ModeStates[RoutingMode] = &Ui::CRoutingState::getInstance();
  m_ModeStates[StoreMode] = &Ui::CStoreState::getInstance();
  m_ModeStates[ReadMode] = &Ui::CReadState::getInstance();
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
  if(m_InputSwitches.getPressed(modeIndex))
  {
    m_State->onInputClick(*this, modeIndex);
  }
  if(m_OutputSwitches.getPressed(modeIndex))
  {
    m_State->onOutputClick(*this, modeIndex);
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
  context.setState(context.determineNextModeState(index));
}
void Ui::CScanState::onEntry(Ui& context) const
{
  context.m_ModeLEDs.clear();
  context.m_ModeLEDs.m_LedArray[0]->set();
}
void Ui::CScanState::onModeClick(Ui& context, int8_t index) const
{
  context.setState(context.determineNextModeState(index));
}

void Ui::CRoutingState::onEntry(Ui& context) const
{
  context.m_ModeLEDs.clear();
  context.m_ModeLEDs.m_LedArray[1]->set();
  context.m_InputLEDs.clear();
  if(context.m_selectedInput != NIL)
    context.m_InputLEDs.m_LedArray[context.m_selectedInput]->set();
}
void Ui::CRoutingState::onExit(Ui& context) const
{
  context.m_InputLEDs.clear();
}
void Ui::CRoutingState::onModeClick(Ui& context, int8_t index) const
{
  context.setState(context.determineNextModeState(index));
}

void Ui::CRoutingState::onInputClick(Ui& context, int8_t index) const
{
  context.m_selectedInput = index;
  context.m_InputLEDs.clear();
  context.m_InputLEDs.m_LedArray[context.m_selectedInput]->set();
}
void Ui::CRoutingState::onOutputClick(Ui& context, int8_t index) const
{
  context.m_OutputLEDs.clear();
  context.m_OutputLEDs.m_LedArray[index]->toggle();
}

void Ui::CStoreState::onEntry(Ui& context) const
{
  context.m_ModeLEDs.clear();
  context.m_ModeLEDs.m_LedArray[2]->set();
}
void Ui::CStoreState::onModeClick(Ui& context, int8_t index) const
{
  context.setState(context.determineNextModeState(index));
}
void Ui::CReadState::onEntry(Ui& context) const
{
  context.m_ModeLEDs.clear();
  context.m_ModeLEDs.m_LedArray[3]->set();
}
void Ui::CReadState::onModeClick(Ui& context, int8_t index) const
{
  context.setState(context.determineNextModeState(index));
}

Ui::IUiState& Ui::determineNextModeState(int8_t modeSwitch)
{
  if(modeSwitch == m_modeSwitchIndex)
  {
    m_modeSwitchIndex = NIL;
    return Ui::CPlayState::getInstance();
  }
  else
  {
    m_modeSwitchIndex = modeSwitch;
    return *m_ModeStates[modeSwitch+1];
  }
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



