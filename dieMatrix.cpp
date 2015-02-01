//#include <avr/io.h>           // Header-Datei f. IO-Register
#include <avr/interrupt.h>      // Header-Datei f. Interruptfunktion
//#include <stdint.h>           // Header-Datei f. standard Datentypen
#include "avrlib/parallel_io.h"
#include "avrlib/devices/shift_register.h"
#include "avrlib/spi.h"
#include "mcp23s17.h"
#include <limits.h>

using namespace avrlib;

static const uint8_t tcount = 160;      // Zählwert für 125kHz (20000 / 125)

ParallelPort<PortC, PARALLEL_BYTE> outp;
ParallelPort<PortD, PARALLEL_BYTE> inp;

typedef SpiMaster<NumberedGpio<4>, MSB_FIRST, 4> spi_master;

typedef MCP23S17<spi_master, 3> portExtender1;
typedef PortX<portExtender1, PORT_B> PortExt1_Out;
typedef PortX<portExtender1, PORT_A> PortExt1_In;

typedef MCP23S17<spi_master, 1> portExtender2;
typedef PortX<portExtender2, PORT_B> PortExt2_Out;
typedef PortX<portExtender2, PORT_A> PortExt2_In;


template<typename PortIn, typename PortOut>
static void InitPorts(void)
{
  PortOut::setMode(DIGITAL_OUTPUT);
  PortIn::setMode(DIGITAL_INPUT);
  PortOut::setInvertPolarity(0x00);
  PortIn::setInvertPolarity(0xFF);
  PortOut::setPullUp(0x00);
  PortIn::setPullUp(0xFF);
  PortIn::Write(0x00);  //reset input
}

inline void InitAllPorts(void)
{
  spi_master::Begin();
  spi_master::Send(0x40);  //0x48
  spi_master::Send(MCP23S17_IOCON);
  spi_master::Send(0x08);  // HAEN=1 (Enable Addressing)
  spi_master::End();

  InitPorts<PortExt1_In, PortExt1_Out>();
  // InitPorts<PortExt2_In, PortExt2_Out>();
  //InitPorts<PortExt3_In, PortExt3_Out>();
}

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

  outp.set_mode(DIGITAL_OUTPUT);
  inp.set_mode(DIGITAL_INPUT);

  initTimer0();
  outp.Write(0xff);

  // init MCP's
  spi_master::Init();
  InitAllPorts();
uint8_t count = 0;

  while(1)
  {
    _delay_ms(5);
    uint8_t switches = PortExt1_In::Read();
    //_delay_ms(1);
    uint8_t v = switches;     // input bits to be reversed
    uint8_t r = v; // r will be reversed bits of v; first get LSB of v
    int8_t s = sizeof(v) * CHAR_BIT - 1; // extra shift needed at end

    for (v >>= 1; v; v >>= 1)
    {
      r <<= 1;
      r |= v & 1;
      s--;
    }
    r <<= s; // shift when v's highest bits are zero
    PortExt1_Out::Write(r);

  }
}
