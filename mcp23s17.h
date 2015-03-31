/*
 * mcp23s17.h
 *
 *  Created on: 19.01.2015
 *      Author: cybaer
 */

#ifndef MCP23S17_H_
#define MCP23S17_H_

#include "avrlib/spi.h"
#include "avrlib/gpio.h"

namespace avrlib
{
  static const uint8_t PORT_A = 0;
  static const uint8_t PORT_B = 1;

  static const uint8_t MCP23S17_WRITE = 0x00;
  static const uint8_t MCP23S17_READ = 0x01;

  static const uint8_t MCP23S17_IODIRA = 0x00; // Controls the direction of the data I/O on Port A
  static const uint8_t MCP23S17_IODIRB = 0x01; // Controls the direction of the data I/O on Port B
  static const uint8_t MCP23S17_IPOLA = 0x02; // Configure the polarity on the corresponding GPIO (Port A)
  static const uint8_t MCP23S17_IPOLB = 0x03; // Configure the polarity on the corresponding GPIO (Port B)
  static const uint8_t MCP23S17_GPINTENA = 0x04; // Controls the interrupt-on change feature for each pin for Port A
  static const uint8_t MCP23S17_GPINTENB = 0x05; // Controls the interrupt-on change feature for each pin for Port B
  static const uint8_t MCP23S17_DEFVALA = 0x06; // The default comparison value if the INTCONA is set to "1" for Port A
  static const uint8_t MCP23S17_DEFVALB = 0x07; // The default comparison value if the INTCONA is set to "1" for Port B
  static const uint8_t MCP23S17_INTCONA = 0x08; // Controls how the associated pin value is compared for the interrupt-on-change feature for Port A
  static const uint8_t MCP23S17_INTCONB = 0x09; // Controls how the associated pin value is compared for the interrupt-on-change feature for Port B
  static const uint8_t MCP23S17_IOCON = 0x0A; // Contains several bits for configuring the device
  static const uint8_t MCP23S17_GPPUA = 0x0C; // Controls the pull-up resistors for the port pins for port A
  static const uint8_t MCP23S17_GPPUB = 0x0D; // Controls the pull-up resistors for the port pins for port B
  static const uint8_t MCP23S17_INTFA = 0x0E; // READ ONLY // reflects the interrupt condition on port A pins of any pin that is enabled for interrupts via the GPINTEN register.
  static const uint8_t MCP23S17_INTFB = 0x0F; // READ ONLY // reflects the interrupt condition on port B pins of any pin that is enabled for interrupts via the GPINTEN register.
  static const uint8_t MCP23S17_INTCAPA = 0x10; // READ ONLY // captures the GPIO port A value at the time the interrupt occurred
  static const uint8_t MCP23S17_INTCAPB = 0x11; // READ ONLY // captures the GPIO port B value at the time the interrupt occurred
  static const uint8_t MCP23S17_GPIOA = 0x12; // Reflects the value on the port A (doing write function it only read input)
  static const uint8_t MCP23S17_GPIOB = 0x13; // Reflects the value on the port B (doing write function it only read input)
  static const uint8_t MCP23S17_OLATA = 0x14; // A write to this register modifies the output latches that modifies the pins configured as outputs for Port A
  static const uint8_t MCP23S17_OLATB = 0x15; // A write to this register modifies the output latches that modifies the pins configured as outputs for Port B


  struct Registers
  {
    uint8_t Gpio;
    uint8_t IoDir;
    uint8_t Gppu;

  };

  template<typename spi_master, uint8_t addr>
  class MCP23S17
  {
  public:

    static inline void Write(uint8_t address, uint8_t data)
    {
      spi_master::Begin();
      spi_master::Send(slaveAddress | MCP23S17_WRITE);
      spi_master::Send(address);
      spi_master::Send(data);
      spi_master::End();
    }

    static inline uint8_t Read(uint8_t address)
    {
      spi_master::Begin();
      spi_master::Send(slaveAddress | MCP23S17_READ);
      spi_master::Send(address);
      uint8_t data = spi_master::Receive();
      spi_master::End();
      return data;
    }

    static inline void ReadIO(void)
    {
      Ports[PORT_A].Gpio = Read(MCP23S17_GPIOA);
      Ports[PORT_B].Gpio = Read(MCP23S17_GPIOB);
    }
    static inline void WriteIO(void)
    {
      Write(MCP23S17_GPIOA, Ports[PORT_A].Gpio);
      Write(MCP23S17_GPIOB, Ports[PORT_B].Gpio);
    }

    static inline void Init(void)
    {
      Write(MCP23S17_IODIRA, Ports[PORT_A].IoDir);
      Write(MCP23S17_IODIRB, Ports[PORT_B].IoDir);
      Write(MCP23S17_GPPUA, Ports[PORT_A].Gppu);
      Write(MCP23S17_GPPUB, Ports[PORT_B].Gppu);
    }

    static Registers Ports[2];

  private:
    static const uint8_t slaveAddress = 0x40 | (addr << 1);

  };

  // funktioniert, aber warum?
  template<typename spi_master, uint8_t addr>
  Registers MCP23S17<spi_master, addr>::Ports[2];

  template<typename Extender, uint8_t Port, uint8_t Pin>
  class PortPin
  {
  public:
    static void clear(){ Extender::Ports[Port].Gpio &= ~_BV(Pin); }
    static void set(){ Extender::Ports[Port].Gpio |= _BV(Pin); }
    static void toggle(){ Extender::Ports[Port].Gpio ^= _BV(Pin); }
    static uint8_t value(){ return Extender::Ports[Port].Gpio & _BV(Pin) ? 0 : 1; }
    static void set_mode(uint8_t mode)
    {
      if(mode == DIGITAL_INPUT)
      {
        Extender::Ports[Port].IoDir |= _BV(Pin);
      }
      else
      {
        Extender::Ports[Port].IoDir &= ~_BV(Pin);
      }
    }
    static void setPullUp(){ Extender::Ports[Port].Gppu |= _BV(Pin); }
  };





  template<typename Extender, uint8_t Port>
  class PortX
  {
  public:
    static inline void Write(uint8_t data)
    {
      Extender::Write(MCP23S17_GPIOA + Port, data);
    }
    static inline uint8_t Read(void)
    {
      return Extender::Read(MCP23S17_GPIOA + Port);
    }
    static inline void setMode(uint8_t mode, uint8_t pins = 0xFF)
    {
      uint8_t direction = mode == DIGITAL_OUTPUT ? ~pins : pins;
      Extender::Write(MCP23S17_IODIRA + Port, direction);
    }
    static inline void setPullUp(uint8_t pins = 0xFF)
    {
      Extender::Write(MCP23S17_GPPUA + Port, pins);
    }

    static inline void setInvertPolarity(uint8_t pins = 0xFF)
    {
      Extender::Write(MCP23S17_IPOLA + Port, pins);
    }
  };
}
#endif /* MCP23S17_H_ */
