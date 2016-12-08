#define __NOP __nop

/*******************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////
// Includes
//////////////////////////////////////////////////////////////////////////////////////////////
#include "msp.h"
#include "main.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#include "Button.hpp"
#include "Screen.hpp"
#include "GamePiece.hpp"
#include "Racket.hpp"
#include "Ball.hpp"
#include <driverlib.h>
#include <stdlib.h>
#include "task_ids.hpp"
/*******************************************************************************************/


/*******************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////
// Definición de defines.
//////////////////////////////////////////////////////////////////////////////////////////////
#define RACKET_LIMIT_X_LEFT 	119
#define RACKET_LIMIT_X_RIGHT 	119
#define RACKET_LIMIT_Y_UP 		103
#define RACKET_LIMIT_Y_DOWN 	24
#define RACKET_LEFT_PERIOD 		100
#define RACKET_RIGHT_PERIOD 	100
#define BALL_PERIOD				100
/*******************************************************************************************/



/*******************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////
// Definición de variables globales.
//////////////////////////////////////////////////////////////////////////////////////////////
// Valor del contador de ms.
volatile static uint64_t SystemTicks = 0;
int* positionDown = new int();
int* positionUp = new int();

// Punteros para enviar como datos en los msjs.
//int* DataToServo = new int();

// Arreglo y puntero al arreglo para la conversión ADC.
int *aDataFromADC = new int();
int counterDown = 0;
int counterUp = 0;

int aDataFromButton[1] = {0};
int *pDataToScreenB = aDataFromButton;
// Una instancia global del Scheduler para que los msjs puedan ser agregados
// por las interrupciones.
Scheduler MainScheduler;
/*******************************************************************************************/










/*******************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////
// Método main
//////////////////////////////////////////////////////////////////////////////////////////////
// El método se encarga de toda la configuración y creación de tareas del RTOS. Así también
// en su ciclo principal se dedica a que cada 1ms (definido así por el Timer32) se ejecuten
// los procesos del scheduler necesarios para el correcto funcionamiento del scheduler.
//////////////////////////////////////////////////////////////////////////////////////////////
void main(void) {

//	Button ButtonDown(BUTTON_DOWN_ID,NOT_PERIODIC_TASK,BUTTON_DOWN_PORT,BUTTON_DOWN_PIN,200);
//	Button ButtonUp(BUTTON_UP_ID,NOT_PERIODIC_TASK,BUTTON_UP_PORT,BUTTON_UP_PIN,200);

	//
	int RacketLimitsX[2] = {RACKET_LIMIT_X_LEFT,RACKET_LIMIT_X_RIGHT};
	int *pRacketLimitsX = RacketLimitsX;

	int RacketLimitsY[2] = {RACKET_LIMIT_Y_UP,RACKET_LIMIT_Y_DOWN};
	int *pRacketLimitsY = RacketLimitsY;
	//

	// Se crean los objetos de pantalla y servo para controlar ambos dispositivos desde el
	// scheduler.
    Screen PrintScreen(SCREEN_ID,NOT_PERIODIC_TASK);
	Racket RacketLeft(RACKET_LEFT_ID,PERIODIC_TASK,RACKET_LEFT_PERIOD,120,63,8,NO_MOVE,NO_MOVE,
			pRacketLimitsX,pRacketLimitsY);


    // Se realizan las configuraciones principales del RTOS.
    Setup();

    // Se agregan los punteros de los tasks creados al scheduler.
    MainScheduler.attach(&PrintScreen);
//    MainScheduler.attach(&ButtonDown);
//    MainScheduler.attach(&ButtonUp);
    MainScheduler.attach(&RacketLeft);


    // Ciclo principal. Cada 1ms entra a ejecutar los procesos necesarios para el correcto
    // funcionamiento del RTOS.
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
/*******************************************************************************************/










