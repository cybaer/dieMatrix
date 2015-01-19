/*
 * mcp23s17.h
 *
 *  Created on: 19.01.2015
 *      Author: cybaer
 */

#ifndef MCP23S17_H_
#define MCP23S17_H_

#include "avrlib/spi.h"


static const uint8_t MCP23S17_WRITE    = 0x00;
static const uint8_t MCP23S17_READ     = 0x01;

static const uint8_t MCP23S17_IODIRA  = 0x00;       // Controls the direction of the data I/O on Port A
static const uint8_t MCP23S17_IODIRB  = 0x01;       // Controls the direction of the data I/O on Port B
static const uint8_t MCP23S17_IPOLA   = 0x02;       // Configure the polarity on the corresponding GPIO (Port A)
static const uint8_t MCP23S17_IPOLB   = 0x03;       // Configure the polarity on the corresponding GPIO (Port B)
static const uint8_t MCP23S17_GPINTENA= 0x04;       // Controls the interrupt-on change feature for each pin for Port A
static const uint8_t MCP23S17_GPINTENB= 0x05;       // Controls the interrupt-on change feature for each pin for Port B
static const uint8_t MCP23S17_DEFVALA = 0x06;       // The default comparison value if the INTCONA is set to "1" for Port A
static const uint8_t MCP23S17_DEFVALB = 0x07;       // The default comparison value if the INTCONA is set to "1" for Port B
static const uint8_t MCP23S17_INTCONA = 0x08;       // Controls how the associated pin value is compared for the interrupt-on-change feature for Port A
static const uint8_t MCP23S17_INTCONB = 0x09;       // Controls how the associated pin value is compared for the interrupt-on-change feature for Port B
static const uint8_t MCP23S17_IOCON   = 0x0A;       // Contains several bits for configuring the device
static const uint8_t MCP23S17_GPPUA   = 0x0C;       // Controls the pull-up resistors for the port pins for port A
static const uint8_t MCP23S17_GPPUB   = 0x0D;       // Controls the pull-up resistors for the port pins for port B
static const uint8_t MCP23S17_INTFA   = 0x0E;       // READ ONLY // reflects the interrupt condition on port A pins of any pin that is enabled for interrupts via the GPINTEN register.
static const uint8_t MCP23S17_INTFB   = 0x0F;       // READ ONLY // reflects the interrupt condition on port B pins of any pin that is enabled for interrupts via the GPINTEN register.
static const uint8_t MCP23S17_INTCAPA = 0x10;       // READ ONLY // captures the GPIO port A value at the time the interrupt occurred
static const uint8_t MCP23S17_INTCAPB = 0x11;       // READ ONLY // captures the GPIO port B value at the time the interrupt occurred
static const uint8_t MCP23S17_GPIOA   = 0x12;       // Reflects the value on the port A (doing write function it only read input)
static const uint8_t MCP23S17_GPIOB   = 0x13;       // Reflects the value on the port B (doing write function it only read input)
static const uint8_t MCP23S17_OLATA   = 0x14;       // A write to this register modifies the output latches that modifies the pins configured as outputs for Port A
static const uint8_t MCP23S17_OLATB   = 0x15;       // A write to this register modifies the output latches that modifies the pins configured as outputs for Port B


template<typename spi_master, uint8_t addr = 0>
class MCP23S17
{
public:
  void Write(uint8_t address, uint8_t data)
  {
    spi_master::Begin();
    spi_master::Send(slaveAddress | MCP23S17_WRITE);
    spi_master::Send(address);
    spi_master::Send(data);
    spi_master::End();
  }

  uint8_t Read(uint8_t address)
  {
    spi_master::Begin();
    spi_master::Send(slaveAddress | MCP23S17_READ);
    spi_master::Send(address);
    uint8_t data = spi_master::Receive();
    spi_master::End();
    return data;
  }

  /*
   * void   config_control_register (char data)
  Configure the control setting (IOCON register).
void  config_pullups (Port port, char data)
  Configure if the port pins has Pull ups or not (GPPUn register).
void  config_direction (Port port, char data)
  Configure the direction of the port pins (IODIRn register).
void  config_polarity (Port port, char data)
  Configure the input polarity of the port pins (IPOLn register).
void  config_interrupt_enable (Port port, char data)
  Configure if the port pins is interruptable or not (GPINTENn register).
void  config_interrupt_control (Port port, char data)
  Configure if the interrupts is compared to default value register or against the previous value (INTCONn register).
void  config_mirror_interrupt (bool mirror)
  Configure if the interrupts pins is connected internally or separated into intA and intB (default = separated).
void  config_defaultvalue (Port port, char data)
  Configure the default value that the interrupt value is compared to if it setup for use (DEFVALn register).
void  config_interrupt_polarity (Polarity polarity)
  Configure if the interrupts is active high or active low.
char  read_interrupt_flag (Port port)
  Read the interrupt flags for the port.
char  read_interrupt_capture (Port port)
  Read the value of the port captured the time the interrupt occurred (use this is for fast changing inputs).
   * */




private:
  static const uint8_t slaveAddress = 0x40 | (addr << 1);
};

#endif /* MCP23S17_H_ */
