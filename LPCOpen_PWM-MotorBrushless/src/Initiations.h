/*
 * Iniciación de puertos GPIO y del PWM Crotolandia
 * PWM:
 * 	-Canal 0 -> Interrupción Habilitada	(Periodo PWM)
 * 	-Canal 5 -> Interrupción Habilitada (DutyCycle PWM)
 */
#include "Defines.h"

void InitPWM(void)
{
	//Initialize PWM peipheral, timer mode
	//-----------------------------------------------------------------------------------------------
	Chip_PWM_PrescaleSet(LPC_PWM1, 0);		//Valor preescalar=100 (divisor de clock)

	//Set match value for PWM match channel0 (frecuency)
	//-----------------------------------------------------------------------------------------------
	Chip_PWM_SetMatch(LPC_PWM1, 0, 1000);		//Establezco el valor en clock del período (canal 0) 25kHz
	Chip_PWM_MatchEnableInt(LPC_PWM1, 0);		//Habilito interrupción
	Chip_PWM_ResetOnMatchEnable(LPC_PWM1, 0);	//Reset auto
	Chip_PWM_StopOnMatchDisable(LPC_PWM1, 0);	//No stop

	//Configure PWM channel edge (single) CHANNEL 5
	//-----------------------------------------------------------------------------------------------
	Chip_PWM_SetControlMode(LPC_PWM1, 5, PWM_SINGLE_EDGE_CONTROL_MODE, PWM_OUT_DISABLED);

	//Configure match value for channel 5
	//-----------------------------------------------------------------------------------------------
	Chip_PWM_SetMatch(LPC_PWM1, 5, 5);		//Establezco el valor en clock del Duty (canal 5) 1%Duty
	Chip_PWM_MatchEnableInt(LPC_PWM1, 5);		//Habilito interrupción
	Chip_PWM_ResetOnMatchDisable(LPC_PWM1, 5);	//No reset auto
	Chip_PWM_StopOnMatchDisable(LPC_PWM1, 5);	//No stop

	//Reset and Start Counter
	//-----------------------------------------------------------------------------------------------
	Chip_PWM_Reset(LPC_PWM1);

	//Start PWM
	//-----------------------------------------------------------------------------------------------
	Chip_PWM_Enable(LPC_PWM1);

	//Enable PWM interrupt
	//-----------------------------------------------------------------------------------------------
	NVIC_EnableIRQ(PWM1_IRQn);
}

void InitGPIO(void)
{
	//CONFIGURAR SALIDAS
	//-----------------------------------------------------------------------------------------------
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q0, PIN_Q0 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q1, PIN_Q1 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q2, PIN_Q2 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q3, PIN_Q3 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q4, PIN_Q4 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q5, PIN_Q5 , SALIDA);	//Configuro el pin como salida
	//APAGAR TRANSISTORES
	//-----------------------------------------------------------------------------------------------
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1, PIN_Q1, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2, PIN_Q2, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q3, PIN_Q3, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q4, PIN_Q4, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q5, PIN_Q5, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q0, PIN_Q0, 1);		//NMOS
	//CONFIGURAR ENTRADAS
	//-----------------------------------------------------------------------------------------------
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z1, PIN_Z1, 0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z2, PIN_Z2, 0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z3, PIN_Z3, 0);
}
