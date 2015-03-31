/*
 * devices.h
 *
 *  Created on: 13.02.2015
 *      Author: cybaer
 */

#ifndef DEVICES_H_
#define DEVICES_H_

using namespace avrlib;
static const int8_t NIL = 0xff;

#include "mcp23s17.h"
static const uint8_t SPI_Speed = 4;
typedef SpiMaster<NumberedGpio<4>, MSB_FIRST, SPI_Speed> spi_master;
typedef MCP23S17<spi_master, 1> portExtender1;
typedef MCP23S17<spi_master, 3> portExtender2;
typedef MCP23S17<spi_master, 7> portExtender3;

class LED_Base
{
public:
  virtual void init(void) = 0;
  virtual void set(void) = 0;
  virtual void clear(void) = 0;
};

template<typename Extender, uint8_t Port, uint8_t PinNumber>
class LED : public LED_Base
{
  typedef PortPin<Extender, Port, PinNumber> Pin;
public:
  LED()
  {}
  void init(void)
  {
    Pin::set_mode(DIGITAL_OUTPUT);
  }
  void set(void) { Pin::set(); }
  void clear(void) { Pin::clear(); }
};

class LEDArray
{
public:
  static const int8_t MAX_COUNT = 10;
  LEDArray(int8_t count, LED_Base* led1, LED_Base* led2, LED_Base* led3, LED_Base* led4, LED_Base* led5=0,
                         LED_Base* led6=0, LED_Base* led7=0, LED_Base* led8=0, LED_Base* led9=0, LED_Base* led10=0)
  : m_count(count)
  {
    m_LedArray[0] = led1;
    m_LedArray[1] = led2;
    m_LedArray[2] = led3;
    m_LedArray[3] = led4;
    m_LedArray[4] = led5;
    m_LedArray[5] = led6;
    m_LedArray[6] = led7;
    m_LedArray[7] = led8;
    m_LedArray[8] = led9;
    m_LedArray[9] = led10;
    if(m_count > MAX_COUNT)
      m_count = MAX_COUNT;
  }
  void init(void)
  {
    for(int8_t i=0; i<m_count; i++)
    {
      m_LedArray[i]->init();
    }
  }
  void clear(void)
  {
    for(int8_t i=0; i<m_count; i++)
    {
      m_LedArray[i]->clear();
    }
  }
  int8_t m_count;
  LED_Base* m_LedArray[10];

private:
  DISALLOW_COPY_AND_ASSIGN(LEDArray);
};

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

class SwitchArray
{
public:
  static const int8_t MAX_COUNT = 10;
  SwitchArray(int8_t count, SwitchBase* sw1, SwitchBase* sw2, SwitchBase* sw3, SwitchBase* sw4, SwitchBase* sw5=0,
                            SwitchBase* sw6=0, SwitchBase* sw7=0, SwitchBase* sw8=0, SwitchBase* sw9=0, SwitchBase* sw10=0)
  : m_count(count)
  {
    m_SwArray[0] = sw1;
    m_SwArray[1] = sw2;
    m_SwArray[2] = sw3;
    m_SwArray[3] = sw4;
    m_SwArray[4] = sw5;
    m_SwArray[5] = sw6;
    m_SwArray[6] = sw7;
    m_SwArray[7] = sw8;
    m_SwArray[8] = sw9;
    m_SwArray[9] = sw10;
    if(m_count > MAX_COUNT)
      m_count = MAX_COUNT;
  }
  void init(void)
  {
    for(int8_t i=0; i<m_count; i++)
    {
      m_SwArray[i]->init();
    }
  }
  void refresh(void)
  {
    for(int8_t i=0; i<m_count; i++)
    {
      m_SwArray[i]->refresh();
    }
  }
  bool getPressed(int8_t& index) const
  {
    for(int8_t i=0; i<m_count; i++)
    {
      if(m_SwArray[i]->pressed())
      {
        index = i;
        return true;
      }
    }
    return false;
  }
  int8_t m_count;
  SwitchBase* m_SwArray[10];

private:
  DISALLOW_COPY_AND_ASSIGN(SwitchArray);
};
#endif /* DEVICES_H_ */
