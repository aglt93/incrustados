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

float lux;

// ***********************************
// Main loop
// ***********************************
volatile uint16_t	g_TimerCount =0;
void main(void)
{
    Setup();
   while(1)
   {
	   lux = OPT3001_getLux();

	   if (lux > 50) {
		   MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN6);
		   GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN4);
		   GPIO_setOutputHighOnPin(GPIO_PORT_P5,GPIO_PIN6);
	   }

	   else {
		   MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN6);
		   GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN4);
		   GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN6);
	   }
	   // - Wait for events
	   //__wfe();

   }
}

// **********************************
// Interrupt service routine for
// Timer A0
// **********************************
void TA0_0_ISR(void)
{
	/*
	TIMER_A0->CTL ^= TIMER_A_CTL_IFG;
	// - Divide the clock further, to achieve human readable times.
	if(g_TimerCount == TIMERA0_COUNT)
	{
		// - Toggle P1.0
		MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
		g_TimerCount = 0U;

	}
	else
	{
		g_TimerCount++;
	}
	*/
	return;
}

void PORT3_PIN5_ISR(void) {

	if( GPIO_getInterruptStatus(main_BUTTON_PORT, main_BUTTON_PIN) ) {
		GPIO_clearInterruptFlag(main_BUTTON_PORT, main_BUTTON_PIN);
		GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
		GPIO_toggleOutputOnPin(GPIO_PORT_P2,GPIO_PIN6);
	}

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
	// - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;


	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P1.0 is connected to the Red LED
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN6);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN4);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P5,GPIO_PIN6);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
	            main_BUTTON_PORT,
	            main_BUTTON_PIN,
	            GPIO_PRIMARY_MODULE_FUNCTION);

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Disable all interrupts
	// - Configure Timer A0 with SMCLK, Division by 8, Enable the interrupt
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	// - Re-enable interrupts
	__disable_irq();

	/* Confinguring P1.1 & P1.4 as an input and enabling interrupts */
	GPIO_setAsInputPinWithPullUpResistor(main_BUTTON_PORT, main_BUTTON_PIN);
	GPIO_interruptEdgeSelect(main_BUTTON_PORT, main_BUTTON_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_registerInterrupt(main_BUTTON_PORT, PORT3_PIN5_ISR);
	GPIO_enableInterrupt(main_BUTTON_PORT, main_BUTTON_PIN);

	__enable_irq();

    /* Initialize I2C communication */
    Init_I2C_GPIO();
    I2C_init();

    /* Initialize OPT3001 digital ambient light sensor */
    OPT3001_init();

    __delay_cycles(100000);


	return;
}
