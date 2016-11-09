#define __NOP __nop

#include "msp.h"
#include "main.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#include "Button.hpp"
//#include "Accelerometer.hpp"
#include "Screen.hpp"
#include <driverlib.h>
#include <stdlib.h>     /* abs */
#include "task_ids.hpp"



uint8_t Task::m_u8NextTaskID = 0;
volatile static uint64_t SystemTicks = 0;



//Accelerometer accelerometer(ACCELEROMETER_ID,NOT_PERIODIC_TASK,ACCELEROMETER_PORT,ACCELEROMETER_PIN,200);

Scheduler MainScheduler;


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

		int* DataToSend = new int();
		*DataToSend = BUTTON_ID;

		MSG callButton = {PORT3_ISR_ID,SCHEDULER_ID,DataToSend};
		MainScheduler.attachMessage(callButton);

	}

}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// **********************************
// Interrupt service routine for
// ADC
// **********************************
//extern "C"
//{
///* This interrupt is fired whenever a conversion is completed and placed in
// * ADC_MEM0 */
///* This interrupt is fired whenever a conversion is completed and placed in
// * ADC_MEM0 */
//void ADC14_IRQHandler(void)
//{
//
//    uint64_t l_u64Status;
//    l_u64Status = MAP_ADC14_getEnabledInterruptStatus();
//    MAP_ADC14_clearInterruptFlag(l_u64Status);
//    int16_t l_fSample = 0;
//
//    if (l_u64Status & ADC_INT0) {
//
//    	l_fSample = MAP_ADC14_getResult(ADC_MEM0);
//    	l_fSample = abs(l_fSample);
//  		GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
//
//    }
//}
//}

void main(void)
{

    GPIO_registerInterrupt(BUTTON_PORT, BUTTON_ISR);
    LED BlinkLED(LED1_ID,PERIODIC_TASK,RGB_BLUE_PORT,RGB_BLUE_PIN,1000);
    LED BlinkLED2(LED2_ID, PERIODIC_TASK, RGB_GREEN_PORT,RGB_GREEN_PIN,2000);
    Button button(BUTTON_ID,NOT_PERIODIC_TASK,BUTTON_PORT,BUTTON_PIN,200);
    Screen PrintScreen(RGB_GREEN_PORT,RGB_GREEN_PIN,2000);

    Setup();

    MainScheduler.attach(&BlinkLED);
    MainScheduler.attach(&BlinkLED2);
    MainScheduler.attach(&button);
    //MainScheduler.attach(&accelerometer);

    while(1){
    	__wfe();

    	if(SystemTicks != MainScheduler.ticks) {
            MainScheduler.ticks = SystemTicks;
            MainScheduler.CalculateNextSchedule();
            MainScheduler.run();
            MainScheduler.clearNextScheduler();
            MainScheduler.ProcessMessageQueue();
        }
    };
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


    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);


	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P1.0 is connected to the Red LED
	P1->DIR |= BIT0;

	GPIO_setAsOutputPin(RGB_RED_PORT,RGB_RED_PIN);
	GPIO_setOutputLowOnPin(RGB_RED_PORT,RGB_RED_PIN);

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
	TIMER32_1->LOAD = 0x0000BB80; //~1ms ---> a 48Mhz
	TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
	NVIC_SetPriority(T32_INT1_IRQn,1);
	NVIC_EnableIRQ(T32_INT1_IRQn);
	__enable_irq();

	return;
}




extern "C" {


	void T32_INT1_IRQHandler(void)
	{
		TIMER32_1->INTCLR = 0U;
		P1->OUT ^= BIT0;
		SystemTicks++;
		return;
	}
}
