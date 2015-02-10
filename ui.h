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
typedef PortPin<portExtender2, PORT_A, 7> SW_1;
typedef PortPin<portExtender2, PORT_A, 6> SW_2;
typedef PortPin<portExtender2, PORT_A, 5> SW_3;
typedef PortPin<portExtender2, PORT_A, 4> SW_4;

typedef PortPin<portExtender2, PORT_B, 0> LED_1;
typedef PortPin<portExtender2, PORT_B, 1> LED_2;
typedef PortPin<portExtender2, PORT_B, 2> LED_3;
typedef PortPin<portExtender2, PORT_B, 3> LED_4;



class Switch;

class Ui
{
public:
  void Init(void)
  {

    SW_1::set_mode(DIGITAL_INPUT);
    SW_1::setPullUp();
    LED_1::set_mode(DIGITAL_OUTPUT);
    LED_2::set_mode(DIGITAL_OUTPUT);
    portExtender2::Init();
  }

  void Poll(void)
  {
    portExtender2::ReadIO();
  }

  void Do(void)
  {
    if(SW_1::value())
      LED_1::set();
    else
      LED_1::clear();

    LED_2::set();
    portExtender2::WriteIO();
  }

private:

};

static Ui ui;

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
class Switch :public SwitchBase
{
public:
  Switch(int pin, bool pulledUp = true)
  : m_Pin(pin)
  , m_Active(!pulledUp)
  {}
  void init()
  {
    //pinMode(m_Pin,INPUT);
    //digitalWrite(m_Pin, m_Active ? LOW : HIGH);
  };
  uint8_t refresh()
  {
    //m_State = (m_State << 1) | (digitalRead(m_Pin) ? m_Active : !m_Active);
    return m_State;
  }

private:
  int m_Pin;
  int m_Active;

  DISALLOW_COPY_AND_ASSIGN(Switch);
};

