#include "device.h"

/**
 * Send infomation about this device.
 * 
 * @param sensor_info
 */
void sendSensorInfo(sensor_info_t* sensor_info)
{
    
    info_packet_t packet = {
        .preamble = RESPONSE_START, 
        .codename = DEVICE_CODENAME1,
    };    
    for(int idx = 0; idx < INFO_CODENAME_LENGTH; idx++)
        packet.codename[idx] = sensor_info->codename[idx];
    
        
	sendPacket((char*)&packet, INFO_PACKET_LENGTH);
}

void sendSensorCoeff(uint8_t idx)
{
    cal_packet_t packet = {        
        .preamble = RESPONSE_START, 
        .coef = 0,
    };
    
        if((idx == 0)||(idx == 3))
        {
            uint24ToString((uint16_t)dig[idx], &packet.coef);
        }
        else
        {
            itoa(dig[idx], &packet.coef);
        }
    
	sendPacket((char*)&packet, (RESPONSE_START_LENGTH+stringlen(packet.coef)));
}


/**
 * Send data through UART interface, LSB-first
 * 
 * @param sensor_data. Data to be sent
 */
void sendSensorData(uint16_t sensor_data)
{
    data_packet_t packet = {.preamble = RESPONSE_START};

    //int16ToString(sensor_data, packet.value);
    uint24ToString((uint24_t)sensor_data, packet.value);
            
    sendPacket((char*)&packet, (RESPONSE_START_LENGTH+stringlen(packet.value)));
}

void sendSensorData24bits(int24_t sensor_data)
{
    data_packet24_t packet = {.preamble = RESPONSE_START};
    
    //int24ToString(sensor_data1, packet.value);
    uint24ToString(sensor_data, packet.value);
                
    sendPacket((char*)&packet, (RESPONSE_START_LENGTH+stringlen(packet.value)));
}

/**
 * 
 * @param str
 * @param buf
 * @param byte_count
 * @return 
 */
int stringCompare(const char* str, const char* buf, int byte_count)
{
	int idx = 0;

	for(idx = 0; idx < byte_count; idx++)
	{
		if(*str != *buf)
			return -1;
		else
		{
			str++;
			buf++;
		}
	}
	return 0;
}

/**
 * 
 * @param str
 * @return 
 */
uint8_t stringlen(const char* str)
{
	uint8_t idx = 0;

	for(idx = 0; idx < 255; idx++)
	{
		if(*str == 0)
			return idx;
        str++;
	}
	return -1;
}


/**
 * 
 * @param value
 * @param data
 */
void int16ToString(uint16_t value, char *data)
{
    uint16_t divisor = 10000; // Max divider
    uint16_t dividend = value;
    uint16_t result;
    bool flag_0 = 0;
    
    for(int i=0; i<5; i++)
    {
        result = dividend / divisor;
        if (!((result == 0)&&(flag_0 == 0)))
        {
            *data = '0'+result;
            data++;
            flag_0 = 1;
        }        
        dividend = dividend % divisor;
        divisor /= 10;
    }
    if(value == 0)
    {
        *data = '0';
    }
}

void uint24ToString(uint24_t value, char *data)
{
    uint24_t divisor = 10000000; // Max divider
    uint24_t dividend = value;
    uint24_t result;
    bool flag_0 = 0;
    
    for(int i=0; i<8; i++)
    {
        result = dividend / divisor;
        if (!((result == 0)&&(flag_0 == 0)))
        {
            *data = '0'+result;
            data++;
            flag_0 = 1;
        }        
        dividend = dividend % divisor;
        divisor /= 10;
    }
    if(value == 0)
    {
        *data = '0';
    }
}

int8_t itoa(int16_t value,char *data)
     {
        int16_t count=0,temp;

        if(value==0)
        {   
            *data='0';
            return 1;
        }

        if(value<0)
        {
            value*=(-1);    
            *data++='-';
            count++;
        }
        for(temp=value;temp>0;temp/=10,data++);
        *data='\0';
        for(temp=value;temp>0;temp/=10)
        {
            *--data=temp%10+'0';
            count++;
        }
        return count;
     }
/**
 * 
 * @param string
 * @param strlen
 * @return 
 */
//uint16_t stringToInt16(char* string, uint16_t strlen)
//{
//    uint16_t result = 0;
//    uint16_t multiple = 1;
//    
//    for(int i=strlen; i>0; --i)
//    {
//        result = (*(string+i) - '0') * multiple;
//        multiple *= 10;
//    }
//    return result;
//}

