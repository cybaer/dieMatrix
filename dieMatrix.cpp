//#include <avr/io.h>           // Header-Datei f. IO-Register
#include <avr/interrupt.h>      // Header-Datei f. Interruptfunktion
//#include <stdint.h>           // Header-Datei f. standard Datentypen
#include "avrlib/parallel_io.h"
#include "avrlib/devices/shift_register.h"
#include "avrlib/spi.h"
#include "mcp23s17.h"

using namespace avrlib;

static const uint8_t tcount = 160;      // Zählwert für 125kHz (20000 / 125)

//ParallelPort<PortA, PARALLEL_BYTE> testPort;
ParallelPort<PortC, PARALLEL_BYTE> outp;
ParallelPort<PortD, PARALLEL_BYTE> inp;

typedef SpiMaster<NumberedGpio<4>, MSB_FIRST, 4> spi_master;
//spi_master spi_interface;
typedef MCP23S17<spi_master> portExtender;

typedef PortA<portExtender> PortExt1Out;
typedef PortB<portExtender> PortExt1In;

typedef MCP23S17<spi_master, 2> portExtender2;


void initTimer0(void)
{
  TCCR0A = (1 << WGM01);  // CTC mode
  TCCR0B = (1<<CS00);     // Prescaler 1
  TCNT0 = 0;              // set count
  OCR0A = tcount;         // compare value
  TIMSK0 |= (1 << OCIE0A);// CTC Interrupt einschalten
  sei();                  // Global Interrupts aktivieren
}

static uint8_t MASK_0 = 0x01;
static uint8_t MASK_1 = 0x02;
static uint8_t MASK_2 = 0x04;
static uint8_t MASK_3 = 0x08;
static uint8_t MASK_4 = 0x10;
static uint8_t MASK_5 = 0x20;
static uint8_t MASK_6 = 0x40;
static uint8_t MASK_7 = 0x80;

static uint8_t bit = 0;

ISR (TIMER0_COMPA_vect) {

  PORTA |= (1 << PA0);
  uint8_t out = 0;
  //uint8_t byte = 0;
  uint8_t val = inp.Read();

  if(val & MASK_7)
    out |= 0x01;
  out <<= 1;
  if(val & MASK_6)
    out |= 0x01;
  out <<= 1;
  if(val & MASK_5)
    out |= 0x01;
  out <<= 1;
  if(val & MASK_4)
    out |= 0x01;
  out <<= 1;

  bit <<= 1;
  if(val & MASK_3)
  {
    out |= 0x01;
    bit |= 1;
  }
  out <<= 1;

  if(val & MASK_2)
    out |= 0x01;
  out <<= 1;
  if(val & MASK_1)
    out |= 0x01;
  out <<= 1;
  if(val & MASK_0)
    out |= 0x01;
  //outp.Write(out);

  if((bit & 0x0f) == 0x07)
    PORTA |= (1 << PA1);
  if((bit & 0x0f) == 0x08)
    PORTA &= ~(1 << PA1);

  PORTA &= ~(1 << PA0);
}

int main(void) {
  DDRA = (1 << DDA0)|(1 << DDA1);         // PortA0 als Ausgang für LED1

//testPort.set_mode(DIGITAL_INPUT);
  outp.set_mode(DIGITAL_OUTPUT);
  inp.set_mode(DIGITAL_INPUT);
  //inp.EnablePullUpResistors();

  initTimer0();
  outp.Write(0xff);
  /*reg.Init();
  regOut.Init();*/
  spi_master::Init();

  _delay_ms(5);

  // init MCP...
  //portExtender.Write(0 /* IODIRA */, 0x00);
  portExtender::setMode<PortA>(DIGITAL_OUTPUT)
  portExtender::setModePortA(DIGITAL_OUTPUT);
  //portExtender.Write(1 /* IODIRB */, 0xFF);
  portExtender::setModePortB(DIGITAL_INPUT);
  portExtender::Write(2 /* IOPOLA */, 0x00);
  portExtender::Write(3 /* IOPOLB */, 0x00);
  //portExtender.Write(0x0C /* GPPUA */, 0);
  portExtender::setPullUpPortA(0x00);
  //portExtender.Write(0x0D /* GPPUB */, 0xFF);
  portExtender::setPullUpPortB(0xFF);
  portExtender::Write(0x13 /* GPIOB */, 0);  //reset input

  portExtender2::Write(0 /* IODIRA */, 0x00);

  while(1)
  {
    _delay_ms(5);
    uint8_t switches = portExtender::Read(0x13 /* GPIOB */);
    //outp.Write(switches);
    portExtender::Write(0x12 /* GPIOA */, switches);
    //Mcp23s17Write(0x13 /* GPIOB */, 0xff);

  }
}
