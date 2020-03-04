
#ifndef SENSORDEFS_H_
#define SENSORDEFS_H_

#include "main.h"
#include "math.h"


// Macro define by datasheet constructor
extern UART_HandleTypeDef huart2;

#define RESET_CMD				0x1E

// pressure -> D1
#define CONVERT_D1_256 			0x40
#define CONVERT_D1_512 			0x42
#define CONVERT_D1_1024			0x44
#define CONVERT_D1_2048			0x46
#define CONVERT_D1_4096			0x48

// temperature -> D2
#define CONVERT_D2_256			0x50
#define CONVERT_D2_512			0x52
#define CONVERT_D2_1024			0x54
#define CONVERT_D2_2048			0x56
#define CONVERT_D2_4096			0x58

#define ADCRead					0x00
#define PROMRead				0xA0
#define PROM_READ_SENS_CMD      0xA2;
#define PROM_READ_OFF_CMD       0xA4;
#define PROM_READ_TCS_CMD       0xA6;
#define PROM_READ_TCO_CMD       0xA8;
#define PROM_READ_TREF_CMD      0xAA;
#define PROM_READ_TEMPSENS_CMD  0xAC;



//Device Address (1110 110)2 = (EC)16
// 11101100 write
// 11101101	read
//111011Cx
#define MASK_32BITS				0x00000011

#define DEVICE_ADDRESS_READ			0xEE
#define DEVICE_ADDRESS_WRITE		0xEE



// call the config spi2 for use

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;

HAL_StatusTypeDef aknowledge;

void init_sensor(void);
void reset_sequence(void);
int32_t readSensor(void);
int32_t temperature(void);
void write_i2c_sensor(void);
uint32_t buffer_stock_32bits(uint8_t,uint8_t);
uint32_t read_sensor_test(void);


uint8_t value_23_16 = 0,value_15_8 = 0,value_7_0 = 0;

uint8_t buf[5]={RESET_CMD,PROMRead,CONVERT_D1_256,CONVERT_D2_4096,ADCRead};

// variable calculator for the temperature calculator, define by the constructor
uint16_t C1,C2,C3,C4,C5,C6;

uint32_t D1,D2;

int32_t dT, TEMP, T2;

int64_t OFF,OFF2, SENS, SENS2, P;




#endif /* MS5607-02BA03_H_ */

