/*
 * PWM USANDO MODULO DEL LPC PARA PRENDER 3 LEDS TIPO TIFASICO, ALTERNADO (Ruben Board)
 */

#include "board.h"
#include "chip.h"
#include "Initiations.h"

// MAIN PROGRAM
int main(void)
{
	Board_SystemInit();
	uint8_t Giro = 0, Giro0 = 0, CloseLoop=0, Inertia=0xFF;
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
			if (Match_Cnt1>=StepPeriod && Giro)
			{
				NextPWM1();		//Conmutación
				NextPWM2();
				Giro0 = 1;		//Flag para incrementar o decrementar duty
				Inertia--;		//Contador decreciente para encontrar BEMF
				if(Inertia==0)
					CloseLoop=1;	//Final del contador -> entro en lazo cerrado
			}
		}else{					//Lazo cerrado
			//Zero_Detect1();		//Detección de cruces por cero (cincronismo)
			//Zero_Detect2();
			if(Conmutar1){
				NextPWM1();
				Conmutar1=0;
			}
			if(Conmutar2){
				NextPWM2();
				Conmutar2=0;
			}
		}
		//-------------------------------------------------------------------------------------------
		//End Test
	}
	return 1;
}

//DETECTOR CRUCES POR CERO
void Zero_Detect1(void)
{/*
	//Lectura de feedback
	//-----------------------------------------------------------------------------------------------
	CruceZero1[0] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z1[0], PIN_Z1[0]);
	CruceZero1[1] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z1[1], PIN_Z1[1]);
	CruceZero1[2] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z1[2], PIN_Z1[2]);
	//Detecto flanco para realizar conmutación
	//-----------------------------------------------------------------------------------------------
	if(CruceZero01[0] != CruceZero1[0]){
		NextPWM1();}
	else{
		if(CruceZero01[1] != CruceZero1[1]){
			NextPWM1();}
		else{
			if(CruceZero01[2] != CruceZero1[2]){
				NextPWM1();}
	}}*/
	if(CruceZero01[0] != (LPC_GPIO2->PIN & 0x000000E0))
					NextPWM1();
}

void Zero_Detect2(void)
{/*
	//Lectura de feedback
	//-----------------------------------------------------------------------------------------------
	CruceZero2[0] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z2[0], PIN_Z2[0]);
	CruceZero2[1] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z2[1], PIN_Z2[1]);
	CruceZero2[2] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z2[2], PIN_Z2[2]);
	//Detecto flanco para realizar conmutación
	//-----------------------------------------------------------------------------------------------
	if(CruceZero02[0] != CruceZero2[0]){
		NextPWM2();}
	else{
		if(CruceZero02[1] != CruceZero2[1]){
			NextPWM2();}
		else{
			if(CruceZero02[2] != CruceZero2[2]){
				NextPWM2();}
	}}*/
	if(CruceZero02[0] != (LPC_GPIO->PIN & 0x0000C000)){
			NextPWM2();}
		else{
			if(CruceZero02[1] != (LPC_GPIO2->PIN & 0x00000100)){
				NextPWM2();}}
}

//DEFAULT
void Stop_and_Default(void)
{
	//Set Period and PWM
	//-----------------------------------------------------------------------------------------------
	StepPeriod = start.periodRange[0];
	DutyCycle = start.powerRange[0];
	DutyCycle01 = DutyCycle;
	DutyCycle02 = DutyCycle;
	//Shut Down All
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
}

