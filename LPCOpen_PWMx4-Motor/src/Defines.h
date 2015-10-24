/*
 * Variables globales y puertos para controlar motores.
 *
 * PORT_Qa y PIN_Qa -> PWM
 * PORT_Qb y PIN_Qb -> Encendido digital
 */

//DEFINICIONES PUERTOS SALIDAS ENTRADAS
//-----------------------------------------------------------------------------------------------
#define PORT_Q11		1
#define PORT_Q12		1
#define PORT_Q13		1
#define PORT_Q14		1
#define PORT_Q15		1
#define PORT_Q10		1
#define PIN_Q10		24
#define PIN_Q11		26
#define PIN_Q12		21
#define PIN_Q13		23
#define PIN_Q14		18
#define PIN_Q15		20

#define PORT_Q21		1
#define PORT_Q22		1
#define PORT_Q23		1
#define PORT_Q24		1
#define PORT_Q25		1
#define PORT_Q20		1
#define PIN_Q20		24
#define PIN_Q21		26
#define PIN_Q22		21
#define PIN_Q23		23
#define PIN_Q24		18
#define PIN_Q25		20

#define PORT_Q31		1
#define PORT_Q32		1
#define PORT_Q33		1
#define PORT_Q34		1
#define PORT_Q35		1
#define PORT_Q30		1
#define PIN_Q30		24
#define PIN_Q31		26
#define PIN_Q32		21
#define PIN_Q33		23
#define PIN_Q34		18
#define PIN_Q35		20

#define PORT_Q41		1
#define PORT_Q42		1
#define PORT_Q43		1
#define PORT_Q44		1
#define PORT_Q45		1
#define PORT_Q40		1
#define PIN_Q40		24
#define PIN_Q41		26
#define PIN_Q42		21
#define PIN_Q43		23
#define PIN_Q44		18
#define PIN_Q45		20

#define PORT_Z11		1
#define PORT_Z12		1
#define PORT_Z13		1
#define PIN_Z11		19
#define PIN_Z12		22
#define PIN_Z13		25

#define PORT_Z21		1
#define PORT_Z22		1
#define PORT_Z23		1
#define PIN_Z21		19
#define PIN_Z22		22
#define PIN_Z23		25

#define PORT_Z31		1
#define PORT_Z32		1
#define PORT_Z33		1
#define PIN_Z31		19
#define PIN_Z32		22
#define PIN_Z33		25

#define PORT_Z41		1
#define PORT_Z42		1
#define PORT_Z43		1
#define PIN_Z41		19
#define PIN_Z42		22
#define PIN_Z43		25

#define PULS1 		18
#define PULS2 		1
#define PULS_PORT 	0

#define REBOTE_ 	100000
#define SALIDA		1

//GLOBALES
//-----------------------------------------------------------------------------------------------
uint32_t Match_Cnt=0, Cycle=0, AntiRebo=REBOTE_,
PORT_Q1a[3]={PORT_Q11,PORT_Q13,PORT_Q15},
PIN_Q1a[6]={PIN_Q11,PIN_Q13,PIN_Q15},
PORT_Q1b[6]={PORT_Q10,PORT_Q12,PORT_Q14},
PIN_Q1b[6]={PIN_Q10,PIN_Q12,PIN_Q14},
PORT_Z1[3]={PORT_Z11, PORT_Z12, PORT_Z13},
PIN_Z1[3]={PIN_Z11, PIN_Z12, PIN_Z13},

PORT_Q2a[3]={PORT_Q21,PORT_Q23,PORT_Q25},
PIN_Q2a[6]={PIN_Q21,PIN_Q23,PIN_Q25},
PORT_Q2b[6]={PORT_Q20,PORT_Q22,PORT_Q24},
PIN_Q2b[6]={PIN_Q20,PIN_Q22,PIN_Q24},
PORT_Z2[3]={PORT_Z21, PORT_Z22, PORT_Z23},
PIN_Z2[3]={PIN_Z21, PIN_Z22, PIN_Z23},

PORT_Q3a[3]={PORT_Q31,PORT_Q33,PORT_Q35},
PIN_Q3a[6]={PIN_Q31,PIN_Q33,PIN_Q35},
PORT_Q3b[6]={PORT_Q30,PORT_Q32,PORT_Q34},
PIN_Q3b[6]={PIN_Q30,PIN_Q32,PIN_Q34},
PORT_Z3[3]={PORT_Z31, PORT_Z32, PORT_Z33},
PIN_Z3[3]={PIN_Z31, PIN_Z32, PIN_Z33},

PORT_Q4a[3]={PORT_Q41,PORT_Q43,PORT_Q45},
PIN_Q4a[6]={PIN_Q41,PIN_Q43,PIN_Q45},
PORT_Q4b[6]={PORT_Q40,PORT_Q42,PORT_Q44},
PIN_Q4b[6]={PIN_Q40,PIN_Q42,PIN_Q44},
PORT_Z4[3]={PORT_Z41, PORT_Z42, PORT_Z43},
PIN_Z4[3]={PIN_Z41, PIN_Z42, PIN_Z43};

volatile uint8_t CruceZero[3]={0,0,0}, CruceZero0[3]={0,0,0};

struct StartParams_s {		// params for startup seq.
	long duration; 			// duration of accel phase of startup, us
	long periodRange[2]; 	// commutation period range for startup, us
	float powerRange[2];  	// frac of full power over startup accel
} start= { 200,   {460, 120},   {10, 300} };	//Cantidad de pasos, período inicial y final, pwm inicial y final para startup
												//-----> 50*20microseg = 1mSeg
long StepPeriod;     			// step duration, us
uint16_t DutyCycle, DutyCycle0; 	// fraction of period hi pins are high

volatile int StepID=0;  		// commutation step counter, 0..5
long Count=0;  					// no full commutation cycles completed

//FUNCIONES
//-----------------------------------------------------------------------------------------------
void PWM1_IRQHandler(void);
void InitPWM(void);
void InitGPIO(void);
void Stop_and_Default(void);
void Start_Up_Brushless(void);
void NextPWM(void);
void Zero_Detect(void);
