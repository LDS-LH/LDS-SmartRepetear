/* 
 * File:   communication.h
 * Author: Fabio
 *
 * Created on 21 de octubre de 2019, 11:56
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#ifdef	__cplusplus
extern "C" {
#endif

//#include "device.h"
#include "mcc_generated_files/mcc.h"

/*  Communication parameters  */    
#define MAX_RX_TIMEOUT      0xFF   
    
void initRxBuffer(char* buf, uint32_t size);
void sendPacket(const char *str, uint32_t byteCount);
uint8_t receiveData(char *data, uint8_t maxdata);
void TxDeviceEnable(void);
void TxDeviceDisable(void);

#ifdef	__cplusplus
}
#endif

#endif	/* COMMUNICATION_H */

