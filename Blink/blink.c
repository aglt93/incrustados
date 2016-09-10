/******************************************************************************
*
* MSP432 blink.c - P1.0 port toggle
*
* CMSIS compliant coding
*
******************************************************************************/


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
//
int g_iSecCount;
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

	   if (g_bSecondPassed) {

		   g_bSecondPassed = false;

		   if (g_bModeSelector == SMART_MODE) {
			   SmartMode();
		   }

		   else {
			   ManualMode();
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


//////////////////////////////////////////////////////////////////////////////////////////////
float SenseLight (void) {

	return OPT3001_getLux();
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
void SmartMode (void) {

	float l_fLux;

	l_fLux = SenseLight();

	if (l_fLux < LUX_LIMIT) {

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
		g_bModeSelector = SMART_MODE;
		g_iSecCount = 0;

	}
}

//////////////////////////////////////////////////////////////////////////////////////////////


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

		SetLamp(TOGGLE_LAMP);
		g_bModeSelector = !GetLampState();
		g_iSecCount = 0;
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
	GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);

}
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
	g_iSecCount = 0;
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

    MAP_Timer32_initModule(TIMER32_BASE, TIMER32_PRESCALER_256, TIMER32_32BIT,
    	            TIMER32_PERIODIC_MODE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
    MAP_Timer32_setCount(TIMER32_BASE,T32_SEC_COUNT);
	MAP_Timer32_enableInterrupt(TIMER32_BASE);
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
