#include "altitude_2_click.h"

uint32_t buffer_I2C[10];
// reset sequence define in bufsheet




void init_sensor(void)
{

	HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[0],1,10);
	while(buffer_I2C[0]==0){
		if(HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[1],1,10)==HAL_OK)
			HAL_I2C_Master_Receive(&hi2c1,DEVICE_ADDRESS_WRITE,(uint8_t*)buffer_I2C,16,10);


	}
	HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[2],1,10);
	HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[3],1,10);
}
void stock_value(uint32_t value_32bits)
{
	value_7_0 = (uint8_t)value_32bits&MASK_32BITS;
	value_15_8 = (value_32bits << 8) & MASK_32BITS;
	value_23_16= (value_32bits << 16) & MASK_32BITS;
}
uint32_t buffer_stock_32bits(uint8_t size,uint8_t var)
{
	uint32_t test = 0x00000000;
	for (var = 1; var < size; ++var)
		test = ((test << 8) + buf[var]);
	return test;
}



uint8_t reponse;
int32_t readSensor(void)
{
	if(HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[2],1,10)==HAL_OK)
	{
		if(HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[4],1,10)==HAL_OK)
		{
			HAL_I2C_Master_Receive(&hi2c1,DEVICE_ADDRESS_WRITE,(uint8_t*)buffer_I2C,4,10);
			D1 = buffer_I2C[0];
			HAL_I2C_Master_Receive(&hi2c1,DEVICE_ADDRESS_WRITE,(uint8_t*)buffer_I2C,4,10);
			D2 = buffer_I2C[0];
		}
	}
	if(HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[3],1,10)==HAL_OK){
		if(HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[4],1,10)==HAL_OK)
		{
			HAL_I2C_Master_Receive(&hi2c1,DEVICE_ADDRESS_WRITE,(uint8_t*)buffer_I2C,4,10);
			D1 = buffer_I2C[0];
			HAL_I2C_Master_Receive(&hi2c1,DEVICE_ADDRESS_WRITE,(uint8_t*)buffer_I2C,4,10);
			D2 = buffer_I2C[0];
		}
	}
	reponse = buffer_I2C[3];
	D1 = buffer_I2C[0];
	return D1;
}

int32_t temperature(void)
{
	TEMP /= 10;
	D1 = (D1/10)- 20;
	if(TEMP<20)
	{
		T2=((dT*dT)/2147483648);
		OFF2 = (61*pow((TEMP-2000),2)/16);
		SENS2=2*pow((TEMP-2000),2);
		if(TEMP<-15)
		{
			OFF2=OFF2+15*pow((TEMP+1500),2);
			SENS2=SENS2+8*pow((TEMP+1500),2);
		}
	}else{
		T2=0,OFF2=0,SENS2=0;
	}

	TEMP=TEMP-T2;//actual temperature
	OFF=OFF-OFF2;//offset temperature
	SENS=SENS-SENS2;//Sensivity of temperature
	return TEMP;
}


void sensor_altitude(void)
{

	TEMP = readSensor();
	TEMP = temperature();
	HAL_UART_Transmit(&huart2,(uint8_t*)buffer_I2C[1], sizeof(buffer_I2C[1]), 100);

}



