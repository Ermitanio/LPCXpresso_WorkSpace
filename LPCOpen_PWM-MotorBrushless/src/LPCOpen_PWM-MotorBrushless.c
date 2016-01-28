/*
 * PWM USANDO MODULO DEL LPC PARA PRENDER 3 LEDS TIPO TIFASICO, ALTERNADO (Ruben Board)
 */

//#include "board.h"
#include "chip.h"
#include "Initiations.h"

/* System oscillator rate and RTC oscillator rate */
const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

uint32_t frecuencia=0;

// MAIN PROGRAM
int main(void)
{
	uint16_t Giro = 0, Giro0 = 0, CloseLoop=0, Inertia=0xFF;

	frecuencia=Chip_Clock_GetSystemClockRate();
	//Init All
	//-----------------------------------------------------------------------------------------------
	Stop_and_Default();	//Condiciones iniciales
	InitGPIO();			//Llamo función para inicializar GPIO
	InitPWM();			//Función inicialización modulo PWM
	Start_Up_Brushless();		//Arranque del motor   **********************7777777777777777777777777777744444444444***************
	Giro=1;
	//Main Loop
	//-----------------------------------------------------------------------------------------------
	while (1)
	{
		//Lectura Pulsadores
		//-------------------------------------------------------------------------------------------
		if (Chip_GPIO_ReadPortBit(LPC_GPIO, PULS_PORT, PULS1)==0 && Chip_GPIO_ReadPortBit(LPC_GPIO, PULS_PORT, PULS2)==0)
		{
			//Detencion y valores de reinicio
			Stop_and_Default();	//Detencion del motor
			Giro = 0;			//Flag que no siga girando
			Giro0 = 0;			//Flag para arranque
			CloseLoop=0;		//Flag para lazo cerrado
			Inertia=0xFF;		//Contador para mantener velocidad hasta encontrar BEMF

			AntiRebo = REBOTE_;	//Reestablezco anti rebote
		}
		if (Chip_GPIO_ReadPortBit(LPC_GPIO, PULS_PORT, PULS1) == 0	&& AntiRebo == 0)
		{
			//Arranque Motor PWM + Period
			if (Giro0 == 0) {				//Primer pulso:
				Start_Up_Brushless();		//Arranque del motor
				Giro = 1;					//Flag que continue girando
			} else {
				if (DutyCycle > 20)
					DutyCycle = DutyCycle - 5;	//Decrementar ciclo actividad
			}
			AntiRebo = REBOTE_;	//Restablezco anti rebote
		}
		if (Chip_GPIO_ReadPortBit(LPC_GPIO, PULS_PORT, PULS2) == 0	&& AntiRebo == 0)
		{
			if (DutyCycle < 980 && Giro0 == 1)
				DutyCycle = DutyCycle + 5;	//Incremento ciclo actividad

			AntiRebo = REBOTE_;	//Restablezco anti rebote
		}
		//-------------------------------------------------------------------------------------------
		if (AntiRebo > 0)
			AntiRebo--;		//Antirebote a lo croto

		//Test PWM
		//-------------------------------------------------------------------------------------------
		if(CloseLoop==0){		//Lazo abierto
			if (Match_Cnt>=StepPeriod && Giro)
			{
				NextPWM();		//Conmutación
				Giro0 = 1;		//Flag para incrementar o decrementar duty
				Inertia--;		//Contador decreciente para encontrar BEMF
				if(Inertia==0)
					CloseLoop=1;	//Final del contador -> entro en lazo cerrado
			}
		}else{					//Lazo cerrado
			Zero_Detect();		//Detección de cruces por cero (cincronismo)
		}
		//-------------------------------------------------------------------------------------------
		//End Test
	}
	return 1;
}

//DETECTOR CRUCES POR CERO
void Zero_Detect(void)
{
	/*
	//Lectura de feedback
	//-----------------------------------------------------------------------------------------------
	CruceZero[0] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z[0], PIN_Z[0]);
	CruceZero[1] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z[1], PIN_Z[1]);
	CruceZero[2] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z[2], PIN_Z[2]);
	//Detecto flanco para realizar conmutación
	//-----------------------------------------------------------------------------------------------
	if(CruceZero0[0] != CruceZero[0]){
		NextPWM();}
	else{
		if(CruceZero0[1] != CruceZero[1]){
			NextPWM();}
		else{
			if(CruceZero0[2] != CruceZero[2]){
				NextPWM();}
	}}*/
	if(CruceZero0[0] != (LPC_GPIO2->PIN & 0x000000E0))
			NextPWM();
}

//DEFAULT
void Stop_and_Default(void)
{
	//Set Period and PWM
	//-----------------------------------------------------------------------------------------------
	StepPeriod = start.periodRange[0];
	DutyCycle = start.powerRange[0];
	DutyCycle0 = DutyCycle;
	//Shut Down All
	//-----------------------------------------------------------------------------------------------
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1, PIN_Q1, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2, PIN_Q2, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q3, PIN_Q3, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q4, PIN_Q4, 1);		//NMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q5, PIN_Q5, 0);		//PMOS
	Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q0, PIN_Q0, 1);		//NMOS
}

