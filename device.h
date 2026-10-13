/* 
 * File:   device.h
 * Author: Fabio
 *
 * Created on 21 de octubre de 2019, 13:24
 */

#ifndef DEVICE_H
#define	DEVICE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "sensor.h"
#include "communication.h"

#define BCAST_ADDRESS   0     // Broadcast address. All device must response to this address
/****************** Device 1 specific parameters ******************************/    
#define DEVICE_ADDRESS1     "001"    // Device specific address (0-255)
#define DEVICE_CODENAME1	"TE001"//
/****************** Device 2 specific parameters ******************************/    
#define DEVICE_ADDRESS2     "002"    // Device specific address (0-255)
#define DEVICE_CODENAME2	"HU002"// 
/****************** Device 3 specific parameters ******************************/    
#define DEVICE_ADDRESS3     "007"    // Device specific address (0-255)
#define DEVICE_CODENAME3	"PA007"// 
/****************** Device 4 specific parameters ******************************/    
#define DEVICE_ADDRESS4     "008"    // Device specific address (0-255)
#define DEVICE_CODENAME4	"TE008"// 
/************************ Command packet parameters *************************/    
//#define CMD_START			"LDS#"
#define CMD_START			"L#"
#define CMD_START_LENGTH	2
#define CMD_ADDRESS_LENGTH	3
#define CMD_COMMAND_LENGTH	1
#define CMD_PACKET_LENGTH 	sizeof(cmd_packet_fields_t)
#define CMD_END             '\n'
/************************ Response packet parameters *************************/    
//#define RESPONSE_START          "LDS>"
#define RESPONSE_START          "L>"
#define RESPONSE_START_LENGTH   2
/* Sensor information packet parameters */    
#define INFO_CODENAME_LENGTH    5 
#define INFO_FREQ_LENGTH        2 
#define INFO_PACKET_LENGTH      sizeof(info_packet_t)
#define CAL_PACKET_LENGTH      sizeof(cal_packet_t)
/* Data packet parameters */    
#define DATA_LENGTH     6 
#define DATA_24LENGTH     8
#define DATA_PACKET_LENGTH      sizeof(data_packet_t) 
#define DATA_PACKET24_LENGTH      sizeof(data_packet24_t)
/**************************** Device commands ********************************/    
#define CMD_GET_INFO    'I' // Device will response with all its information 
#define CMD_SENSE       'S' // Device will take sensor measurement   
#define CMD_GET_DATA    'D' // Device will communicate its last measured data
#define CMD_GET_COEFF   'C' // Device will communicate its coeff
 
typedef struct {
	char start[CMD_START_LENGTH];
	char address[CMD_ADDRESS_LENGTH];
	char command;
} cmd_packet_fields_t;

typedef union
{
	char cmd_packet[CMD_PACKET_LENGTH];
	cmd_packet_fields_t cmd_packet_fields;
} cmd_packet_t;

typedef struct
{
    char preamble[RESPONSE_START_LENGTH];
	char codename[CODENAME_LENGTH];
} info_packet_t;


typedef struct
{
    char preamble[RESPONSE_START_LENGTH];
	char coef[6];
} cal_packet_t;

typedef struct
{
    char preamble[RESPONSE_START_LENGTH];
	char value[DATA_LENGTH];
} data_packet_t;

typedef struct
{
    char preamble[RESPONSE_START_LENGTH];
	char value[DATA_24LENGTH];
    char end;
} data_packet24_t; 

int receiveCommand(char* cmd_buf);
void sendSensorInfo(sensor_info_t* sensor_info);
void sendSensorCoeff(uint8_t idx);
void sendSensorData(uint16_t sensor_data);
void sendSensorData24bits(int24_t sensor_data);
int stringCompare(const char* str, const char* buf, int byte_count);
uint8_t stringlen(const char* str);
void int16ToString(uint16_t value, char *data);
void uint24ToString(uint24_t value, char *data);
int8_t itoa(int16_t value,char *data);


#ifdef	__cplusplus
}
#endif

#endif	/* DEVICE_H */

