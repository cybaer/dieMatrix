/*
 * ui.h
 *
 *  Created on: 27.01.2015
 *      Author: cybaer
 */

#include "avrlib/spi.h"
#include "avrlib/gpio.h"
#include "mcp23s17.h"
#include "avrlib/devices/switch.h"

using namespace avrlib;

typedef SpiMaster<NumberedGpio<4>, MSB_FIRST, 4> spi_master;

typedef MCP23S17<spi_master, 1> portExtender2;
//MCP23S17<spi_master, 1> pE2;

//typedef PortX<portExtender2, PORT_B> PortExt2_Out;
//typedef PortX<portExtender2, PORT_A> PortExt2_In;



typedef PortPin<portExtender2, PORT_B, 0> LED_1;
typedef PortPin<portExtender2, PORT_B, 1> LED_2;
typedef PortPin<portExtender2, PORT_B, 2> LED_3;
typedef PortPin<portExtender2, PORT_B, 3> LED_4;




class SwitchBase
{
public:
  SwitchBase(void)
  : m_State(0xff)
  {}
  virtual void init(void) = 0;
  virtual uint8_t refresh(void) = 0;
  bool released() { return m_State == 0x80; }
  bool pressed()  { return m_State == 0x7f; }
  bool active()   { return m_State == 0xff; }
  bool inactive() { return m_State == 0x00; }
  uint8_t getState() const { return m_State; }

protected:
  uint8_t m_State;
};

/* ein Schalter, debounced */
template<typename Extender, uint8_t Port, uint8_t PinNumber, uint8_t PullUp = 1>
class Switch : public SwitchBase
{
  typedef PortPin<Extender, Port, PinNumber> Pin;
public:
  Switch()
  {}
  void init(void)
  {
    Pin::set_mode(DIGITAL_INPUT);
    if(PullUp)
      Pin::setPullUp();
  }
  uint8_t refresh()
  {
    m_State = (m_State << 1) | (Pin::value() ? PullUp : !PullUp);
    return m_State;
  }

private:

  DISALLOW_COPY_AND_ASSIGN(Switch);
};

Switch<portExtender2, PORT_A, 7> SW_1;
Switch<portExtender2, PORT_A, 6> SW_2;
Switch<portExtender2, PORT_A, 5> SW_3;
Switch<portExtender2, PORT_A, 4> SW_4;

static SwitchBase* _4Switches[] = {&SW_1, &SW_2, &SW_3, &SW_4};

class Ui
{
public:
  void Init(void)
  {

    for(int i=0; i<4; i++)
    {
      _4Switches[i]->init();
    }
    LED_1::set_mode(DIGITAL_OUTPUT);
    LED_2::set_mode(DIGITAL_OUTPUT);
    portExtender2::Init();
  }

  void Poll(void)
  {
    portExtender2::ReadIO();
    SW_1.refresh();
    SW_2.refresh();
    SW_3.refresh();
    SW_4.refresh();
  }

  void Do(void)
  {
    if(SW_1.active())
      LED_1::set();
    else
      LED_1::clear();

    if(SW_4.active())
      LED_2::set();
    else
      LED_2::clear();

    portExtender2::WriteIO();
  }

private:

};

static Ui ui;

