#include "Initializer.h"
#include "I2C_MPU.h"



void InitHardware(void)
{
	Chip_SetupXtalClocking();
	Chip_SYSCTL_SetFLASHAccess(FLASHTIM_100MHZ_CPU);
	SystemCoreClockUpdate();
	InitI2C();
	SysTick_Config(SystemCoreClock/1000);
}
/*
void UART0_Init(void)
{
	int pclk;
	unsigned long int Fdiv;

	pclk = SYSTEMCORECLOCK / 4;								//PCLK_UART0 se setea a 1/4 de SystemCoreClock

	PCONP0_ |= PCUART0_POWERON;								//Enciendo la UART0 (ON por default)

	// Seleccion de clock
	// 00 PCLK_peripheral = CCLK / 4 (Valor de reset)		01 PCLK_peripheral = CCLK
	// 10 PCLK_peripheral = CCLK / 2						11 PCLK_peripheral = CCLK / 8

	PCLKSEL0_ &= ~(3 << PCLK_UART0);							//PCLK_periph = CCLK/4

	U0LCR = 0x83;											//8 bits, sin paridad, 1 bit Stop, DLAB = 1
	Fdiv = (pclk / 16) / 115200;							//FR = 1, Fdiv = 162,76 Fdiv adoptado 162 (sin divisor fraccional)
	U0DLM = Fdiv / 256;										//BR calculado = 9645,06 con un Er = 0.47% - MUY BUENO
	U0DLL = Fdiv % 256;										//BR medida = 9642 con un Er = 0.44% - MUY BUENO - U0DLL = 0x82

	SetPINSEL (TX0 , FUNC_TXD0);
	SetPINSEL (RX0 , FUNC_RXD0);

	U0LCR = 0x03;											//8 bits, sin paridad, 1 bit Stop, DLAB = 0

	U0IER = U0IER | ( 1 << IER_RBR ) | ( 1 << IER_THRE );	//Habilito interrupciones por TX y RX
	ISE_UART0;												//Habilito NVIC

}

void UART0_IRQHandler(void)
{
	int dato;
	uint32_t iir;

	do
	{
		iir = U0IIR;									//IIR es reset por HW, una vez que lo lei se resetea.
		if( iir & 0x04 )		//interrupcion por recepcion
		{
			dato = U0RBR;
			PushRX(dato);
		}
		if( iir & 0x02 )		//interrupcion por transmision
		{
		dato = PopTX();
			if (dato >=0)
			{
				U0THR = dato;
			}
			else
			{
				flagTX = 0;					//el else indica buffer vacio
				//U0THR = 0;
			}
		}
	}while(!iir);
}
*/
