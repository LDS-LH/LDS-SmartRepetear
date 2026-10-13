#include "sensor.h"


/*
uint16_t getADCData(void* sensorSelection)
{
    uint16_t result;
    adcc_channel_t channel;
    
    channel = *((adcc_channel_t *) sensorSelection);
    result = ADCC_GetSingleConversion(channel);
    
    return result;
}

uint8_t readBMP280(uint8_t regadd)
{
    uint8_t ret;
    uint8_t* pRxbuffer;
    uint8_t* pTxbuffer;
    pRxbuffer = &ret;
    pTxbuffer = &regadd;
    I2C1_WriteNBytes(ADDRESS_BMP280,pTxbuffer,1);
    I2C1_ReadNBytes(ADDRESS_BMP280,pRxbuffer,1);    
    return ret;
}

void writeBMP280(uint8_t regadd, uint8_t regdata)
{
    uint8_t Txbuffer[2];
    uint8_t* pTxbuffer;
    
    Txbuffer[0] = regadd;
    Txbuffer[1] = regdata;
    pTxbuffer = &Txbuffer[0];
    I2C1_WriteNBytes(ADDRESS_BMP280,pTxbuffer,2);   
}

void InitBMP280(void)
{
    writeBMP280(CONFIG_ADDRESS,0x00);// NO IIR FILTER
    writeBMP280(CTRL_MEAS_ADDRESS,0xFC);// OVERSAMPLED X16, SLEEP
}

void readBMP280coeff(uint16_t *coeff_buff)
{    
    uint8_t Txbuffer;
    
    Txbuffer = CALIB00_ADDRESS;
    
    for(uint8_t i=0;i<26;i++)
    {
        coeff_reg[i] = readBMP280(Txbuffer+i);
    }
    adjustBMP280coeff(coeff_buff);
}

void adjustBMP280coeff(uint16_t *coeff_buff)
{
    /*
    dig_t1 = (uint16_t) (((uint16_t) coeff_reg[1] << 8) | ((uint16_t) coeff_reg[0]));
    dig_t2 = (int16_t) (((int16_t) coeff_reg[3] << 8) | ((int16_t) coeff_reg[2]));
    dig_t3 = (int16_t) (((int16_t) coeff_reg[5] << 8) | ((int16_t) coeff_reg[4]));
    dig_p1 = (uint16_t) (((uint16_t) coeff_reg[7] << 8) | ((uint16_t) coeff_reg[6]));
    dig_p2 = (int16_t) (((int16_t) coeff_reg[9] << 8) | ((int16_t) coeff_reg[8]));
    dig_p3 = (int16_t) (((int16_t) coeff_reg[11] << 8) | ((int16_t) coeff_reg[10]));
    dig_p4 = (int16_t) (((int16_t) coeff_reg[13] << 8) | ((int16_t) coeff_reg[12]));
    dig_p5 = (int16_t) (((int16_t) coeff_reg[15] << 8) | ((int16_t) coeff_reg[14]));
    dig_p6 = (int16_t) (((int16_t) coeff_reg[17] << 8) | ((int16_t) coeff_reg[16]));
    dig_p7 = (int16_t) (((int16_t) coeff_reg[19] << 8) | ((int16_t) coeff_reg[18]));
    dig_p8 = (int16_t) (((int16_t) coeff_reg[21] << 8) | ((int16_t) coeff_reg[20]));
    dig_p9 = (int16_t) (((int16_t) coeff_reg[23] << 8) | ((int16_t) coeff_reg[22]));
    
    coeff_buff[0] = (uint16_t) (((uint16_t) coeff_reg[1] << 8) | ((uint16_t) coeff_reg[0]));
    coeff_buff[1] = (int16_t) (((int16_t) coeff_reg[3] << 8) | ((int16_t) coeff_reg[2]));
    coeff_buff[2] = (int16_t) (((int16_t) coeff_reg[5] << 8) | ((int16_t) coeff_reg[4]));
    coeff_buff[3] = (uint16_t) (((uint16_t) coeff_reg[7] << 8) | ((uint16_t) coeff_reg[6]));
    coeff_buff[4] = (int16_t) (((int16_t) coeff_reg[9] << 8) | ((int16_t) coeff_reg[8]));
    coeff_buff[5] = (int16_t) (((int16_t) coeff_reg[11] << 8) | ((int16_t) coeff_reg[10]));
    coeff_buff[6] = (int16_t) (((int16_t) coeff_reg[13] << 8) | ((int16_t) coeff_reg[12]));
    coeff_buff[7] = (int16_t) (((int16_t) coeff_reg[15] << 8) | ((int16_t) coeff_reg[14]));
    coeff_buff[8] = (int16_t) (((int16_t) coeff_reg[17] << 8) | ((int16_t) coeff_reg[16]));
    coeff_buff[9] = (int16_t) (((int16_t) coeff_reg[19] << 8) | ((int16_t) coeff_reg[18]));
    coeff_buff[10] = (int16_t) (((int16_t) coeff_reg[21] << 8) | ((int16_t) coeff_reg[20]));
    coeff_buff[11] = (int16_t) (((int16_t) coeff_reg[23] << 8) | ((int16_t) coeff_reg[22]));
}

uint24_t getBMP280Data(ParamDPS422_t param)
{
    uint8_t Txbuffer;
    uint8_t Rxbuffer[3] = {0,0,0};
    
    uint24_t ret = 0;
    
    writeBMP280(CTRL_MEAS_ADDRESS,0xFD);//OSR_t:x1(001),OSR_p:x1(001),forced mode(01)
    for(uint16_t i=0;i<2000;i++)
    {
    } 
    //writeBMP280(CTRL_MEAS_ADDRESS,0xFD);//OSR_t:x1(001),OSR_p:x1(001),forced mode(01)
    
    
    if(param == TEMP) Txbuffer = TEMP_MSB_ADDRESS;
    else if(param == PRES)Txbuffer = PRESS_MSB_ADDRESS; 
    
        
    for(uint8_t i=0;i<3;i++)
    {
        Rxbuffer[i] = readBMP280(Txbuffer+i);
    }   

    ret = Rxbuffer[0];
    ret = ret << 8;        
    ret += Rxbuffer[1];
    ret = ret << 8;
    ret += Rxbuffer[2];
    ret = ret >> 4;
    
    return ret;
}

uint8_t readDPS422(uint8_t regadd)
{
    uint8_t ret;
    uint8_t* pRxbuffer;
    uint8_t* pTxbuffer;
    pRxbuffer = &ret;
    pTxbuffer = &regadd;
    I2C1_WriteNBytes(ADDRESS_DPS422,pTxbuffer,1);
    I2C1_ReadNBytes(ADDRESS_DPS422,pRxbuffer,1);    
    return ret;
}

void writeDPS422(uint8_t regadd, uint8_t regdata)
{
    uint8_t Txbuffer[2];
    uint8_t* pTxbuffer;
    
    Txbuffer[0] = regadd;
    Txbuffer[1] = regdata;
    pTxbuffer = &Txbuffer[0];
    I2C1_WriteNBytes(ADDRESS_DPS422,pTxbuffer,2);   
}

void InitDPS422(void)
{
    
    writeDPS422(PSR_CFG_ADDRESS,0x00);
    writeDPS422(TEMP_CFG_ADDRESS,0x00);
}
    
    
void readDPS422coeff(void)
{    
    uint8_t Txbuffer;
    
    Txbuffer = COEFF_REG_1_ADDRESS;
    
    for(uint8_t i=0;i<20;i++)
    {
        coeff_reg[i] = readDPS422(Txbuffer+i);
    }  
}

int24_t getDPS422Data(ParamDPS422_t param)
{
    uint8_t Txbuffer;
    uint8_t Rxbuffer[3];
    
    int24_t ret = 0;
    
    writeDPS422(MEAS_CFG_ADDRESS,MEAS_CFG_PRESTEMP);
    
    if(param == TEMP) Txbuffer = TMP_B2_ADDRESS;
    else if(param == PRES)Txbuffer = PSR_B2_ADDRESS; 
        
    for(uint8_t i=0;i<3;i++)
    {
        Rxbuffer[i] = readDPS422(Txbuffer+i);
    }   

    ret = Rxbuffer[0];
    ret = ret << 8;        
    ret += Rxbuffer[1];
    ret = ret << 8;
    ret += Rxbuffer[2];
    
    return ret;
}

uint16_t getSHTC3Data(void)
{
    uint16_t result;
    uint8_t Txbuffer[2];
    uint8_t Rxbuffer[2];
    uint8_t* pbuffer;
    Txbuffer[0] = 0x35;
    Txbuffer[1] = 0x17;    
    pbuffer = &Txbuffer[0];
    I2C1_WriteNBytes(ADDRESS_SHTC3,pbuffer,2); // Wakeup command
    
    Txbuffer[0] = 0x5C;
    Txbuffer[1] = 0x24;
    pbuffer = &Txbuffer[0];
    I2C1_WriteNBytes(ADDRESS_SHTC3,pbuffer,2); // Meas command
    
    pbuffer = &Rxbuffer[0];
    I2C1_ReadNBytes(ADDRESS_SHTC3,pbuffer,2); // Read out command
    
    
    Txbuffer[0] = 0xB0;
    Txbuffer[1] = 0x98;
    pbuffer = &Txbuffer[0];
    I2C1_WriteNBytes(ADDRESS_SHTC3,pbuffer,2); // Sleep command
    /*
    while(result == 0x0000)
    {
        result = I2C1_Read2ByteRegister(ADDRESS_SHCT3,BuffReg[0]); // Read out command
    }
    
    I2C1_Write2ByteRegister(ADDRESS_SHCT3,BuffReg[0],SLEEP_CMD); // Sleep command
    
    
    result = Rxbuffer[0] << 8;
    result += Rxbuffer[1];
    return result;
}*/