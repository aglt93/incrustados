#define __NOP __nop
#include "msp.h"
#include "main.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#include <driverlib.h>


uint8_t Task::m_u8NextTaskID = 0;
volatile static uint64_t SystemTicks = 0;

void main(void)
{
    Scheduler MainScheduler;
    LED BlinkLED(RGB_BLUE_PORT,RGB_BLUE_PIN,5);
    LED BlinkLED2(RGB_GREEN_PORT,RGB_GREEN_PIN,3);
    Setup();
	GPIO_clearInterruptFlag(BUTTON_PORT, BUTTON_PIN);
	GPIO_enableInterrupt(BUTTON_PORT, BUTTON_PIN);
    MainScheduler.attach(&BlinkLED);
    MainScheduler.attach(&BlinkLED2);
    while(1){
    	__wfe();
        if(SystemTicks != MainScheduler.ticks)
        {
            MainScheduler.ticks = SystemTicks;
            MainScheduler.CalculateNextSchedule();
            MainScheduler.run();
        }
    };
}

//////////////////////////////////////////////////////////////////////////////////////////////
// **********************************
// Interrupt service routine for
// PORT3
// **********************************
extern "C"
{
void BUTTON_ISR(void) {

	// ISR for PIN5
	if(GPIO_getInterruptStatus(BUTTON_PORT, BUTTON_PIN)) {
		// Clear interrupt flag and toggle output LEDs.
		GPIO_clearInterruptFlag(BUTTON_PORT, BUTTON_PIN);
		GPIO_toggleOutputOnPin(LED_RED_PORT, LED_RED_PIN);

	}

}
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
	// - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;


	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P1.0 is connected to the Red LED
	P1->DIR |= BIT0;



	GPIO_setAsOutputPin(LED_RED_PORT, LED_RED_PIN);
	GPIO_setOutputLowOnPin(LED_RED_PORT, LED_RED_PIN);
	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Disable all interrupts
	// - Configure Timer32_1  with MCLK (3Mhz), Division by 1, Enable the interrupt, Periodic Mode
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	// - Re-enable interrupts
	__disable_irq();
	//TIMER32_1->LOAD = 0x002DC6C0; //~1s ---> a 3Mhz
	TIMER32_1->LOAD = 0x00000BB8; //~1ms ---> a 3Mhz
	TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
	NVIC_SetPriority(T32_INT1_IRQn,1);
	NVIC_EnableIRQ(T32_INT1_IRQn);

	GPIO_setAsInputPinWithPullUpResistor(BUTTON_PORT, BUTTON_PIN);

	/* Configuring and enabling P2.6 interrupt triggered by a HIGH TO LOW transition */

	GPIO_interruptEdgeSelect(BUTTON_PORT, BUTTON_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_registerInterrupt(BUTTON_PORT, BUTTON_ISR);

	__enable_irq();


	return;
}




extern "C"
{
	void T32_INT1_IRQHandler(void)
	{
		TIMER32_1->INTCLR = 0U;
		P1->OUT ^= BIT0;
		SystemTicks++;
		return;
	}
}
