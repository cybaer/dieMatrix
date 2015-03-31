/*
 * hardware.h
 *
 *  Created on: 25.03.2015
 *      Author: cybaer
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "avrlib/spi.h"
#include "avrlib/gpio.h"
#include "devices.h"

using namespace avrlib;




LED<portExtender1, PORT_B, 7> LED_In1;
LED<portExtender1, PORT_B, 6> LED_In2;
LED<portExtender1, PORT_B, 5> LED_In3;
LED<portExtender1, PORT_B, 4> LED_In4;
LED<portExtender1, PORT_B, 3> LED_In5;
LED<portExtender1, PORT_B, 2> LED_In6;
LED<portExtender1, PORT_B, 1> LED_In7;
LED<portExtender1, PORT_B, 0> LED_In8;
LED<portExtender2, PORT_B, 7> LED_In9;
LED<portExtender2, PORT_B, 6> LED_In10;

Switch<portExtender1, PORT_A, 0> SW_In1;
Switch<portExtender1, PORT_A, 1> SW_In2;
Switch<portExtender1, PORT_A, 2> SW_In3;
Switch<portExtender1, PORT_A, 3> SW_In4;
Switch<portExtender1, PORT_A, 4> SW_In5;
Switch<portExtender1, PORT_A, 5> SW_In6;
Switch<portExtender1, PORT_A, 6> SW_In7;
Switch<portExtender1, PORT_A, 7> SW_In8;
Switch<portExtender2, PORT_A, 0> SW_In9;
Switch<portExtender2, PORT_A, 1> SW_In10;

LED<portExtender2, PORT_B, 5> LED_Out1;
LED<portExtender2, PORT_B, 4> LED_Out2;
LED<portExtender2, PORT_B, 3> LED_Out3;
LED<portExtender2, PORT_B, 2> LED_Out4;
LED<portExtender2, PORT_B, 1> LED_Out5;
LED<portExtender2, PORT_B, 0> LED_Out6;
LED<portExtender3, PORT_B, 7> LED_Out7;
LED<portExtender3, PORT_B, 6> LED_Out8;
LED<portExtender3, PORT_B, 5> LED_Out9;
LED<portExtender3, PORT_B, 4> LED_Out10;

Switch<portExtender2, PORT_A, 2> SW_Out1;
Switch<portExtender2, PORT_A, 3> SW_Out2;
Switch<portExtender2, PORT_A, 4> SW_Out3;
Switch<portExtender2, PORT_A, 5> SW_Out4;
Switch<portExtender2, PORT_A, 6> SW_Out5;
Switch<portExtender2, PORT_A, 7> SW_Out6;
Switch<portExtender3, PORT_A, 0> SW_Out7;
Switch<portExtender3, PORT_A, 1> SW_Out8;
Switch<portExtender3, PORT_A, 2> SW_Out9;
Switch<portExtender3, PORT_A, 3> SW_Out10;

LED<portExtender3, PORT_B, 3> LED_M1;
LED<portExtender3, PORT_B, 2> LED_M2;
LED<portExtender3, PORT_B, 1> LED_M3;
LED<portExtender3, PORT_B, 0> LED_M4;

Switch<portExtender3, PORT_A, 4> SW_M1;
Switch<portExtender3, PORT_A, 5> SW_M2;
Switch<portExtender3, PORT_A, 6> SW_M3;
Switch<portExtender3, PORT_A, 7> SW_M4;




#endif /* HARDWARE_H_ */
