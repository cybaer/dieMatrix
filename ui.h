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


typedef Gpio<PortB, 0> Debug1;
typedef Gpio<PortB, 1> Debug2;

static const int8_t INPUT_COUNT = 10;
static const int8_t OUTPUT_COUNT = 10;
enum {PlayMode = 0, ScanMode, RoutingMode, StoreMode, ReadMode, ModeCount};

class Ui
{
public:
  Ui(void);
  void Init(void);
  void Poll(void);
  void Do(void);

private:
  void testSwitchLED(void);
  bool isScanMode(void);

  class IUiState;
  IUiState& determineNextModeState(int8_t modeSwitch);

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
    IUiState(void) {}
    virtual void onEntry(Ui&) const {}
    virtual void onExit(Ui&) const {}
    virtual void onModeClick(Ui&, int8_t index) const {}
    virtual void onInputClick(Ui&, int8_t index) const {}
    virtual void onOutputClick(Ui&, int8_t index) const {}

    //virtual void onLongClick(Ui&) const {}

  private:
    /** Forbidden */
    DISALLOW_COPY_AND_ASSIGN (IUiState);
  };

  /**
   * State machine
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
    virtual void onInputClick(Ui&, int8_t index) const;
    virtual void onOutputClick(Ui&, int8_t index) const;
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
    virtual void onExit(Ui&) const;
    virtual void onModeClick(Ui&, int8_t index) const;
  };
  class CRoutingState: public IUiState
  {
  public:
    static CRoutingState& getInstance(void)
    {
      static CRoutingState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onModeClick(Ui&, int8_t index) const;
    virtual void onInputClick(Ui&, int8_t index) const;
    virtual void onOutputClick(Ui&, int8_t index) const;
  };

  class CRoutingByOutState: public IUiState
  {
  public:
    static CRoutingByOutState& getInstance(void)
    {
      static CRoutingByOutState s_instance;
      return s_instance;
    }
    virtual void onExit(Ui&) const;
    virtual void onModeClick(Ui&, int8_t index) const;
    virtual void onInputClick(Ui&, int8_t index) const;
    //virtual void onOutputClick(Ui&, int8_t index) const;
  };

  class CRoutingByInState: public IUiState
    {
    public:
      static CRoutingByInState& getInstance(void)
      {
        static CRoutingByInState s_instance;
        return s_instance;
      }
      virtual void onExit(Ui&) const;
      virtual void onModeClick(Ui&, int8_t index) const;
      virtual void onOutputClick(Ui&, int8_t index) const;
    };

  class CStoreState: public IUiState
  {
  public:
    static CStoreState& getInstance(void)
    {
      static CStoreState s_instance;
      return s_instance;
    }
    virtual void onEntry(Ui&) const;
    virtual void onModeClick(Ui&, int8_t index) const;
  };
  class CReadState: public IUiState
  {
  public:
    static CReadState& getInstance(void)
    {
      static CReadState s_instance;
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

  IUiState* m_State;
  int8_t m_modeSwitchIndex;
  IUiState* m_ModeStates[ModeCount];
  int8_t m_selectedInput;
  int8_t m_selectedOutput;
};

static Ui ui;

