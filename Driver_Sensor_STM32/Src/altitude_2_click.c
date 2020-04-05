#include "altitude_2_click.h"


// reset sequence define in bufsheet
uint8_t buffer_I2C[10];
uint8_t buf[5]={RESET_CMD,PROMRead,CONVERT_D1_4096,CONVERT_D2_4096,ADCRead};


// Read the Coefficient of sensor
void init_sensor(void)
{

	uint16_t *prom = 0;
	HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[0],1,10);

	if(HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[1],1,10)==HAL_OK)
	{
		for (int var = 0; var < 8; var++)
		{
			HAL_I2C_Master_Receive(&hi2c1,DEVICE_ADDRESS_WRITE,(uint8_t*)buffer_I2C,2,10);
			prom[var] = Reverse_stock_value(0x00,buffer_I2C[1], buffer_I2C[0]);
		}
	}
	C1 = prom[0];
	C2 = prom[1];
	C3 = prom[2];
	C4 = prom[3];
	C5 = prom[4];
	C6 = prom[5];
}

uint8_t Reverse_Value(uint8_t value){
	uint8_t temp_value;
	temp_value = value & 0x0f;
	temp_value <<= 4;
	value = value & 0xf0;
	value >>=4;
	value = temp_value | value;
	return value;
}

// stock 4 value in uint8_t in uint32_t and reverse
uint32_t Reverse_stock_value(uint8_t value_23_16, uint8_t value_15_8, uint8_t value_7_0)
{
	uint32_t value_32bits = 0x000000;
	value_7_0 = Reverse_Value(value_7_0);
	value_15_8 = Reverse_Value(value_15_8);
	value_23_16 = Reverse_Value(value_23_16);

	value_32bits = value_7_0 << 16 | value_32bits;
	value_32bits = value_15_8 << 8 | value_32bits;
	value_32bits = value_23_16 | value_32bits;
	return  value_32bits;
}

// stock 4 value in uint8_t in uint32_t
uint32_t stock_value(uint8_t value_23_16, uint8_t value_15_8, uint8_t value_7_0)
{
	uint32_t value_32bits = 0x000000;

	value_32bits = value_7_0 << 16 | value_32bits;
	value_32bits = value_15_8 << 8 | value_32bits;
	value_32bits = value_23_16 | value_32bits;
	return  value_32bits;
}



// Call the function in the main
void readSensor(void)
{
	uint32_t T = 0 , Pres = 0;
	T = temperature();
	Pres = pression();
	HAL_UART_Transmit(&huart2, (uint8_t *)D1, 3, 2000);
	HAL_UART_Transmit(&huart2, (uint8_t *)D2, 3, 2000);
	uint32_t alti  = altitude();
	Send_Data(alti);
}

// Read temperature value
uint32_t temperature(void)
{
	if(HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[1],1,10)==HAL_OK){
			if(HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[4],1,10)==HAL_OK)
			{
				HAL_I2C_Master_Receive(&hi2c1,DEVICE_ADDRESS_WRITE,(uint8_t*)buffer_I2C,3,10);
				D2 = Reverse_stock_value(buffer_I2C[2],buffer_I2C[1], buffer_I2C[0]);
			}
	}
	calculate();
	return D1;
}

// Read pressure value
uint32_t pression(void)
{
	if(HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[3],1,10)==HAL_OK){
			if(HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS_WRITE,&buf[4],1,10)==HAL_OK)
			{
				HAL_I2C_Master_Receive(&hi2c1,DEVICE_ADDRESS_WRITE,(uint8_t*)buffer_I2C,3,10);
				D1 = Reverse_stock_value(buffer_I2C[2],buffer_I2C[1], buffer_I2C[0]);
			}
	}
	calculate();
	return P;
}

// Convert the coefficient with the Data Value D1 and D2 in unity wanted
void calculate(void)
{
	// calcul and name of variable define in the datasheet, this code its a library present on Github

	dT= D2 - (C5*pow(2,8));

	TEMP=(20 + (dT*C6) / (pow(2,23))); // TEMP ( 2000

	OFF=C2*pow(2,17)+dT*C4/pow(2,6);

	SENS=C1*pow(2,16)+dT*C3/pow(2,7);

	P=(((D1*SENS)/pow(2,21)-OFF)/pow(2,15));

	// perform higher order corrections
	if(TEMP<20) { // TEMP 2000

	  T2=dT*dT/pow(2,31);

	  OFF2=61*(TEMP-2000)*(TEMP-2000)/pow(2,4);

	  SENS2=2*(TEMP-2000)*(TEMP-2000);

	  if(TEMP<-1500) {

	    OFF2+=15*(TEMP+1500)*(TEMP+1500);

	    SENS2+=8*(TEMP+1500)*(TEMP+1500);
	  }
	}

	TEMP-=T2;
	OFF-=OFF2;
	SENS-=SENS2;

	P=(((D1*SENS)/pow(2,21)-OFF)/pow(2,15));
	TEMP= TEMP/100;
	P = P/100;

}

// Convert D1 and D2 in altitude
uint32_t altitude(void)
{
	uint32_t alti = 0;
	alti = 153.84615*(pow(D1,0.19) - 1)*(D2+273.15);
    return alti;

}




void TestSend_Data()
{
	uint32_t RandomValue = rand();
/*	if(RandomValue % 50)
		Send_Data(RandomValue);*/
	for (int var = 0; var < 99; ++var) {
		Send_Data(var);
	}
}

// Conversion and Send data
void Send_Data(uint32_t data)
{
	char tab[4];
	int test = (int)data;
	itoa(test,tab,10);
	HAL_UART_Transmit(&huart2, tab, 4, 2000);
	HAL_UART_Transmit(&huart2, "\r\n", 3, 2000);
}
