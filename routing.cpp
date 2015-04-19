/*
 * routing.cpp
 *
 *  Created on: 02.04.2015
 *      Author: cybaer
 */

#include "routing.h"
#include "devices.h"

Routing routing;

int8_t  eeData[OutputCount] EEMEM ;//= {0,0,0,0,0,0,0,0,0,0};
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

uint16_t Routing::getScanBitsIn(void)
{
  uint8_t in = ~ScanData_3_10;
  uint16_t bits = 0;

  for(int8_t i=9; i>=0; i--)
  {
    if(in & MB_Bits[i])
        bits |= 1;
      bits <<= 1;
  }

  in = ~ScanData_1_2;
  ScanData_1_2 = 0xFF;


  if(in & MA_Bits[1])
    bits |= 1;
  bits <<= 1;
  if(in & MA_Bits[0])
    bits |= 1;

  return bits;
}
uint16_t Routing::getScanBitsOut(void)
{
  uint8_t in_A = ~ScanData_1_2;
  uint8_t in_B = ~ScanData_3_10;
  uint16_t bits = 0;

  for(int8_t i=9; i>=0; i--)
  {
    int8_t input= m_MiOuts[i];
    if(input != NIL)
    {
      if(input < 2)
      {
        if(in_A & MA_Bits[input])
          bits |= 1;
      }
      else
      {
        if(in_B & MB_Bits[input-2])
          bits |= 1;
      }
    }
    bits <<= 1;
  }
  bits >>= 1;
  return bits;
}

void Routing::setAllRoutes(void)
{
  for(int8_t i=0; i<OutputCount; i++)
  {
    int8_t in = m_MiOuts[i];

    if(in == NIL)
    {
      *MA[i] = 0;
      *MB[i] = 0;
      continue;
    }
    if(in < 2)
    {
      *MA[i] = MA_Bits[in];
      *MB[i] = 0;
    }
    else
    {
      *MB[i] = MB_Bits[in-2];
      *MA[i] = 0;
    }
  }
}
