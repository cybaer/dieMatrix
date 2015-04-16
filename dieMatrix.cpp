#include <avr/interrupt.h>      // Header-Datei f. Interruptfunktion
#include "avrlib/parallel_io.h"
#include "avrlib/devices/shift_register.h"
#include "avrlib/spi.h"
#include "avrlib/gpio.h"
#include "mcp23s17.h"
#include <limits.h>

#include "ui.h"
#include "routing.h"

// __Compiler Bug__
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
void __cxa_pure_virtual() {};


using namespace avrlib;

static uint8_t MA_1 = 0x00;
static uint8_t MA_2 = 0x00;
static uint8_t MA_3 = 0x00;
static uint8_t MA_4 = 0x00;
static uint8_t MA_5 = 0x00;
static uint8_t MA_6 = 0x00;
static uint8_t MA_7 = 0x00;
static uint8_t MA_8 = 0x00;
static uint8_t MA_9 = 0x00;
static uint8_t MA_10 = 0x00;

static uint8_t MB_1 = 0x00;
static uint8_t MB_2 = 0x00;
static uint8_t MB_3 = 0x00;
static uint8_t MB_4 = 0x00;
static uint8_t MB_5 = 0x00;
static uint8_t MB_6 = 0x00;
static uint8_t MB_7 = 0x00;
static uint8_t MB_8 = 0x00;
static uint8_t MB_9 = 0x00;
static uint8_t MB_10 = 0x00;

uint8_t midi_1_2;
uint8_t midi_3_10;

uint8_t* MA[] = {&MA_1, &MA_2, &MA_3, &MA_4, &MA_5, &MA_6, &MA_7, &MA_8, &MA_9, &MA_10};
uint8_t* MB[] = {&MB_1, &MB_2, &MB_3, &MB_4, &MB_5, &MB_6, &MB_7, &MB_8, &MB_9, &MB_10};

//Midi Hardware
typedef Gpio<PortD, 5> MidiOut1;
typedef Gpio<PortD, 4> MidiOut2;

typedef Gpio<PortA, 4> MidiOut3;
typedef Gpio<PortA, 5> MidiOut4;
typedef Gpio<PortA, 6> MidiOut5;
typedef Gpio<PortA, 7> MidiOut6;
typedef Gpio<PortA, 0> MidiOut7;
typedef Gpio<PortA, 1> MidiOut8;
typedef Gpio<PortA, 2> MidiOut9;
typedef Gpio<PortA, 3> MidiOut10;


static const uint8_t tcount = 160;      // Zählwert für 125kHz (20000 / 125)

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
  //sei();                  // Global Interrupts aktivieren
}



ParallelPort<PortC, PARALLEL_BYTE> MidiIn_3_10;
ParallelPort<PortD, PARALLEL_DOUBLE_HIGH> MidiIn_1_2;

ISR (TIMER0_COMPA_vect)
{
  Debug1::set_value(true);

  midi_1_2 = MidiIn_1_2.Read();
  midi_3_10 = MidiIn_3_10.Read();

  MidiOut1::set_value((midi_3_10 & MB_1) || (midi_1_2 & MA_1));
  MidiOut2::set_value((midi_3_10 & MB_2) || (midi_1_2 & MA_2));
  MidiOut3::set_value((midi_3_10 & MB_3) || (midi_1_2 & MA_3));
  MidiOut4::set_value((midi_3_10 & MB_4) || (midi_1_2 & MA_4));
  MidiOut5::set_value((midi_3_10 & MB_5) || (midi_1_2 & MA_5));

  MidiOut6::set_value((midi_3_10 & MB_6) || (midi_1_2 & MA_6));
  MidiOut7::set_value((midi_3_10 & MB_7) || (midi_1_2 & MA_7));
  MidiOut8::set_value((midi_3_10 & MB_8) || (midi_1_2 & MA_8));
  MidiOut9::set_value((midi_3_10 & MB_9) || (midi_1_2 & MA_9));
  MidiOut10::set_value((midi_3_10 & MB_10) || (midi_1_2 & MA_10));
/*
  MidiOut1::Low();
  if((midi_1_2 & MA_1) || (midi_3_10 & MB_1))
  MidiOut1::High();

  MidiOut2::Low();
  if((midi_1_2 & MA_2) || (midi_3_10 & MB_2))
  MidiOut2::High();

  MidiOut3::Low();
  if((midi_1_2 & MA_3) || (midi_3_10 & MB_3))
  MidiOut3::High();

  MidiOut4::Low();
  if((midi_1_2 & MA_4) || (midi_3_10 & MB_4))
  MidiOut4::High();

  MidiOut5::Low();
  if((midi_1_2 & MA_5) || (midi_3_10 & MB_5))
  MidiOut5::High();

  MidiOut6::Low();
  if((midi_1_2 & MA_6) || (midi_3_10 & MB_6))
  MidiOut6::High();

  MidiOut7::Low();
  if((midi_1_2 & MA_7) || (midi_3_10 & MB_7))
  MidiOut7::High();

  MidiOut8::Low();
  if((midi_1_2 & MA_8) || (midi_3_10 & MB_8))
  MidiOut8::High();

  MidiOut9::Low();
  if((midi_1_2 & MA_9) || (midi_3_10 & MB_9))
  MidiOut9::High();

  MidiOut10::Low();
  if((midi_1_2 & MA_10) || (midi_3_10 & MB_10))
  MidiOut10::High();
*/
  Debug1::set_value(false);

}

int main(void)
{
 // _delay_ms(50);

  MidiIn_3_10.set_mode(DIGITAL_INPUT);
  MidiIn_1_2.set_mode(DIGITAL_INPUT);

  MidiOut1::set_mode(DIGITAL_OUTPUT);
  MidiOut2::set_mode(DIGITAL_OUTPUT);
  MidiOut3::set_mode(DIGITAL_OUTPUT);
  MidiOut4::set_mode(DIGITAL_OUTPUT);
  MidiOut5::set_mode(DIGITAL_OUTPUT);
  MidiOut6::set_mode(DIGITAL_OUTPUT);
  MidiOut7::set_mode(DIGITAL_OUTPUT);
  MidiOut8::set_mode(DIGITAL_OUTPUT);
  MidiOut9::set_mode(DIGITAL_OUTPUT);
  MidiOut10::set_mode(DIGITAL_OUTPUT);


  Debug1::set_mode(DIGITAL_OUTPUT);
  Debug1::set_value(false);
  Debug2::set_mode(DIGITAL_OUTPUT);
  Debug2::set_value(false);

  initTimer0();

  //_delay_ms(20);
  // init MCP's
  spi_master::Init();
  SetSpiToHwAdressMode();
  //_delay_ms(20);
  ui.Init();
  sei();                  // Global Interrupts aktivieren

  while(1)
  {
    //Debug2::Toggle();
    //_delay_ms(5);
    ui.Poll();
    ui.Do();

  }
}
