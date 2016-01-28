//#include "board.h"
#include "chip.h"
#include "ssp_17xx_40xx.h"
#include "SPI_RF.h"
#include "nRF24L01.h"
#include "RF24.h"

#define TICKRATE_HZ1 1	// 1 ticks per second

volatile unsigned long SysTickCnt; //Contador de interrupciones

void SysTick_Handler (void);       //Prototipo de la ISR
void Delay (unsigned long tick);   //Prototipo de la función Delay

int main(void)
{
	//char trans[6]={0,0,0,0,0,0}, buff_leer[6]={0,0,0,0,0,0}, i=0xFF;
	uint8_t flag=0,led_on[4]={0xAA,0xBB,0xCC,0xDD}, led_off[4]={0xEE,0xFF,0x01,0x23}, led_up[4]={0x01,0x23,0x45,0x67}, led_down[4]={0x76,0x54,0x32,0x10};// i=0xFF;
	//uint32_t data=0;
	uint8_t addresses1[5] = {"edoN1"}, addresses2[5] = {"edoN2"};

	InitHardware();
	InitSPI ();

	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 11, 0); //Led stick
	Chip_GPIO_WriteDirBit(LPC_GPIO, 2, 12, 0); //Puerto CE


	//SystemCoreClockUpdate();
	SysTick_Config(12000000 / TICKRATE_HZ1);
	//SysTick_Config(SystemCoreClock / TICKRATE_HZ1);

	begin();
	setPALevel(RF24_PA_LOW);
	openWritingPipe(&addresses1[0]);
	openReadingPipe(1,&addresses2[0]);	//1Node: Transmite paquetes el tx por este pide (addres)

	stopListening();

	while(1)
	{/*
		 if(!Chip_GPIO_ReadPortBit(LPC_GPIO, 2, 11) && !Chip_GPIO_ReadPortBit(LPC_GPIO, 2, 12) && flag)
		 {
			 write(&led_off[0],4);
			 flag=0;
		 }

		 if(!Chip_GPIO_ReadPortBit(LPC_GPIO, 2, 11) && flag)
			 write(&led_up[0],4);
		 if(!Chip_GPIO_ReadPortBit(LPC_GPIO, 2, 12) && flag)
			 write(&led_down[0],4);

		 Delay(5);

		 if(!Chip_GPIO_ReadPortBit(LPC_GPIO, 2, 11) && !Chip_GPIO_ReadPortBit(LPC_GPIO, 2, 12) && !flag)
		 {
			 write(&led_on[0],4);
		 	 flag=1;
		 }
		 Delay(5);*/
		if(!Chip_GPIO_ReadPortBit(LPC_GPIO, 2, 11))
			 write(&led_on[0],4);
		Delay(2);
		 if(!Chip_GPIO_ReadPortBit(LPC_GPIO, 2, 12))
			 write(&led_off[0],4);
		 Delay(2);
	}
    return 0 ;
}

void SysTick_Handler (void)        //ISR (Interrupt Service Routine)
{
   SysTickCnt++;
}

void Delay (unsigned long tick) //Función Delay
{
   unsigned long systickcnt;

   systickcnt = SysTickCnt;
   while ((SysTickCnt - systickcnt) < tick);
}
