/*
 * ui.h
 *
 *  Created on: 27.01.2015
 *      Author: cybaer
 */
// __Compiler Bug__
__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);
extern "C" void __cxa_pure_virtual();

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

  class IUiState
  {
  public:
    //** Empty Constructor */
    IUiState(void)
    {
    }

    virtual void onEntry(Ui&) const
    {
    }
    virtual void onExit(Ui&) const
    {
    }
    virtual void onModeClick(Ui&, int8_t index) const
    {
    }
    //virtual void onLongClick(Ui&) const {}

  private:
    /** Forbidden */
    DISALLOW_COPY_AND_ASSIGN (IUiState);
  };

  /**
   * State
   */
  class CPlayState: public IUiState
  {
  public:
    static CPlayState& getInstance(void)
    {
      static CPlayState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onModeClick(Ui&, int8_t index) const;
  };

  class CScanState: public IUiState
  {
  public:
    static CScanState& getInstance(void)
    {
      static CScanState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onModeClick(Ui&, int8_t index) const;
  };

  void setState(IUiState& state)
  {
    m_State->onExit(*this);
    m_State = &state;
    m_State->onEntry(*this);
  }

  /** The internal State of this thing */
  IUiState* m_State;
};

static Ui ui;