//ARRANQUE
void Start_Up_Brushless(void)
{
	uint32_t t = 1, dr, dPwr;

	//Drive at const rate for a few cycles to make sure rotor is synched.
	//-----------------------------------------------------------------------------------------------
	Count = 0;
	NextPWM();	//Siguiente conmutación
	while (Count < 10)				//Primeras 3 conmutaciones a período inicial (lentas) por sincronizmo
	{
		while (Match_Cnt < StepPeriod);	//Delay hasta sig conmutación
		NextPWM();						//Siguiente conmutación
	}
	//Set variables para ecuaciones de arranque
	//-----------------------------------------------------------------------------------------------
	dPwr = (start.powerRange[1] - start.powerRange[0])/start.duration; 	//Diferencia de Duty
	dr = (start.periodRange[0] -start.periodRange[1])/start.duration;

	t = 0;
	//Arranque del Motor	(Clock:25MHz, Divisor pwm:1, Ciclos pwm:1000, -> [1 Match_Cnt = 40 MicroSeg]
	//-----------------------------------------------------------------------------------------------
	while (StepPeriod > start.periodRange[1])
	{
		while (Match_Cnt < StepPeriod);//Delay hasta la siguiente conmutación (bloqueante solo durante arranque)
		NextPWM();						//Siguiente conmutación

		DutyCycle = start.powerRange[0] + t * dPwr;//Incremento Duty de manera lineal desde powerRange0 a powerRange1
		StepPeriod =start.periodRange[0] - t * dr;	//Disminuye período entre conmutaciones de manera exponencial decreciente
		t++;																					//desde periodRange0 hasta periodRange1
	}

	DutyCycle = 150;		// (150/1000)-> 15% Duty

	Chip_PWM_SetMatch(LPC_PWM1, 5, DutyCycle);
	//Chip_PWM_Reset(LPC_PWM1);
	Chip_PWM_LatchEnable(LPC_PWM1, 5, PWM_OUT_ENABLED);

}

//PWM
void NextPWM(void)
{
	//Actualizar DutyCycle
	//-----------------------------------------------------------------------------------------------
	if (DutyCycle != DutyCycle0)
	{
		Chip_PWM_SetMatch(LPC_PWM1, 5, DutyCycle);
		//Chip_PWM_Reset(LPC_PWM1);
		Chip_PWM_LatchEnable(LPC_PWM1, 5, PWM_OUT_ENABLED);
		DutyCycle0 = DutyCycle;
	}
	//Conmutaciones MOSfet
	//-----------------------------------------------------------------------------------------------
	switch (StepID)
	{
	case 0:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[2], PIN_Qb[2], 1);	//Apago Q4
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[0], PIN_Qb[0], 0);	//Prendo Q0
		break;
	case 1:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[1], PIN_Qa[1], 0);	//Apago Q3
		Cycle = 2;													//Prendo Q5
		break;
	case 2:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[0], PIN_Qb[0], 1);	//Apago Q0
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[1], PIN_Qb[1], 0);	//Prendo Q2
		break;
	case 3:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[2], PIN_Qa[2], 0);	//Apago Q5
		Cycle = 0;													//Prendo Q1
		break;
	case 4:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[1], PIN_Qb[1], 1);	//Apago Q2
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[2], PIN_Qb[2], 0);	//Prendo Q4
		break;
	default:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[0], PIN_Qa[0], 0);	//Apago Q1
		Cycle = 1;													//Prendo Q3
	}

	if (StepID > 4)	//Si StepID es mayor a 4 reseteo variable StepID
	{
		StepID = 0;
		Count++;
	} else
		StepID++;//Incremento StepID para la siguiente conmutación (6 conmutaciones)

	Match_Cnt = 0;	//Reinicio Match_Cnt

	//Estado anterior cruces zeros
	//-----------------------------------------------------------------------------------------------
	//CruceZero0[0] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z[0], PIN_Z[0]);
	//CruceZero0[1] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z[1], PIN_Z[1]);
	//CruceZero0[2] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z[2], PIN_Z[2]);
	CruceZero0[0] = LPC_GPIO2->PIN & 0x000000E0;
}

//INTERRUPT PWM
void PWM1_IRQHandler(void)
{
	//Interupción Canal 0 -> PERIODO
	//-----------------------------------------------------------------------------------------------
	if (Chip_PWM_MatchPending(LPC_PWM1, 0))	//Reviso interrupción pendiente canal PWM 0
	{
		Match_Cnt++;						//Incremento contador para el brillo
		Chip_PWM_ClearMatch(LPC_PWM1, 0);	//Limpio interrupción canal PWM 0

		//PWM sobre transistores PMOS
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[Cycle], PIN_Qa[Cycle], 1);	//Encender
	}
	//Interrupción Canal 5 -> DUTYCICLE
	//-----------------------------------------------------------------------------------------------
	if (Chip_PWM_MatchPending(LPC_PWM1, 5)) //Reviso interrupción pendiente canal PWM 5
	{
		Chip_PWM_ClearMatch(LPC_PWM1, 5);	//Limpio interrupción canal PWM 5

		//PWM sobre transistores NMOS
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[Cycle], PIN_Qa[Cycle], 0);	//Apagar
	}
}

