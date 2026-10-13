#include "communication.h"

/**
 * @brief Initializes full buffer to null character.
 * @param buf
 * @param size
 */
void initRxBuffer(char* buf, uint32_t size)
{
    for(int i=0; i<size; i++)
    {
        buf[i]='\0';
    }
}

/**
 * @brief Send a null-terminated string
 * @param str
 */
void sendPacket(const char *str, uint32_t byteCount)
{
    TxDeviceEnable();

    for(int i=0; i<byteCount; i++)
    {
        if(EUSART1_is_tx_ready())
        {
            EUSART1_Write(*str);
            while(!EUSART1_is_tx_done());
            str++;
        }
    }
    if(EUSART1_is_tx_ready())
        {
            EUSART1_Write('\n');
            while(!EUSART1_is_tx_done());
        }
    TxDeviceDisable();
}

/**
 * 
 * @param data
 * @param timeout
 */
uint8_t receiveData(char *data, uint8_t maxdata)
{
    uint8_t charCount = 0;
    uint8_t c;
    
    do
    {
        c = EUSART1_Read();
        if(charCount <= maxdata)
        {
            data[charCount++] = c;
        }
    }while(c != 0x0A);
    
    data[charCount] = 0;
    return charCount;
}

void TxDeviceEnable(void)
{
    LATCbits.LATC1 = 1;   
}

void TxDeviceDisable(void)
{
    LATCbits.LATC1 = 0;
}
        

