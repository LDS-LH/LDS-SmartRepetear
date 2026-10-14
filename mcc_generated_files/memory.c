/**
  MEMORY Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    memory.c

  @Summary
    This is the generated driver implementation file for the MEMORY driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This file provides implementations of driver APIs for MEMORY.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.80.0
        Device            :  PIC16F18424
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.10 and above
        MPLAB             :  MPLAB X 5.30
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "memory.h"

/**
  Section: Data EEPROM Module APIs
*/

void DATAEE_WriteByte(uint16_t bAdd, uint8_t bData)
{
    uint8_t GIEBitValue = INTCONbits.GIE;

    NVMADRH = ((bAdd >> 8) & 0xFF);
    NVMADRL = (bAdd & 0xFF);
    NVMDATL = bData;    
    NVMCON1bits.NVMREGS = 1;
    NVMCON1bits.WREN = 1;
    INTCONbits.GIE = 0;     // Disable interrupts
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
    // Wait for write to complete
    while (NVMCON1bits.WR)
    {
    }

    NVMCON1bits.WREN = 0;
    INTCONbits.GIE = GIEBitValue;   // restore interrupt enable
}

void DATAEE_WriteWord(uint16_t bAdd, uint16_t bData)
{
    DATAEE_WriteByte(bAdd, bData & 0xFF);
    DATAEE_WriteByte(bAdd + 0x0001, (bData>>8) & 0xFF);
}

uint16_t DATAEE_ReadWord(uint16_t bAdd)
{
    uint8_t low_byte = DATAEE_ReadByte(bAdd);
    uint8_t high_byte = DATAEE_ReadByte(bAdd + 0x0001);
    
    return ( high_byte << 8 | low_byte);
}

uint8_t DATAEE_ReadByte(uint16_t bAdd)
{
    NVMADRH = ((bAdd >> 8) & 0xFF);
    NVMADRL = (bAdd & 0xFF);
    NVMCON1bits.NVMREGS = 1;    
    NVMCON1bits.RD = 1;
    NOP();  // NOPs may be required for latency at high frequencies
    NOP();

    return (NVMDATL);
}


/**
 End of File
*/
