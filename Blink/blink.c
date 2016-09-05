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

// ***********************************
// Main loop
// ***********************************
volatile uint16_t	g_TimerCount =0;
void main(void)
{
    Setup();
   while(1)
   {
	   // - Wait for events
	   __wfe();
	   //MAP_GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
/*
	   if(MAP_GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN5) == 0) {
		   P1->OUT = BIT0;
	   }

	   else {
		   P1->OUT = 0U;
	   }
*/
   }
}

// **********************************
// Interrupt service routine for
// Timer A0
// **********************************
void TA0_0_ISR(void)
{
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
	return;
}

void PORT3_PIN5_ISR(void) {

	if( GPIO_getInterruptStatus(main_BUTTON_PORT, main_BUTTON_PIN) ) {
		GPIO_clearInterruptFlag(main_BUTTON_PORT, main_BUTTON_PIN);
		GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
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
	//P1->DIR |= BIT0;
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
	            GPIO_PORT_P3,
	            GPIO_PIN5,
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

/*
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__2 | TIMER_A_CTL_IE;
	NVIC_SetPriority(TA0_N_IRQn,1);
	NVIC_EnableIRQ(TA0_N_IRQn);
	TIMER_A0->CCR[0] = 0xFFFF;
	TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
	*/
	__enable_irq();
	return;
}
