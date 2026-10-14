/**
  EUSART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart1.c

  @Summary
    This is the generated driver implementation file for the EUSART1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for EUSART1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.80.0
        Device            :  PIC16F18424
        Driver Version    :  2.1.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.10 and above
        MPLAB 	          :  MPLAB X 5.30
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
#include "eusart1.h"
#include "tmr1.h"

/**
  Section: Macro Declarations
*/

/**
  Section: Global Variables
*/

volatile uint8_t eusart1RxHead = 0;
volatile uint8_t eusart1RxTail = 0;
volatile uint8_t eusart1RxBuffer[EUSART1_RX_BUFFER_SIZE];
volatile eusart1_status_t eusart1RxStatusBuffer[EUSART1_RX_BUFFER_SIZE];
volatile uint8_t eusart1RxCount;
volatile uint8_t eusart1PacketFinish = 0;
volatile eusart1_status_t eusart1RxLastError;

extern uart_expire_time;

/**
  Section: EUSART1 APIs
*/
void (*EUSART1_RxDefaultInterruptHandler)(void);

void (*EUSART1_FramingErrorHandler)(void);
void (*EUSART1_OverrunErrorHandler)(void);
void (*EUSART1_ErrorHandler)(void);

void EUSART1_DefaultFramingErrorHandler(void);
void EUSART1_DefaultOverrunErrorHandler(void);
void EUSART1_DefaultErrorHandler(void);

uint8_t setBaudRateEUSART1(uint32_t baudRate)
{
    TX1STAbits.TXEN = 0;  // Deshabilitar transmisión
    RC1STAbits.CREN = 0;  // Deshabilitar recepción


    switch(baudRate)
    {
        case 300:             
            // SP1BRGL 64; 
             SP1BRGL = 0x40;

            // SP1BRGH 3; 
            SP1BRGH = 0x03;

            //TMR1H 222; 
            TMR1H = 0xDE;

            //TMR1L 204; 
            TMR1L = 0xCC;
            break;
        case 1200:
            // SP1BRGL 207; 
            SP1BRGL = 0xCF;

            // SP1BRGH 0; 
            SP1BRGH = 0x00;

            //TMR1H 247; 
            TMR1H = 0xF7;

            //TMR1L 179; 
            TMR1L = 0xB3;
            break;
        case 2400:
            // SP1BRGL 103; 
            SP1BRGL = 0x67;
            // SP1BRGH 0; 
            SP1BRGH = 0x00;

            //TMR1H 251; 
            TMR1H = 0xFB;

            //TMR1L 230; 
            TMR1L = 0xE6;
            break;
        case 4800:
            // SP1BRGL 103; 
            SP1BRG  = (_XTAL_FREQ / (4 * 4800)) - 1;
            
            //TMR1H 253; 
            TMR1H = 0xFD;

            //TMR1L 243; 
            TMR1L = 0xF3;
            break;
        case 9600:
            // SP1BRGL 25; 
            SP1BRGL = 0x19;

            // SP1BRGH 0; 
            SP1BRGH = 0x00;
           
            //TMR1H 254; 
            TMR1H = 0xFE;

            //TMR1L 237; 
            TMR1L = 0xED;
            break;
        case 19200:
            // SP1BRGL 12; 
            SP1BRGL = 0x0C;

            // SP1BRGH 0; 
            SP1BRGH = 0x00;
            
            //TMR1H 255; 
            TMR1H = 0xFF;

            //TMR1L 106; 
            TMR1L = 0x6A;
            break;
        default:
            // Reiniciar los registros relacionados con el USART1
            TX1STAbits.TXEN = 1;  // Habilitar transmisión
            RC1STAbits.CREN = 1;  // Habilitar recepción
            return 1;
    }
     TX1STAbits.TXEN = 1;  // Habilitar transmisión
     RC1STAbits.CREN = 1;  // Habilitar recepción
    return 0;
}           

void EUSART1_Initialize(void)
{
    // disable interrupts before changing states
    PIE3bits.RC1IE = 0;
    EUSART1_SetRxInterruptHandler(EUSART1_Receive_ISR);
    // Set the EUSART1 module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;

    // SP1BRGL 25; 
    SP1BRGL = 0x19;

    // SP1BRGH 0; 
    SP1BRGH = 0x00;
    
//    BAUD1CONbits_t.ABDEN = 1;
//    BAUD1CONbits_t.WUE  = 1;


    EUSART1_SetFramingErrorHandler(EUSART1_DefaultFramingErrorHandler);
    EUSART1_SetOverrunErrorHandler(EUSART1_DefaultOverrunErrorHandler);
    EUSART1_SetErrorHandler(EUSART1_DefaultErrorHandler);

    eusart1RxLastError.status = 0;


    eusart1RxHead = 0;
    eusart1RxTail = 0;
    eusart1RxCount = 0;

    // enable receive interrupt
    PIE3bits.RC1IE = 1;
}

