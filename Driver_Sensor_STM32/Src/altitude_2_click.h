
#ifndef SENSORDEFS_H_
#define SENSORDEFS_H_

#include "main.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>


/* Macro define by datasheet constructor
 ********************************************************************************/
#define RESET_CMD				0x1E

// pressure Resolution -> D1
#define CONVERT_D1_256 			0x40
#define CONVERT_D1_512 			0x42
#define CONVERT_D1_1024			0x44
#define CONVERT_D1_2048			0x46
#define CONVERT_D1_4096			0x48

// temperature Resolution -> D2
#define CONVERT_D2_256			0x50//1 ms
#define CONVERT_D2_512			0x52//3 ms
#define CONVERT_D2_1024			0x54//4 ms
#define CONVERT_D2_2048			0x56//6	ms
#define CONVERT_D2_4096			0x58//10 ms


// Command of MS5607-02BA03
#define ADCRead					0x00
#define PROMRead				0xA0
#define PROM_READ_SENS_CMD      0xA2
#define PROM_READ_OFF_CMD       0xA4
#define PROM_READ_TCS_CMD       0xA6
#define PROM_READ_TCO_CMD       0xA8
#define PROM_READ_TREF_CMD      0xAA
#define PROM_READ_TEMPSENS_CMD  0xAC

//Device Address (1110 110)2 = (EC)16
/*
#define DEVICE_ADDRESS_READ			0x76
#define DEVICE_ADDRESS_WRITE		0x77
*/

#define DEVICE_ADDRESS_READ			0xEE
#define DEVICE_ADDRESS_WRITE		0xEE




/* call the config I2C for the communication with the sensor and UART for send to other PC
 ********************************************************************************/
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
/* ********************************************************************************/


/* Function Declaration
********************************************************************************/
void init_sensor(void);
void reset_sequence(void);
void Send_Data(uint32_t data);
uint8_t Reverse_Value(uint8_t value);
uint32_t Reverse_stock_value(uint8_t value_23_16, uint8_t value_15_8, uint8_t value_7_0);
uint32_t stock_value(uint8_t , uint8_t, uint8_t);

void readSensor(void);
uint32_t pression(void);
uint32_t temperature(void);
void calculate(void);
uint32_t altitude(void);

/* ********************************************************************************/


/* Buffer of commande altitude and buffer I2C
 ********************************************************************************/

/* ********************************************************************************/


/* variable calculator for the temperature calculator, define by the constructor
 ********************************************************************************/
uint16_t C1,C2,C3,C4,C5,C6;

uint32_t D1,D2;

int32_t dT, TEMP, T2;

int64_t OFF,OFF2, SENS, SENS2, P;

/* Information in Datasheet :
 * C1 Pressure sensitivity | SENST1
 * C2 Pressure offset | OFFT1
 * C3 Temperature coefficient of pressure sensitivity | TCS
 * C4 Temperature coefficient of pressure offset | TCO
 * C5 Reference temperature | TREF
 * C6 Temperature coefficient of the temperature | TEMPSENS
 * D1 Digital pressure value
 * D2 Digital temperature value
 * dT Difference between actual and reference temperature
 * TEMP Actual temperature
 * OFF Offset at actual temperature
 * SENS Sensitivity at actual temperature
 * P Temperature compensated pressure
 *
 */
/* ********************************************************************************/




#endif /* MS5607-02BA03_H_ */