//ARRANQUE
void Start_Up_Brushless(void)
{
	uint32_t t = 1, dr, dPwr;

		//Drive at const rate for a few cycles to make sure rotor is synched.
		//-----------------------------------------------------------------------------------------------
		Count = 0;
		NextPWM1();	//Siguiente conmutación
		NextPWM2();
		while (Count < 10)				//Primeras 3 conmutaciones a período inicial (lentas) por sincronizmo
		{
			while (Match_Cnt1 < StepPeriod);	//Delay hasta sig conmutación
			NextPWM1();						//Siguiente conmutación
			NextPWM2();
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
			while (Match_Cnt1 < StepPeriod);//Delay hasta la siguiente conmutación (bloqueante solo durante arranque)
			NextPWM1();						//Siguiente conmutación
			NextPWM2();

			DutyCycle = start.powerRange[0] + t * dPwr;//Incremento Duty de manera lineal desde powerRange0 a powerRange1
			StepPeriod =start.periodRange[0] - t * dr;	//Disminuye período entre conmutaciones de manera exponencial decreciente
			t++;																					//desde periodRange0 hasta periodRange1
		}

		DutyCycle = 150;		// (150/1000)-> 15% Duty

		Chip_PWM_SetMatch(LPC_PWM1, 5, DutyCycle);
		Chip_PWM_SetMatch(LPC_PWM1, 6, DutyCycle);
		//Chip_PWM_Reset(LPC_PWM1);
		Chip_PWM_LatchEnable(LPC_PWM1, 5, PWM_OUT_ENABLED);
		Chip_PWM_LatchEnable(LPC_PWM1, 6, PWM_OUT_ENABLED);
}

//PWM
void NextPWM1()
{
	//Actualizar DutyCycle
	//-----------------------------------------------------------------------------------------------
	if (DutyCycle != DutyCycle01)
	{
		Chip_PWM_SetMatch(LPC_PWM1, 5, DutyCycle);
		//Chip_PWM_Reset(LPC_PWM1);
		Chip_PWM_LatchEnable(LPC_PWM1, 5, PWM_OUT_ENABLED);
		DutyCycle01 = DutyCycle;
	}
	//Conmutaciones MOSfet
	//-----------------------------------------------------------------------------------------------
	switch (StepID1)
	{
	case 0:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1a[2], PIN_Q1a[2], 1);	//Apago Q4
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1a[0], PIN_Q1a[0], 0);	//Prendo Q0
		break;
	case 1:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1b[1], PIN_Q1b[1], 0);	//Apago Q3 inv
		Cycle = 2;													//Prendo Q5
		break;
	case 2:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1a[0], PIN_Q1a[0], 1);	//Apago Q0
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1a[1], PIN_Q1a[1], 0);	//Prendo Q2
		break;
	case 3:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1b[2], PIN_Q1b[2], 0);	//Apago Q5
		Cycle = 0;													//Prendo Q1
		break;
	case 4:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1a[1], PIN_Q1a[1], 1);	//Apago Q2
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1a[2], PIN_Q1a[2], 0);	//Prendo Q4
		break;
	default:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1b[0], PIN_Q1b[0], 0);	//Apago Q1
		Cycle = 1;													//Prendo Q3
	}

	if (StepID1 > 4)	//Si StepID es mayor a 4 reseteo variable StepID
	{
		StepID1 = 0;
		Count++;
	} else
		StepID1++;//Incremento StepID para la siguiente conmutación (6 conmutaciones)

	Match_Cnt1 = 0;	//Reinicio Match_Cnt

	//Estado anterior cruces zeros
	//-----------------------------------------------------------------------------------------------
	//CruceZero01[0] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z1[0], PIN_Z1[0]);
	//CruceZero01[1] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z1[1], PIN_Z1[1]);
	//CruceZero01[2] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z1[2], PIN_Z1[2]);
	//CruceZero01[0] = LPC_GPIO2->PIN & 0x000000E0;
}

