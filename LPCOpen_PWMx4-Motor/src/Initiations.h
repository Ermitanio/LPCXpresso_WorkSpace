/*
 * Iniciación de puertos GPIO y del PWM Crotolandia
 * PWM:
 * 	-Canal 0 -> Interrupción Habilitada	(Periodo PWM)
 * 	-Canal 5 -> Interrupción Habilitada (DutyCycle PWM)
 * 	-Muchos canales wiiii
 */
#include "Defines.h"

void InitPWM(void)
{
	uint8_t channel=0;
	//Initialize PWM peipheral, timer mode
	//-----------------------------------------------------------------------------------------------
	Chip_PWM_PrescaleSet(LPC_PWM1, 0);		//Valor preescalar=100 (divisor de clock)
	//Set match value for PWM match channel0 (frecuency)

	//-----------------------------------------------------------------------------------------------
	Chip_PWM_SetMatch(LPC_PWM1, 0, 1000);		//Establezco el valor en clock del período (canal 0) 25kHz
	Chip_PWM_MatchEnableInt(LPC_PWM1, 0);		//Habilito interrupción
	Chip_PWM_ResetOnMatchEnable(LPC_PWM1, 0);	//Reset auto
	Chip_PWM_StopOnMatchDisable(LPC_PWM1, 0);	//No stop

	//Configuro los demas canales (3 -> 6)
	for(channel=3; channel <= 6; channel++)
	{
		//Configure PWM channel edge (single) CHANNEL 5
		//-----------------------------------------------------------------------------------------------
		Chip_PWM_SetControlMode(LPC_PWM1, channel, PWM_SINGLE_EDGE_CONTROL_MODE, PWM_OUT_DISABLED);

		//Configure match value for channel 5
		//-----------------------------------------------------------------------------------------------
		Chip_PWM_SetMatch(LPC_PWM1, channel, 10);		//Establezco el valor en clock del Duty (canal 5) 1%Duty
		Chip_PWM_MatchEnableInt(LPC_PWM1, channel);		//Habilito interrupción
		Chip_PWM_ResetOnMatchDisable(LPC_PWM1, channel);	//No reset auto
		Chip_PWM_StopOnMatchDisable(LPC_PWM1, channel);	//No stop
	}
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
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q10, PIN_Q10 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q11, PIN_Q11 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q12, PIN_Q12 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q13, PIN_Q13 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q14, PIN_Q14 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q15, PIN_Q15 , SALIDA);	//Configuro el pin como salida

	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q20, PIN_Q20 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q21, PIN_Q21 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q22, PIN_Q22 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q23, PIN_Q23 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q24, PIN_Q24 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q25, PIN_Q25 , SALIDA);	//Configuro el pin como salida

	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q30, PIN_Q30 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q31, PIN_Q31 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q32, PIN_Q32 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q33, PIN_Q33 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q34, PIN_Q34 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q35, PIN_Q35 , SALIDA);	//Configuro el pin como salida

	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q40, PIN_Q40 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q41, PIN_Q41 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q42, PIN_Q42 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q43, PIN_Q43 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q44, PIN_Q44 , SALIDA);	//Configuro el pin como salida
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Q45, PIN_Q45 , SALIDA);	//Configuro el pin como salida
	//APAGAR TRANSISTORES
	//-----------------------------------------------------------------------------------------------
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q11, PIN_Q11, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q12, PIN_Q12, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q13, PIN_Q13, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q14, PIN_Q14, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q15, PIN_Q15, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q10, PIN_Q10, 1);		//NMOS

	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q21, PIN_Q21, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q22, PIN_Q22, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q23, PIN_Q23, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q24, PIN_Q24, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q25, PIN_Q25, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q20, PIN_Q20, 1);		//NMOS

	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q31, PIN_Q31, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q32, PIN_Q32, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q33, PIN_Q33, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q34, PIN_Q34, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q35, PIN_Q35, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q30, PIN_Q30, 1);		//NMOS

	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q41, PIN_Q41, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q42, PIN_Q42, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q43, PIN_Q43, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q44, PIN_Q44, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q45, PIN_Q45, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q40, PIN_Q40, 1);		//NMOS
	//CONFIGURAR ENTRADAS
	//-----------------------------------------------------------------------------------------------
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z11, PIN_Z11, 0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z12, PIN_Z12, 0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z13, PIN_Z13, 0);

	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z21, PIN_Z21, 0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z22, PIN_Z22, 0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z23, PIN_Z23, 0);

	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z31, PIN_Z31, 0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z32, PIN_Z32, 0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z33, PIN_Z33, 0);

	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z41, PIN_Z41, 0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z42, PIN_Z42, 0);
	Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_Z43, PIN_Z43, 0);
}
