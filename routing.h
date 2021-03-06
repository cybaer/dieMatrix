/*
 * routing.h
 *
 *  Created on: 02.04.2015
 *      Author: cybaer
 */

#ifndef ROUTING_H_
#define ROUTING_H_

#include "avrlib/avrlib.h"
#include <avr/eeprom.h>

static const int8_t OutputCount = 10;
static const int8_t InputCount = 10;

extern uint8_t ScanData_1_2;
extern uint8_t ScanData_3_10;
extern uint8_t* MA[];
extern uint8_t* MB[];

class Routing
{
public:
  Routing();
  void setRoute(int8_t out, int8_t in);
  void setRouteByBeginOut(int8_t in);
  void setRouteByBeginIn(int8_t out);
  void unRoute(int8_t out);
  void setRouteBeginOut(int8_t out);
  void setRouteBeginIn(int8_t in);
  uint16_t getOutBitsByIn(int8_t in);
  int8_t getInByOut(int8_t out);
  uint16_t getOutBitsByBeginIn(void);
  bool isRouted(int8_t out) const;
  void readDefaultData(void);
  void writeDefaultData(void);
  uint16_t getScanBitsIn(void);
  uint16_t getScanBitsOut(void);
  void setAllRoutes(void);
  void resetScanBits(void) {ScanData_3_10 = 0xFF; ScanData_1_2 = 0xFF; };
private:
  int8_t m_MiOuts[10];
  uint8_t MA_Bits[2];
  uint8_t MB_Bits[10];
  int8_t m_RouteBeginOut;
  int8_t m_RouteBeginIn;
};
extern Routing routing;

#endif /* ROUTING_H_ */
