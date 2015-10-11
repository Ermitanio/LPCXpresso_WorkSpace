/*
 * PWM USANDO MODULO DEL LPC PARA PRENDER 3 LEDS TIPO TIFASICO, ALTERNADO (Ruben Board)
 */

#include "board.h"
#include "chip.h"
#include "Initiations.h"

// MAIN PROGRAM
int main(void)
{
	uint8_t Giro = 0, Giro0 = 0, CloseLoop=0, Inertia=0xFF;
	//Init All
	//-----------------------------------------------------------------------------------------------
	Stop_and_Default();	//Condiciones iniciales
	InitGPIO();			//Llamo función para inicializar GPIO
	InitPWM();			//Función inicialización modulo PWM
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
				//Inertia--;		//Contador decreciente para encontrar BEMF
				//if(Inertia==0)
				//	CloseLoop=1;	//Final del contador -> entro en lazo cerrado
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
	}}
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
	float r0, dr, dPwr, frac;
	uint32_t t = 1;

	//Drive at const rate for a few cycles to make sure rotor is synched.
	//-----------------------------------------------------------------------------------------------
	Count = 0;
	NextPWM();	//Siguiente conmutación
	while (Match_Cnt < 25000);		//Delay de 1Seg para asegurar sincronizmo y arranque
	while (Count < 3)				//Primeras 3 conmutaciones a período inicial (lentas) por sincronizmo
	{
		while (Match_Cnt < StepPeriod);	//Delay hasta sig conmutación
		NextPWM();						//Siguiente conmutación
	}
	//Set variables para ecuaciones de arranque
	//-----------------------------------------------------------------------------------------------
	dPwr = start.powerRange[1] - start.powerRange[0]; 	//Diferencia de Duty
	r0 = 1.0 / start.periodRange[0];
	dr = (1.0 / start.periodRange[1]) - r0;

	Count = 0;
	t = 1;
	//Arranque del Motor	(Clock:25MHz, Divisor pwm:1, Ciclos pwm:1000, -> [1 Match_Cnt = 40 MicroSeg]
	//-----------------------------------------------------------------------------------------------
	while (StepPeriod > start.periodRange[1])
	{
		while (Match_Cnt < StepPeriod);//Delay hasta la siguiente conmutación (bloqueante solo durante arranque)
		NextPWM();						//Siguiente conmutación

		frac = (float) (t / start.duration);//Fracción de decrecimiento del período
		if (frac > 1)
			frac = 1;
		DutyCycle = start.powerRange[1] - frac * dPwr;//Incremento Duty de manera lineal desde powerRange0 a powerRange1
		StepPeriod = (frac > 0.999) ? start.periodRange[1] : (long) (1.0 / (r0 + frac * dr));	//Disminuye período entre conmutaciones de manera exponencial decreciente
		t++;																					//desde periodRange0 hasta periodRange1
	}

	DutyCycle = 300;		// (75/500)-> 15% Duty
	//DutyCycle = 0.5 * (start.powerRange[0] + start.powerRange[1]); // Reduce Duty al final del arranque

	Chip_PWM_SetMatch(LPC_PWM1, 5, DutyCycle);
	Chip_PWM_Reset(LPC_PWM1);
}

//PWM
void NextPWM(void)
{
	//Actualizar DutyCycle
	//-----------------------------------------------------------------------------------------------
	//if (DutyCycle != DutyCycle0)

		Chip_PWM_SetMatch(LPC_PWM1, 5, DutyCycle);
		Chip_PWM_Reset(LPC_PWM1);
		//DutyCycle0 = DutyCycle;

	//Conmutaciones MOSfet
	//-----------------------------------------------------------------------------------------------
	switch (StepID)
	{
	case 0:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[2], PIN_Qa[2], 0);	//Apago Q4
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[0], PIN_Qa[0], 1);	//Prendo Q0
		break;
	case 1:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[1], PIN_Qb[1], 1);	//Apago Q3 inv
		Cycle = 2;													//Prendo Q5
		break;
	case 2:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[0], PIN_Qa[0], 0);	//Apago Q0
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[1], PIN_Qa[1], 1);	//Prendo Q2
		break;
	case 3:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[2], PIN_Qb[2], 1);	//Apago Q5
		Cycle = 0;													//Prendo Q1
		break;
	case 4:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[1], PIN_Qa[1], 0);	//Apago Q2
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qa[2], PIN_Qa[2], 1);	//Prendo Q4
		break;
	default:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[0], PIN_Qb[0], 1);	//Apago Q1
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
	CruceZero0[0] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z[0], PIN_Z[0]);
	CruceZero0[1] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z[1], PIN_Z[1]);
	CruceZero0[2] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z[2], PIN_Z[2]);
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
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[Cycle], PIN_Qb[Cycle], 0);	//Encender (invertido por el tbj emisor común)
	}
	//Interrupción Canal 5 -> DUTYCICLE
	//-----------------------------------------------------------------------------------------------
	if (Chip_PWM_MatchPending(LPC_PWM1, 5)) //Reviso interrupción pendiente canal PWM 5
	{
		Chip_PWM_ClearMatch(LPC_PWM1, 5);	//Limpio interrupción canal PWM 5

		//PWM sobre transistores NMOS
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Qb[Cycle], PIN_Qb[Cycle], 1);	//Apagar (invertido por el tbj emisor común)
	}
}
