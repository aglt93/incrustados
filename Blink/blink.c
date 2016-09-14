/******************************************************************************
*
* MSP432 blink.c - P1.0 port toggle
*
* CMSIS compliant coding
*
******************************************************************************/

// 1 SEC MAIN KVALLECI 20:08 13/9/16
//////////////////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include "msp.h"
#include "blink.h"
#include <driverlib.h>
#include <HAL_I2C.h>
#include <HAL_OPT3001.h>
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
// Global Variables Definition.
bool g_checkLightSensorBool;
bool g_bBlink3Done;
bool g_bSecondPassed;
bool g_bModeSelector;	// TRUE -> SMART_MODE;	FALSE -> MANUAL_MODE
bool g_bLampState;		// TRUE -> LAMP_ON;		FALSE -> LAMP_OFF
bool g_bToggleLamp;
//
int g_iSecCount;
//////////////////////////////////////////////////////////////////////////////////////////////
/* Statics */
static volatile float g_fSample1SecBuffer[MAX_SAMPLE];
static volatile float g_fSample200msBuffer[MAX_SAMPLE];
static volatile int16_t g_f200msSample;
static volatile int16_t g_u8Count200ms;
static volatile int16_t g_u8Count200msI;

static volatile uint16_t g_u8Count1Sec;
static volatile uint16_t g_u8Count1SecI;
static volatile bool g_b200msPassed;
static volatile bool g_bSecSamplerInitDone;
static volatile bool g_bSecSamplerMainDone;
static volatile bool g_bSample5SecDone;
static volatile bool g_b1SecPassed;
static volatile bool g_bOutlierDetected;
static volatile int16_t g_fSampleAverage;
static volatile int16_t g_fTotalAverage;
static volatile int16_t g_fSumAverage;
static volatile int16_t g_fTotalAverage;
static volatile int16_t g_f200msSumSample;


//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
/* Timer_A Continuous Mode Configuration Parameter */
const Timer_A_UpModeConfig upModeConfig =
{
        TIMER_A_CLOCKSOURCE_ACLK,            // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,       // ACLK/1 = 32Khz
		6554,//32768,//6554,//16384,
        TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, // Disable CCR0
        TIMER_A_DO_CLEAR                     // Clear Counter
};

