#define __NOP __nop

#include "msp.h"
#include "main.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#include "Button.hpp"
#include "Screen.hpp"
#include "Servo.hpp"
#include <driverlib.h>
#include <stdlib.h>     /* abs */
#include "task_ids.hpp"

#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"

Graphics_Context g_sContext;


uint8_t Task::m_u8NextTaskID = 0;
volatile static uint64_t SystemTicks = 0;
volatile static int x = 40; // tomala alejandro playo...
uint64_t g_u64Status;
int* DataToSend = new int();
int* DataToSendADC = new int();
int* DataToSend2ADC  = new int();

//typedef Ant* AntPtr;
int aDataToSendADC[2];
int *pDataToSendADC;

int* DataToSend3 = new int();
Scheduler MainScheduler;
Servo Servo1(SERVO_ID,NOT_PERIODIC_TASK,SERVO_PORT,SERVO_PIN);
int counterADC;

extern "C"
{
void BUTTON_ISR(void) {

	// ISR for PIN5
	if(GPIO_getInterruptStatus(BUTTON_PORT, BUTTON_PIN)) {
		// Clear interrupt flag and toggle output LEDs.
		GPIO_clearInterruptFlag(BUTTON_PORT, BUTTON_PIN);
        GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
        //Servo1.run();

//		*DataToSend3 = 300;
//		MSG changeServo = {PORT3_ISR_ID,SERVO_ID,DataToSend3};
//		MainScheduler.attachMessage(changeServo);

	}

}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
{
/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM2. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{

    g_u64Status = MAP_ADC14_getEnabledInterruptStatus();

    MAP_ADC14_clearInterruptFlag(g_u64Status);
    //GPIO_toggleOutputOnPin(LED_GREEN_PORT,LED_GREEN_PIN);
    counterADC++;
    /* ADC_MEM2 conversion completed */
    if(g_u64Status & ADC_INT2 && counterADC == 200)
    {
        GPIO_toggleOutputOnPin(LED_GREEN_PORT,LED_GREEN_PIN);
        counterADC = 0;
        /* Store ADC14 conversion results */
    	aDataToSendADC[0] = ADC14_getResult(ADC_MEM1);
    	aDataToSendADC[1] = ADC14_getResult(ADC_MEM2);

        pDataToSendADC = aDataToSendADC;

        MSG changeScreen = {ADC_ISR_ID,SCREEN_ID,pDataToSendADC};
        MainScheduler.attachMessage(changeScreen);

         *DataToSend3 = aDataToSendADC[1];
		MSG changeServo = {PORT3_ISR_ID,SERVO_ID,DataToSend3};
		MainScheduler.attachMessage(changeServo);


//        *DataToSend3 = 900;
//		MSG changeServo = {PORT3_ISR_ID,SERVO_ID,DataToSend3};
//		MainScheduler.attachMessage(changeServo);

        //Servo1.ProcessMessage(changeServo);
    }
}
}
#define DUTY_CYCLE_90		7600
#define DUTY_CYCLE_MINUS_90	1600
#define DEGREE_DIFF_10		300
#define PERIOD_20_MS		60000


void main(void)

{


	counterADC = 0;
    GPIO_registerInterrupt(BUTTON_PORT, BUTTON_ISR);
   // LED BlinkLED(LED1_ID,PERIODIC_TASK,RGB_BLUE_PORT,RGB_BLUE_PIN,1000);
   // LED BlinkLED2(LED2_ID, PERIODIC_TASK, RGB_GREEN_PORT,RGB_GREEN_PIN,2000);
    Button button(BUTTON_ID,NOT_PERIODIC_TASK,BUTTON_PORT,BUTTON_PIN,200);

    Screen PrintScreen(SCREEN_ID,NOT_PERIODIC_TASK);

    Setup();

    //MainScheduler.attach(&BlinkLED);
    //MainScheduler.attach(&BlinkLED2);
    MainScheduler.attach(&button);
    MainScheduler.attach(&PrintScreen);
    MainScheduler.attach(&Servo1);

    while(1){
    	__wfe();

    	if(SystemTicks != MainScheduler.ticks) {
            MainScheduler.ticks = SystemTicks;
            //MainScheduler.CalculateNextSchedule();
            //MainScheduler.run();
            //MainScheduler.clearNextScheduler();
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
	//WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    /* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P1.0 is connected to the Red LED
	//P1->DIR |= BIT0;

//	GPIO_setAsOutputPin(RGB_RED_PORT,RGB_RED_PIN);
//	GPIO_setOutputLowOnPin(RGB_RED_PORT,RGB_RED_PIN);
//
//	GPIO_setAsOutputPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
//	GPIO_setOutputLowOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
//
//	GPIO_setAsOutputPin(RGB_GREEN_PORT,RGB_GREEN_PIN);
//	GPIO_setOutputLowOnPin(RGB_GREEN_PORT,RGB_GREEN_PIN);

	GPIO_setAsOutputPin(LED_RED_PORT,LED_RED_PIN);
	GPIO_setOutputLowOnPin(LED_RED_PORT,LED_RED_PIN);

	GPIO_setAsOutputPin(LED_BLUE_PORT,LED_BLUE_PIN);
	GPIO_setOutputLowOnPin(LED_BLUE_PORT,LED_BLUE_PIN);

	GPIO_setAsOutputPin(LED_GREEN_PORT,LED_GREEN_PIN);
	GPIO_setOutputLowOnPin(LED_GREEN_PORT,LED_GREEN_PIN);

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Disable all interrupts
	// - Configure Timer32_1  with MCLK (3Mhz), Division by 1, Enable the interrupt, Periodic Mode
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	// - Re-enable interrupts
    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

	/* Configures Pin 4.0, 4.2, and 6.1 as ADC input */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2, GPIO_TERTIARY_MODULE_FUNCTION);
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION);

	    /* Initializing ADC (ADCOSC/64/8) */
	MAP_ADC14_enableModule();
	ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_64, ADC_DIVIDER_8,0);

	/* Configuring ADC Memory (ADC_MEM0 - ADC_MEM2 (A11, A13, A14)  with no repeat)
	 * with internal 2.5v reference */
	MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM2, true);
	MAP_ADC14_configureConversionMemory(ADC_MEM0,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A14, ADC_NONDIFFERENTIAL_INPUTS);

	MAP_ADC14_configureConversionMemory(ADC_MEM1,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A13, ADC_NONDIFFERENTIAL_INPUTS);

	MAP_ADC14_configureConversionMemory(ADC_MEM2,ADC_VREFPOS_AVCC_VREFNEG_VSS,ADC_INPUT_A11, ADC_NONDIFFERENTIAL_INPUTS);

//	/* Enabling the interrupt when a conversion on channel 2 (end of sequence)
//	*  is complete and enabling conversions */
	MAP_ADC14_enableInterrupt(ADC_INT2);
//
//	/* Enabling Interrupts */
	MAP_Interrupt_enableInterrupt(INT_ADC14);
	MAP_Interrupt_enableMaster();
//
//	/* Setting up the sample timer to automatically step through the sequence
//	* convert.
//	*/
	MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
//
//	/* Triggering the start of the sample */
	ADC14_enableConversion();
	ADC14_toggleConversionTrigger();


	__disable_irq();
	//TIMER32_1->LOAD = 0x002DC6C0; //~1s ---> a 3Mhz
	TIMER32_1->LOAD = 0x0000BB80; //~1ms ---> a 48Mhz
	//TIMER32_1->LOAD = 0x00005DC0; //~1ms ---> a 48Mhz
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
		//P1->OUT ^= BIT0;
	    //GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
	   // GPIO_toggleOutputOnPin(LED_BLUE_PORT,LED_BLUE_PIN);


		SystemTicks++;
		return;
	}
}