/*******************************************************************************************/
// INTERRUPCIONES
/*******************************************************************************************/
extern "C" {

void BUTTON_DOWN_ISR(void) {

	if(GPIO_getInterruptStatus(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN)) {
		// Clear interrupt flag and toggle output LEDs.
		GPIO_disableInterrupt(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
		GPIO_clearInterruptFlag(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
		//GPIO_toggleOutputOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);

		MSG message = {BUTTON_DOWN_ISR_ID,RACKET_LEFT_ID,0,0,SUPRESSION_TIME};
		MainScheduler.attachMessage(message);
	}



}

void BUTTON_UP_ISR(void) {

	// ISR for PIN5
	if(GPIO_getInterruptStatus(BUTTON_UP_PORT, BUTTON_UP_PIN)) {
		// Clear interrupt flag and toggle output LEDs.
		GPIO_disableInterrupt(BUTTON_UP_PORT, BUTTON_DOWN_PIN);
		GPIO_clearInterruptFlag(BUTTON_UP_PORT, BUTTON_UP_PIN);
		//GPIO_toggleOutputOnPin(RGB_RED_PORT,RGB_RED_PIN);

		MSG message = {BUTTON_UP_ISR_ID,RACKET_LEFT_ID,0,0,SUPRESSION_TIME};
		MainScheduler.attachMessage(message);
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////
// ISR para el Timer32
//////////////////////////////////////////////////////////////////////////////////////////////
// La interrupción se realiza cada 1ms y cambia el valor de SystemTicks para indicarle al
// programa principal que ya ha transcurrido tal cantidad de tiempo.
//////////////////////////////////////////////////////////////////////////////////////////////
void T32_INT1_IRQHandler() {

	TIMER32_1->INTCLR = 0U;
	SystemTicks++;

}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
// ISR para la interrupción del ADC.
//////////////////////////////////////////////////////////////////////////////////////////////
// La interrupción se realiza cada vez que se termina una conversión y es almacenada en el
// ADC_MEM2. Cuando estos sucede, la interrupción recoge los resultados de conversión y los
// envía en msjs a la pantalla y al servo para que estos reflejen los cambios. La bandera de
// interrupción es limpiada hasta que el scheduler procesa los msjs y limpia la cola de msjs.
// Así se evita un sobre envío de msjs.
//////////////////////////////////////////////////////////////////////////////////////////////
void ADC14_IRQHandler(void) {

	//GPIO_toggleOutputOnPin(RGB_RED_PORT,RGB_RED_PIN);

    // Si la conversión fue completada
	// Extraiga los datos de la memoria del ADC.
	*aDataFromADC = ADC14_getResult(ADC_MEM0);

	// Envíe el msj a la pantalla para reflejar el cambio.
	MSG changeLeftRacket = {ADC_ISR_ID,RACKET_LEFT_ID,aDataFromADC,0,SUPRESSION_TIME};
	MainScheduler.attachMessage(changeLeftRacket);

	uint64_t g_u64Status = MAP_ADC14_getEnabledInterruptStatus();
	MAP_ADC14_disableInterrupt(ADC_INT0);
	MAP_ADC14_clearInterruptFlag(g_u64Status);

}
}
//////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************/















/*******************************************************************************************/
// SETUP
/*******************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////
// SetupADC
//////////////////////////////////////////////////////////////////////////////////////////////
// Método que se encarga de realizar toda la configuración del ADC para que lea los valores del
// acelerómetro, los convierta en valores digitales y envíe el cambio en forma de msjs a la
// pantalla y servo para que estos reflejen los cambios al usuario en el giroscopio.
//////////////////////////////////////////////////////////////////////////////////////////////
void setupADC() {

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
	//MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

	/* Configures Pin 4.0, 4.2, and 6.1 as ADC input */
    /* Configures Pin 6.0 and 4.4 as ADC input */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);

	/* Initializing ADC (ADCOSC/64/8) */
	MAP_ADC14_enableModule();
	ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_64, ADC_DIVIDER_8,0);

	/* Configuring ADC Memory (ADC_MEM0 - ADC_MEM2 (A11, A13, A14)  with no repeat)
	 * with internal 2.5v reference */
	MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
	MAP_ADC14_configureConversionMemory(ADC_MEM0,
	            ADC_VREFPOS_AVCC_VREFNEG_VSS,
	            ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

	/* Enabling the interrupt when a conversion on channel 2 (end of sequence)
	*  is complete and enabling conversions */
	MAP_ADC14_enableInterrupt(ADC_INT0);

	/* Enabling Interrupts */
	MAP_Interrupt_enableInterrupt(INT_ADC14);
	MAP_Interrupt_enableMaster();

	/* Setting up the sample timer to automatically step through the sequence
	* convert.
	*/
	MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

	/* Triggering the start of the sample */
	ADC14_enableConversion();
	ADC14_toggleConversionTrigger();

}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
// SetupButtonUp
//////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////
void setupButtonUp() {

	//////////////////////////////////////////////////////////////////////////
	GPIO_setAsInputPinWithPullUpResistor(BUTTON_UP_PORT, BUTTON_UP_PIN);
	// Configuring and enabling P2.6 interrupt triggered by a HIGH TO LOW transition
	GPIO_interruptEdgeSelect(BUTTON_UP_PORT, BUTTON_UP_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_clearInterruptFlag(BUTTON_UP_PORT, BUTTON_UP_PIN);
	GPIO_enableInterrupt(BUTTON_UP_PORT, BUTTON_UP_PIN);
	GPIO_registerInterrupt(BUTTON_UP_PORT, BUTTON_UP_ISR);
	//////////////////////////////////////////////////////////////////////////


}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
// SetupButtonDown
//////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////
void setupButtonDown() {

	//////////////////////////////////////////////////////////////////////////
	GPIO_setAsInputPinWithPullUpResistor(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
	// Configuring and enabling P2.6 interrupt triggered by a HIGH TO LOW transition
	GPIO_interruptEdgeSelect(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_clearInterruptFlag(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
	GPIO_enableInterrupt(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
	GPIO_registerInterrupt(BUTTON_DOWN_PORT, BUTTON_DOWN_ISR);
	//////////////////////////////////////////////////////////////////////////


}
//////////////////////////////////////////////////////////////////////////////////////////////








//////////////////////////////////////////////////////////////////////////////////////////////
// Setup
//////////////////////////////////////////////////////////////////////////////////////////////
// Método principal que se encarga de la configuración del micro antes de timer32 que
// comience el ciclo principal en el main.
//////////////////////////////////////////////////////////////////////////////////////////////
void Setup(void)
{
    /* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();


    GPIO_setAsOutputPin(RGB_RED_PORT,RGB_RED_PIN);
	GPIO_setOutputLowOnPin(RGB_RED_PORT,RGB_RED_PIN);

	GPIO_setAsOutputPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
	GPIO_setOutputLowOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);

	GPIO_setAsOutputPin(RGB_GREEN_PORT,RGB_GREEN_PIN);
	GPIO_setOutputLowOnPin(RGB_GREEN_PORT,RGB_GREEN_PIN);

	// Se configuran los botones.
	setupButtonUp();
	setupButtonDown();

	// Se configura el ADC.
	setupADC();

	// Se configura la interrupción del Timer32 para que interrumpa con un periodo de 1ms.
	__disable_irq();
	TIMER32_1->LOAD = 0x0000BB80; //~1ms ---> a 48Mhz
	//TIMER32_1->LOAD = 0x00005DC0; //~2ms ---> a 48Mhz
	TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
	NVIC_SetPriority(T32_INT1_IRQn,1);
	NVIC_EnableIRQ(T32_INT1_IRQn);
	__enable_irq();

	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************************/