void NextPWM2()
{
	//Actualizar DutyCycle
	//-----------------------------------------------------------------------------------------------
	if (DutyCycle != DutyCycle02)
	{
		Chip_PWM_SetMatch(LPC_PWM1, 6, DutyCycle);
		//Chip_PWM_Reset(LPC_PWM1);
		Chip_PWM_LatchEnable(LPC_PWM1, 6, PWM_OUT_ENABLED);
		DutyCycle02 = DutyCycle;
	}
	//Conmutaciones MOSfet
	//-----------------------------------------------------------------------------------------------
	switch (StepID2)
	{
	case 0:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2a[2], PIN_Q2a[2], 1);	//Apago Q4
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2a[0], PIN_Q2a[0], 0);	//Prendo Q0
		break;
	case 1:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2b[1], PIN_Q2b[1], 0);	//Apago Q3 inv
		Cycle = 2;													//Prendo Q5
		break;
	case 2:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2a[0], PIN_Q2a[0], 1);	//Apago Q0
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2a[1], PIN_Q2a[1], 0);	//Prendo Q2
		break;
	case 3:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2b[2], PIN_Q2b[2], 0);	//Apago Q5
		Cycle = 0;													//Prendo Q1
		break;
	case 4:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2a[1], PIN_Q2a[1], 1);	//Apago Q2
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2a[2], PIN_Q2a[2], 0);	//Prendo Q4
		break;
	default:
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2b[0], PIN_Q2b[0], 0);	//Apago Q1
		Cycle = 1;													//Prendo Q3
	}

	if (StepID2 > 4)	//Si StepID es mayor a 4 reseteo variable StepID
	{
		StepID2 = 0;
		//Count++;
	} else
		StepID2++;//Incremento StepID para la siguiente conmutación (6 conmutaciones)

	Match_Cnt2 = 0;	//Reinicio Match_Cnt

	//Estado anterior cruces zeros
	//-----------------------------------------------------------------------------------------------
	//CruceZero02[0] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z2[0], PIN_Z2[0]);
	//CruceZero02[1] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z2[1], PIN_Z2[1]);
	//CruceZero02[2] = Chip_GPIO_ReadPortBit(LPC_GPIO, PORT_Z2[2], PIN_Z2[2]);
	//CruceZero02[0] = LPC_GPIO->PIN & 0x0000C000;
	//CruceZero02[1] = LPC_GPIO2->PIN & 0x00000100;
}

//INTERRUPT PWM
void PWM1_IRQHandler(void)
{
	//Interupción Canal 0 -> PERIODO
	//-----------------------------------------------------------------------------------------------
	if (Chip_PWM_MatchPending(LPC_PWM1, 0))	//Reviso interrupción pendiente canal PWM 0
	{
		Match_Cnt1++;						//Incremento contador para el brillo
		Match_Cnt2++;
		Chip_PWM_ClearMatch(LPC_PWM1, 0);	//Limpio interrupción canal PWM 0

		//PWM sobre transistores PMOS
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1b[Cycle], PIN_Q1b[Cycle], 1);	//Encender (invertido por el tbj emisor común)
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2b[Cycle], PIN_Q2b[Cycle], 1);	//Encender (invertido por el tbj emisor común)
	}

	//Interrupción Canal 5 -> DUTYCICLE
	//-----------------------------------------------------------------------------------------------
	if (Chip_PWM_MatchPending(LPC_PWM1, 5)) //Reviso interrupción pendiente canal PWM 5
	{
		Chip_PWM_ClearMatch(LPC_PWM1, 5);	//Limpio interrupción canal PWM 5

		//PWM sobre transistores NMOS
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q1b[Cycle], PIN_Q1b[Cycle], 0);	//Apagar (invertido por el tbj emisor común)
	}
	//Interrupción Canal 5 -> DUTYCICLE
	//-----------------------------------------------------------------------------------------------
	if (Chip_PWM_MatchPending(LPC_PWM1, 6)) //Reviso interrupción pendiente canal PWM 5
	{
		Chip_PWM_ClearMatch(LPC_PWM1, 6);	//Limpio interrupción canal PWM 5

		//PWM sobre transistores NMOS
		Chip_GPIO_WritePortBit(LPC_GPIO, PORT_Q2b[Cycle], PIN_Q2b[Cycle], 0);	//Apagar (invertido por el tbj emisor común)
	}
}
void EINT3_IRQHandler(void)
{

	 if(P2_6REI || P2_6FEI || P2_7REI || P2_7FEI || P2_8REI || P2_8FEI)
	 {
		 P2_6CI=1;
		 P2_7CI=1;
		 P2_8CI=1;
		 Conmutar1=1;
	 }

	 if(P0_15REI || P0_15FEI || P0_16REI || P0_16FEI || P2_9REI || P2_9FEI)
	 {
		 P0_15CI=1;
		 P0_16CI=1;
		 P2_9CI=1;
		 Conmutar2=1;
	 }
}