bool EUSART1_is_tx_ready(void)
{
    return (bool)(PIR3bits.TX1IF && TX1STAbits.TXEN);
}

bool EUSART1_is_rx_ready(void)
{
    return (eusart1RxCount ? true : false);
}

bool EUSART1_is_tx_done(void)
{
    return TX1STAbits.TRMT;
}

eusart1_status_t EUSART1_get_last_status(void){
    return eusart1RxLastError;
}

#define bkbhit (eusart1RxHead != eusart1RxTail)
uint8_t EUSART1_Read(void)
{
    uint8_t readValue  = 0;
    
    //while(0 == eusart1RxCount)
    //{
    //}

    //eusart1RxLastError = eusart1RxStatusBuffer[eusart1RxTail];
//    while(!bkbhit); // se cuelga si queres leer y esta vacio
    
    readValue = eusart1RxBuffer[eusart1RxTail];
    eusart1RxTail = (eusart1RxTail + 1) % EUSART1_RX_BUFFER_SIZE;

    //PIE3bits.RC1IE = 0;
    //eusart1RxCount--;
    //PIE3bits.RC1IE = 1;
    return readValue;
    
}

void EUSART1_Write(uint8_t txData)
{
    while(0 == PIR3bits.TX1IF)
    {
    }

    TX1REG = txData;    // Write the data byte to the USART.
}



void EUSART1_Receive_ISR(void)
{
    
    eusart1RxStatusBuffer[eusart1RxHead].status = 0;

    if(RC1STAbits.FERR){
        eusart1RxStatusBuffer[eusart1RxHead].ferr = 1;
        EUSART1_FramingErrorHandler();
    }

    if(RC1STAbits.OERR){
        eusart1RxStatusBuffer[eusart1RxHead].oerr = 1;
        EUSART1_OverrunErrorHandler();
    }
    
    if(eusart1RxStatusBuffer[eusart1RxHead].status){
        EUSART1_ErrorHandler();
    } else {
        EUSART1_RxDataHandler();
    }
    
    // or set custom function using EUSART1_SetRxInterruptHandler()
}

void EUSART1_RxDataHandler(void){
    // use this default receive interrupt handler code
    int t;
    eusart1RxBuffer[eusart1RxHead] = RC1REG;
    eusart1RxCount++;
    
    if(eusart1RxCount == 8)
    {
        eusart1PacketFinish++;
        eusart1RxCount = 0;
        TMR1_StopTimer(); 
    }else if(eusart1RxCount == 1){
        TMR1_WriteTimer(0);
        TMR1_StartTimer();
    }
    
    uart_expire_time = false;
    t=eusart1RxHead;
    eusart1RxHead = (eusart1RxHead + 1)% EUSART1_RX_BUFFER_SIZE;
    if(eusart1RxHead == eusart1RxTail)
    {
        eusart1RxHead = t;
    }
}

void EUSART1_DefaultFramingErrorHandler(void){}

void EUSART1_DefaultOverrunErrorHandler(void){
    // EUSART1 error - restart

    RC1STAbits.CREN = 0;
    RC1STAbits.CREN = 1;

}

void EUSART1_DefaultErrorHandler(void){
    EUSART1_RxDataHandler();
}

void EUSART1_SetFramingErrorHandler(void (* interruptHandler)(void)){
    EUSART1_FramingErrorHandler = interruptHandler;
}

void EUSART1_SetOverrunErrorHandler(void (* interruptHandler)(void)){
    EUSART1_OverrunErrorHandler = interruptHandler;
}

void EUSART1_SetErrorHandler(void (* interruptHandler)(void)){
    EUSART1_ErrorHandler = interruptHandler;
}


void EUSART1_SetRxInterruptHandler(void (* interruptHandler)(void)){
    EUSART1_RxDefaultInterruptHandler = interruptHandler;
}

void UART_ClearBuffer() {
    eusart1RxHead = 0;
    eusart1RxTail = 0;
    eusart1RxCount = 0;
}
/**
  End of File
*/
