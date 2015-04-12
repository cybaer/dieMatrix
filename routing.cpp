/*
 * routing.cpp
 *
 *  Created on: 02.04.2015
 *      Author: cybaer
 */

#include "routing.h"
#include "devices.h"

Routing routing;

int8_t  eeData[OutputCount] EEMEM = {0,0,0,0,0,0,0,0,0,0};
/*uint8_t  eeData0[OutputCount] EEMEM = {0};
uint8_t  eeData1[OutputCount] EEMEM = {0};
uint8_t  eeData2[OutputCount] EEMEM = {0};
uint8_t  eeData3[OutputCount] EEMEM = {0};
uint8_t  eeData4[OutputCount] EEMEM = {0};
uint8_t  eeData5[OutputCount] EEMEM = {0};
uint8_t  eeData6[OutputCount] EEMEM = {0};
uint8_t  eeData7[OutputCount] EEMEM = {0};
uint8_t  eeData8[OutputCount] EEMEM = {0};
uint8_t  eeData9[OutputCount] EEMEM = {0};
*/
Routing::Routing()
: m_RouteBeginOut(NIL)
, m_RouteBeginIn(NIL)
{
  for(int8_t i=0; i<OutputCount; i++)
  {
    m_MiOuts[i] = NIL;
  }
  MA_Bits[0] = 0x2;   // Midi In-1
  MA_Bits[1] = 0x1;   // Midi In-2
  MB_Bits[0] = 0x08;  // Midi In-3
  MB_Bits[1] = 0x04;  // Midi In-4
  MB_Bits[2] = 0x02;  // Midi In-5
  MB_Bits[3] = 0x01;  // Midi In-6
  MB_Bits[4] = 0x80;  // Midi In-7
  MB_Bits[5] = 0x40;  // Midi In-8
  MB_Bits[6] = 0x20;  // Midi In-9
  MB_Bits[7] = 0x10;  // Midi In-10

}
void Routing::setRoute(int8_t out, int8_t in)
{
  m_MiOuts[out] = in;
  if(in < 2)
  {
    *MA[out] = MA_Bits[in];
    *MB[out] = 0;
  }
  else
  {
    *MB[out] = MB_Bits[in-2];
    *MA[out] = 0;
  }

  TIMSK0 &= ~(1 << OCIE0A);
  writeDefaultData();
  TIMSK0 |= (1 << OCIE0A);

}
void Routing::setRouteByBeginOut(int8_t in)
{
  setRoute(m_RouteBeginOut, in);
}
void Routing::setRouteByBeginIn(int8_t out)
{
  if(isRouted(out))
    unRoute(out);
  else
    setRoute(out, m_RouteBeginIn);
}
void Routing::unRoute(int8_t out)
{
  m_MiOuts[out] = NIL;
  *MA[out] = 0;
  *MB[out] = 0;

  //TIMSK0 &= ~(1 << OCIE0A);
  writeDefaultData();
  //TIMSK0 |= (1 << OCIE0A);

}
void Routing::setRouteBeginOut(int8_t out)
{
  m_RouteBeginOut = out;
  m_RouteBeginIn = NIL;
}

void Routing::setRouteBeginIn(int8_t in)
{
  m_RouteBeginIn = in;
  m_RouteBeginOut = NIL;
}

uint16_t Routing::getOutBitsByIn(int8_t in)
{
  uint16_t out = 0;
  for(int8_t i=0; i<OutputCount; i++)
  {
    if(m_MiOuts[i] == in)
      out |= 1 << i;
  }
  return out;
}
int8_t Routing::getInByOut(int8_t out)
{
  return m_MiOuts[out];
}

uint16_t Routing::getOutBitsByBeginIn(void)
{
  return getOutBitsByIn(m_RouteBeginIn);
}
bool Routing::isRouted(int8_t out) const
{
  const int8_t data = m_MiOuts[out];
  return (data < OutputCount && data >= 0);
}

void Routing::readDefaultData(void)
{
  eeprom_read_block (m_MiOuts, eeData, OutputCount);
}

void Routing::writeDefaultData(void)
{
  eeprom_write_block (m_MiOuts, eeData, OutputCount);
}
