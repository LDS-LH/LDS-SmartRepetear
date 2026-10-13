/* 
 * File:   sensor.h
 * Author: Fabio
 *
 * Created on 21 de octubre de 2019, 13:03
 */

#ifndef SENSOR_H
#define	SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mcc_generated_files/mcc.h"
#include "communication.h"

#define CODENAME_LENGTH  5      // Number of characters for sensor codename
#define N_COEF  12    

uint8_t coeff_reg[26]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'};
uint16_t dig[N_COEF];
/*
uint16_t dig_t1;
int16_t dig_t2;
int16_t dig_t3;
uint16_t dig_p1;
int16_t dig_p2;
int16_t dig_p3;
int16_t dig_p4;
int16_t dig_p5;
int16_t dig_p6;
int16_t dig_p7;
int16_t dig_p8;
int16_t dig_p9;
*/
int32_t t_fine;
typedef uint16_t sensor_data_t;

typedef enum{
    TEMP,
    PRES,
}ParamDPS422_t;

typedef struct
{
	char codename[CODENAME_LENGTH];
	char address[3];
	uint16_t frequency;
    uint8_t ADCChannel;
}sensor_info_t;

typedef struct {
	sensor_info_t sensor_info;
	uint16_t (*sense)(void* sensor_conf);
}sensor_t;

uint16_t getADCData(void* sensorSelection);

uint8_t readBMP280(uint8_t regadd);
void writeBMP280(uint8_t regadd, uint8_t regdata);
void InitBMP280(void);
void readBMP280coeff(uint16_t *coeff_buff);
void adjustBMP280coeff(uint16_t *coeff_buff);
uint24_t getBMP280Data(ParamDPS422_t param);

uint8_t readDPS422(uint8_t regadd);
void writeDPS422(uint8_t regadd, uint8_t regdata);
void InitDPS422(void);
void readDPS422coeff(void);
int24_t getDPS422Data(ParamDPS422_t param);

uint16_t getSHTC3Data(void);

extern sensor_t sensor1;
extern sensor_t sensor2;
extern sensor_t sensor3;

#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_H */

