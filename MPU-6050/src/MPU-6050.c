/*
 *	PROYECTO MEDIDAS ELECTRÓNICAS I (2015) (UTN-FRBA)
 *
 *	Castro Miranda, Maximiliano
 *
 *	Lectura módulo MPU-6050 (Acelerometro y giroscopio)
 *	Lectura mediante I2C y transmisión a pc mediante UART
 *	Algoritmo de kalman y corrección por temperatura del sensor
 */

#include "I2C_MPU.h"
#include "MPU6050.h"
#include "Initializer.h"

uint8_t Data_Read[120]={0};		//Buffer de lectura de datos


void SysTick_Handler (void)        //ISR (Interrupt Service Routine)
{
   SysTickCnt++;
}

int main(void)
{
	uint32_t CuentaFIFO=0;
	InitHardware();
	//UART0_Init();
	reg_set_regs_inet();	//Setea todos los registros necesarios para utilizar la FIFO

	//Read_FIFO(14, &Data_Read[0]); 	//envío cantidad a leer y el puntero de almacenamiento

	while(1)
	{
		Read_FIFO(28, &Data_Read[0]); 	//envío cantidad a leer y el puntero de almacenamiento
		//ReadByte_MPU (WHO_AM_I_MPU6050, &Data_Read[0]);
		//Read_MPU (ACCEL_XOUT_H, &Data_Read[0], 14);
		//CuentaFIFO=FIFO_Count();
	}
	return 0;
}