/* Timer_A Compare Configuration Parameter */
const Timer_A_CompareModeConfig compareConfig =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_SET_RESET,               // Toggle output but
		6554//32768//6554//16384                                       // 16000 Period
};
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
// ***********************************
// Main loop
// ***********************************
void main(void) {

	//////////////////////////////////////////////////////////////////////////////////////////////
	Setup();
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	while (!g_bBlink3Done) {

	   if(g_bSecondPassed) {
		   g_bSecondPassed = false;
		   Blink3();
	   }
	   if(g_b200msPassed){
		   g_b200msPassed = false;
		   msSampler();
		   //GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
		   if(g_b1SecPassed){
			   GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
			   if(!g_bSecSamplerInitDone){
				   g_bSecSamplerInitDone = secSamplerInit();
				//   GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
			   }
			   g_u8Count1Sec++;
		   }
	   }
	   __wfe();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////
	// Until the system is ready the user can turn on the night lamp manually.
	GPIO_clearInterruptFlag(BUTTON_PORT, BUTTON_PIN);
	GPIO_enableInterrupt(BUTTON_PORT, BUTTON_PIN);

	g_bSecondPassed = true;
	g_bModeSelector = SMART_MODE;
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	while(1) {

		if(g_b200msPassed){
		   g_b200msPassed = false;
		   msSampler();
//		   GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
		   if(g_b1SecPassed){
		   GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
			   if(!g_bSecSamplerMainDone){
				   g_bSecSamplerMainDone = secSamplerInit();
				   if(g_bSecSamplerMainDone){
					   g_bOutlierDetected = outlier10p();
				   }
				  // GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
			   }
			   g_u8Count1Sec++;
		   }
		}

	   if (g_bSecondPassed) {

		   g_bSecondPassed = false;

		   if (g_bModeSelector == SMART_MODE) {
			   SmartMode();
		   }

		   else {
			   ManualMode();
		   }
	   }

	   if (g_bToggleLamp) {

			g_bToggleLamp = false;
			__delay_cycles(100000);
			if (GPIO_getInputPinValue(BUTTON_PORT,BUTTON_PIN) == 0) {
				SetLamp(TOGGLE_LAMP);
				g_bModeSelector = !GetLampState();
				ResetT32_1();
				g_iSecCount = 0;
			}

	   }

	   __wfe();
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
void Blink3(void) {

	switch (g_iSecCount) {

		case 0:
			SetLamp(TURN_ON_LAMP);
			break;

		case 1:
			SetLamp(TURN_OFF_LAMP);
			break;

		case 2:
			SetLamp(TURN_ON_LAMP);
			break;

		case 3:
			SetLamp(TURN_OFF_LAMP);
			break;

		case 4:
			SetLamp(TURN_ON_LAMP);
			break;

		case 5:
			SetLamp(TURN_OFF_LAMP);
			break;

		case 6:
			g_bBlink3Done = true;
			break;

		default:
			SetLamp(TURN_OFF_LAMP);
			g_bBlink3Done = true;
			break;
	}

	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool msSampler(void){
	g_f200msSumSample+=g_f200msSample;
	g_u8Count200msI = g_u8Count200ms-1;
	g_fSample200msBuffer[g_u8Count200msI]=g_f200msSample;
	//printf("g_f200msSample = %i, count %i\n", g_f200msSample ,g_u8Count200ms );
	if (g_u8Count200ms==5){
		//printf("g_f200msSumSample = %i, count %u\n", g_f200msSumSample ,g_u8Count200ms );
		g_fSampleAverage = g_f200msSumSample/MAX_SAMPLE;
		//printf("g_fSampleAverage = %f, count %u\n", g_fSampleAverage ,g_u8Count200ms );
		g_u8Count200ms = 0;
		g_f200msSumSample = 0;
		g_b1SecPassed = true;
		return true;
	}
	g_b1SecPassed = false;
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
bool secSamplerInit(void){
	//g_u8Count1SecI=g_u8Count1Sec%5;
//	printf("g_fSampleAverage = %i, g_u8Count1Sec %i, g_u8Count1SecI %i\n", g_fSampleAverage ,g_u8Count1Sec, g_u8Count1SecI );
	g_fSample1SecBuffer[g_u8Count1Sec]=g_fSampleAverage;
	g_fSumAverage+=g_fSampleAverage;
	//printf("g_fSampleAverage = %i, g_u8Count1Sec %i\n", g_fSampleAverage ,g_u8Count1Sec );

	if (g_u8Count1Sec==5){
		//GPIO_toggleOutputOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
		g_fTotalAverage = g_fSumAverage/MAX_SAMPLE;
		g_bSample5SecDone = true;
		g_u8Count1Sec = 0;
		return true;
	}
	g_bSample5SecDone = false;
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
bool secSamplerMain(void){
	//g_u8Count1SecI=g_u8Count1Sec%5;
//	printf("g_fSampleAverage = %i, g_u8Count1Sec %i, g_u8Count1SecI %i\n", g_fSampleAverage ,g_u8Count1Sec, g_u8Count1SecI );
	g_fSample1SecBuffer[g_u8Count1Sec]=g_fSampleAverage;
	g_fSumAverage+=g_fSampleAverage;
	//printf("g_fSampleAverage = %i, g_u8Count1Sec %i\n", g_fSampleAverage ,g_u8Count1Sec );

	if (g_u8Count1Sec==5){
		//GPIO_toggleOutputOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
		g_fTotalAverage = g_fSumAverage/MAX_SAMPLE;
		g_bSample5SecDone = true;
		g_u8Count1Sec = 0;
		return true;
	}
	g_bSample5SecDone = false;
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////
bool OutlierDetected(void){
	   int i;

	   /* for loop execution */
	   for( i = 0; i < 5; i++ ){
		   if(g_fSample1SecBuffer[i]*1.1>g_fTotalAverage){
			   g_bOutlierDetected = true;
		   }
		   g_bOutlierDetected = false;
	   }
	   return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
float SenseLight (void) {

	return OPT3001_getLux();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void SmartMode (void) {

	float l_fLux;

	l_fLux = SenseLight();

	if (l_fLux < LUX_LIMIT && g_bOutlierDetected ) {
//	if (SenseSound()) {

		SetLamp(TURN_ON_LAMP);
		ResetT32_1();
		g_iSecCount = 0;

	}

	else if (g_iSecCount == SEC_COUNT_LIMIT) {

		SetLamp(TURN_OFF_LAMP);

	}

	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
void ManualMode (void) {

	if (g_iSecCount == SEC_COUNT_LIMIT) {

		SetLamp(TURN_OFF_LAMP);
		ResetT32_1();
		g_iSecCount = 0;
		g_bModeSelector = SMART_MODE;

	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//

//////////////////////////////////////////////////////////////////////////////////////////////
void SetLamp (int i_iState) {

	switch (i_iState) {

		case TURN_ON_LAMP:
			#if (defined(LAMP_POWER5) || defined(LAMP_POWER10) || defined(LAMP_POWER15))
			GPIO_setOutputHighOnPin(RGB_RED_PORT,RGB_RED_PIN);
			#endif
			#if (defined(LAMP_POWER10) || defined(LAMP_POWER15))
			GPIO_setOutputHighOnPin(RGB_GREEN_PORT,RGB_GREEN_PIN);
			#endif
			#ifdef LAMP_POWER15
			GPIO_setOutputHighOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
			#endif

			break;

		case TURN_OFF_LAMP:
			#if (defined(LAMP_POWER5) || defined(LAMP_POWER10) || defined(LAMP_POWER15))
			GPIO_setOutputLowOnPin(RGB_RED_PORT,RGB_RED_PIN);
			#endif
			#if (defined(LAMP_POWER10) || defined(LAMP_POWER15))
			GPIO_setOutputLowOnPin(RGB_GREEN_PORT,RGB_GREEN_PIN);
			#endif
			#ifdef LAMP_POWER15
			GPIO_setOutputLowOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
			#endif

			break;

		case TOGGLE_LAMP:
			#if (defined(LAMP_POWER5) || defined(LAMP_POWER10) || defined(LAMP_POWER15))
			GPIO_toggleOutputOnPin(RGB_RED_PORT,RGB_RED_PIN);
			#endif
			#if (defined(LAMP_POWER10) || defined(LAMP_POWER15))
			GPIO_toggleOutputOnPin(RGB_GREEN_PORT,RGB_GREEN_PIN);
			#endif
			#ifdef LAMP_POWER15
			GPIO_toggleOutputOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
			#endif

			break;

		default:
			#if (defined(LAMP_POWER5) || defined(LAMP_POWER10) || defined(LAMP_POWER15))
			GPIO_setOutputLowOnPin(RGB_RED_PORT,RGB_RED_PIN);
			#endif
			#if (defined(LAMP_POWER10) || defined(LAMP_POWER15))
			GPIO_setOutputLowOnPin(RGB_GREEN_PORT,RGB_GREEN_PIN);
			#endif
			#ifdef LAMP_POWER15
			GPIO_setOutputLowOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
			#endif

			break;

	}

}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
void ResetT32_1 (void) {

	MAP_Timer32_clearInterruptFlag(TIMER32_BASE);
	MAP_Timer32_setCount(TIMER32_BASE,T32_SEC_COUNT);
	MAP_Timer32_enableInterrupt(TIMER32_BASE);
	MAP_Timer32_startTimer(TIMER32_BASE, true);

}

//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
bool GetLampState (void) {

	bool l_bLampState = false;

	#if (defined(LAMP_POWER5) || defined(LAMP_POWER10) || defined(LAMP_POWER15))
	l_bLampState = GPIO_getInputPinValue(RGB_RED_PORT,RGB_RED_PIN);
	#endif
	#if (defined(LAMP_POWER10) || defined(LAMP_POWER15))
	l_bLampState = l_bLampState && GPIO_getInputPinValue(RGB_GREEN_PORT,RGB_GREEN_PIN);
	#endif
	#ifdef LAMP_POWER15
	l_bLampState = l_bLampState && GPIO_getInputPinValue(RGB_BLUE_PORT,RGB_BLUE_PIN);
	#endif

	return l_bLampState;
}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
// **********************************
// Interrupt service routine for
// PORT3
// **********************************
void BUTTON_ISR(void) {

	// ISR for PIN5
	if(GPIO_getInterruptStatus(BUTTON_PORT, BUTTON_PIN)) {
		// Clear interrupt flag and toggle output LEDs.
		GPIO_clearInterruptFlag(BUTTON_PORT, BUTTON_PIN);

		g_bToggleLamp = true;
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
// **********************************
// Interrupt service routine for
// TIMER32
// **********************************
void T32_ISR(void) {

	MAP_Timer32_clearInterruptFlag(TIMER32_BASE);
	//
	g_iSecCount++;
	g_bSecondPassed = true;
	//
	MAP_Timer32_setCount(TIMER32_BASE,T32_SEC_COUNT);
	MAP_Timer32_enableInterrupt(TIMER32_BASE);
	MAP_Timer32_startTimer(TIMER32_BASE, true);
	//
//	GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);

}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM0 */
void ADC14_IRQHandler(void)
{
    uint64_t l_u64Status;
    l_u64Status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(l_u64Status);
    if (l_u64Status & ADC_INT0)
    {
    	g_f200msSample= MAP_ADC14_getResult(ADC_MEM0);
    	g_f200msSample = abs(g_f200msSample);
    	g_u8Count200ms++;
//    	g_u8Count1Sec++;
    	g_b200msPassed = true;
    	//GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// **********************************
// Setup function for the application
// @input - none
// @output - none
// **********************************
void Setup(void)
{
	// ****************************
	//         DEVICE CONFIG
	// ****************************
	/* Halting the Watchdog */
	MAP_WDT_A_holdTimer();

	//////////////////////////////////////////////////////////////////////////////////////////////
	// ****************************
	//     INIT GLOBAL VARIABLES
	// ****************************
	g_checkLightSensorBool = false;
	g_bBlink3Done = false;
	g_bSecondPassed = true;
	g_bModeSelector = SMART_MODE;
	g_bToggleLamp = false;
	g_iSecCount = 0;
	int i = 0;
	for (i = 0; i < MAX_SAMPLE-1; i++){
		g_fSample1SecBuffer[i] = 0.0;
	}
	g_f200msSample = 0.0;
	g_fSampleAverage = 0.0;
	g_fSumAverage = 0;
	g_fTotalAverage = 0;
	g_u8Count200ms = 0;
	g_u8Count200msI = 0;
	g_u8Count1Sec = 0;
	g_u8Count1SecI = 0;
	g_b200msPassed = true;
	g_b1SecPassed = false;
	g_bSecSamplerInitDone = false;
	g_bSecSamplerMainDone = false;
	g_bSample5SecDone = false;
	g_f200msSumSample = 0.0;
	g_bOutlierDetected = false;

	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	// ****************************
	//         PORT CONFIG
	// ****************************
	// Set RGB LEDs as outputs
	GPIO_setAsOutputPin(RGB_RED_PORT,RGB_RED_PIN);
	GPIO_setAsOutputPin(RGB_GREEN_PORT,RGB_GREEN_PIN);
	GPIO_setAsOutputPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
	GPIO_setAsOutputPin(LED_RED_PORT,LED_RED_PIN);
	// Initial value of LEDs on low.
	GPIO_setOutputLowOnPin(RGB_RED_PORT,RGB_RED_PIN);
	GPIO_setOutputLowOnPin(RGB_GREEN_PORT,RGB_GREEN_PIN);
	GPIO_setOutputLowOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
	GPIO_setOutputLowOnPin(LED_RED_PORT,LED_RED_PIN);
	//
	GPIO_setAsInputPinWithPullUpResistor(BUTTON_PORT, BUTTON_PIN);
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
    /* Initialize I2C communication */
    Init_I2C_GPIO();
    I2C_init();

    /* Initialize OPT3001 digital ambient light sensor */
    OPT3001_init();

    // Delay in order to make stable the sensor.
    __delay_cycles(100000);

    MAP_Timer32_initModule(TIMER32_BASE, TIMER32_PRESCALER_256, TIMER32_32BIT,TIMER32_PERIODIC_MODE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Timer32_setCount(TIMER32_BASE,T32_SEC_COUNT);
	MAP_Timer32_enableInterrupt(TIMER32_BASE);

	//////////////////////////////////////////////////////////////////////////////////////////////
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    /* Initializing ADC (MCLK/1/1) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,0);
    /* Configuring GPIOs (5.5 A0) */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(MIC_PORT, MIC_PIN,GPIO_TERTIARY_MODULE_FUNCTION);
    /* Configuring ADC Memory */
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A10, false);

    ADC14_setResultFormat(ADC_SIGNED_BINARY);
    /* Configuring Timer_A in continuous mode and sourced from ACLK */
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);
    /* Configuring Timer_A0 in CCR1 to trigger at 16000 (0.5s) */
    MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);
    /* Configuring the sample trigger to be sourced from Timer_A0  and setting it
    * to automatic iteration after it is triggered*/
    MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);
    /* Enabling the interrupt when a conversion on channel 1 is complete and
    * enabling conversions */
    MAP_ADC14_enableInterrupt(ADC_INT0);
    MAP_ADC14_enableConversion();
    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();
    /* Starting the Timer */
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	// ****************************
	//      INTERRUPT CONFIG
	// ****************************
	// Disable IRQ interruptions before configuring interruptions.
	__disable_irq();
	/* Configuring and enabling P2.6 interrupt triggered by a HIGH TO LOW transition */
	GPIO_interruptEdgeSelect(BUTTON_PORT, BUTTON_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_registerInterrupt(BUTTON_PORT, BUTTON_ISR);
	__enable_irq();
	//////////////////////////////////////////////////////////////////////////////////////////////


	MAP_Timer32_startTimer(TIMER32_BASE, true);


	return;
}
