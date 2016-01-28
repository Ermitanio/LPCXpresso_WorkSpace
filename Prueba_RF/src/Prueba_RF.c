//nRF24L01+
//#include "board.h"
#include "chip.h"
#include "ssp_17xx_40xx.h"
#include "SPI_RF.h"
#include "nRF24L01.h"
#include "RF24.h"

int main(void)
{
	//char trans[6]={0,0,0,0,0,0}, buff_leer[6]={0,0,0,0,0,0}, i=0xFF;
	uint8_t data_led[4]={0,0,0,0}, estado[6]={0,0,0,0,0,0}, i=0xFF;
	uint32_t data=0;
	uint8_t addresses1[5] = {"edoN1"}, addresses2[5] = {"edoN2"};

	InitHardware();
	InitSPI ();

	begin();
	setPALevel(RF24_PA_LOW);
	openWritingPipe(&addresses2[0]);
	openReadingPipe(1,&addresses1[0]);	//1Node: Transmite paquetes el tx por este pide (addres)

	startListening();

	while(1)
	{
		 if(available())
		 {
			 read( &data_led[0], 4 );
			 data=data_led[0];
			 data=(data<<8)|data_led[1];
			 data=(data<<8)|data_led[2];
			 data=(data<<8)|data_led[3];
		 }
		 if(data == 0xAABBCCDD)
			 Chip_GPIO_SetPinOutLow(LPC_GPIO, 0, 22);
		 if(data == 0xEEFF0123)
			 Chip_GPIO_SetPinOutHigh(LPC_GPIO, 0, 22);
	}

    return 0 ;
}
