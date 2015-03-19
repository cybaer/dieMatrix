#include <avr/interrupt.h>      // Header-Datei f. Interruptfunktion
#include "avrlib/parallel_io.h"
#include "avrlib/devices/shift_register.h"
#include "avrlib/spi.h"
#include "avrlib/gpio.h"
#include "mcp23s17.h"
#include <limits.h>

#include "ui.h"

using namespace avrlib;

static const uint8_t tcount = 160;      // Zählwert für 125kHz (20000 / 125)

ParallelPort<PortA, PARALLEL_BYTE> outp;
ParallelPort<PortC, PARALLEL_BYTE> inp;

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

inline void SetSpiToHwAdressMode(void)
{
  spi_master::Begin();
  spi_master::Send(0x40);  //0x48
  spi_master::Send(MCP23S17_IOCON);
  spi_master::Send(0x08);  // HAEN=1 (Enable Addressing)
  spi_master::End();
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
static uint8_t MASK_5 = 0x80;
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

  if(val & MASK_3)
    out |= 0x01;
  out <<= 1;

  if(val & MASK_2)
    out |= 0x01;
  out <<= 1;
  if(val & MASK_1)
    out |= 0x01;
  out <<= 1;
  if(val & MASK_0)
    out |= 0x01;

  outp.Write(out);

  PORTA &= ~(1 << PA0);
}

int main(void) {
  _delay_ms(50);
  DDRA = (1 << DDA0)|(1 << DDA1);         // PortA0 als Ausgang für LED1

  outp.set_mode(DIGITAL_OUTPUT);
  inp.set_mode(DIGITAL_INPUT);

  initTimer0();
  outp.Write(0xff);

  _delay_ms(20);
  // init MCP's
  spi_master::Init();
  SetSpiToHwAdressMode();
  _delay_ms(20);
  ui.Init();

  while(1)
  {
    _delay_ms(5);

    ui.Poll();
    ui.Do();
  }
}
