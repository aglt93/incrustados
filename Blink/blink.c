/******************************************************************************
*
* MSP432 blink.c - P1.0 port toggle
*
* CMSIS compliant coding
*
******************************************************************************/

#include "msp.h"
#include "blink.h"
#include <driverlib.h>
#include <HAL_I2C.h>
#include <HAL_OPT3001.h>
#include <stdbool.h>

// Float to store the lux value from the sensor.
//float g_lux;
bool g_checkLightSensorBool = false;

// ***********************************
// Main loop
// ***********************************
void main(void)
{
   Setup();
   while(1) {
	   // - Wait for events
	   __wfe();

	   if(g_checkLightSensorBool){
		   checkLightSensor();
	   }



   }
}

void checkLightSensor(void) {

	float g_lux = 55;//OPT3001_getLux();

	if (g_lux < 50) {
	   GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN6);
	   GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN4);
	   GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN6);
	}

	else {
	   GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN6);
	   GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
	   GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN6);
	}

}


// **********************************
// Interrupt service routine for
// PORT3
// **********************************
void PORT3_ISR(void) {

	// ISR for PIN5
	if(GPIO_getInterruptStatus(main_BUTTON_PORT, main_BUTTON_PIN)) {
		// Clear interrupt flag and toggle output LEDs.
		GPIO_clearInterruptFlag(main_BUTTON_PORT, main_BUTTON_PIN);
		GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
		GPIO_toggleOutputOnPin(GPIO_PORT_P2,GPIO_PIN6);

	}

}

void T32_ISR(void) {

	MAP_Timer32_clearInterruptFlag(TIMER32_BASE);
	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
	//
	//MAP_Timer32_setCount(TIMER32_BASE,11720);
	//MAP_Timer32_enableInterrupt(TIMER32_BASE);
	//MAP_Timer32_startTimer(TIMER32_BASE, true);

	g_checkLightSensorBool = true;

}


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

	// ****************************
	//         PORT CONFIG
	// ****************************
	// Set RGB LEDs as outputs
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN6);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN4);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P5,GPIO_PIN6);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
	// Initial value of LEDs on low.
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN6);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN6);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
	//
	GPIO_setAsInputPinWithPullUpResistor(main_BUTTON_PORT, main_BUTTON_PIN);
	/*
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
	            main_BUTTON_PORT,
	            main_BUTTON_PIN,
	            GPIO_PRIMARY_MODULE_FUNCTION);
	*/
	// ****************************
	//      INTERRUPT CONFIG
	// ****************************
	// Disable IRQ interruptions before configuring interruptions.

	//__disable_irq();

	/* Configuring and enabling P2.6 interrupt triggered by a HIGH TO LOW transition */
/*
	GPIO_interruptEdgeSelect(main_BUTTON_PORT, main_BUTTON_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_registerInterrupt(main_BUTTON_PORT, PORT3_ISR);
	GPIO_enableInterrupt(main_BUTTON_PORT, main_BUTTON_PIN);

	__enable_irq();
*/
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
    MAP_Timer32_setCount(TIMER32_BASE,11720);
	MAP_Timer32_enableInterrupt(TIMER32_BASE);
	MAP_Timer32_startTimer(TIMER32_BASE, true);

	return;
}
