/* 
 * File:   port_control.h
 * Author: luchop
 *
 * Created on October 13, 2026, 2:28 PM
 */

#ifndef PORT_CONTROL_H
#define	PORT_CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdint.h>

#ifndef TEST
#include "mcc_generated_files/mcc.h"
#endif

#define PORT_CONTROL_1 1
#define PORT_CONTROL_2 2
#define PORT_CONTROL_3 3
#define PORT_CONTROL_4 4


int set_high_port(uint16_t port);
int set_high_all();
int set_low_port(uint16_t port);
int set_low_all();

#ifdef	__cplusplus
}
#endif

#endif	/* PORT_CONTROL_H */

