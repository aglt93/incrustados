#define __NOP __nop

/*******************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////
// Includes
//////////////////////////////////////////////////////////////////////////////////////////////
#include "msp.h"
#include "main.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include "Screen.hpp"
#include "GamePiece.hpp"
#include "Racket.hpp"
#include "Ball.hpp"
#include "GameLogic.hpp"
#include "Buzzer.hpp"
#include <driverlib.h>
#include <stdlib.h>
#include "task_ids.hpp"
#include "game_env.hpp"
/*******************************************************************************************/



/*******************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////
// Definición de variables globales.
//////////////////////////////////////////////////////////////////////////////////////////////
// Valor del contador de ms.
volatile static uint64_t SystemTicks = 0;

// Entero para enviar como msj con el resultado de la conversión ADC.
int *aDataFromADC = new int();

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

	// Se crean los objetos de pantalla, el juego y la música para controlarlos como tareas del scheduler.
    Screen PrintScreen(SCREEN_ID,NOT_PERIODIC_TASK);
	GameLogic PingPong(LOGIC_ID,PERIODIC_TASK,LOGIC_PERIOD);
	//Buzzer BuzzerTest(SERVO_ID,PERIODIC_TASK,BUZZER_PORT,BUZZER_PIN,BUZZER_PERIOD);

    // Se realizan las configuraciones principales del RTOS.
    Setup();

    // Se agregan los punteros de los tasks creados al scheduler.
    MainScheduler.attach(&PrintScreen);
    MainScheduler.attach(&PingPong);
    //MainScheduler.attach(&BuzzerTest);


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


//////////////////////////////////////////////////////////////////////////////////////////////
// ISR para el botón de abajo del Booster Pack
//////////////////////////////////////////////////////////////////////////////////////////////
// Interrupción que se realiza cada vez que se presione el botón de abajo del BoosterPack
// se encarga de enviar un msj a la lógica del juego para indicar esta condición. No es necesario
// enviar datos entonces se envía un NULL pointer.
//////////////////////////////////////////////////////////////////////////////////////////////
void BUTTON_DOWN_ISR(void) {

	if(GPIO_getInterruptStatus(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN)) {
		GPIO_disableInterrupt(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
		GPIO_clearInterruptFlag(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
		//GPIO_toggleOutputOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);

		MSG message = {BUTTON_DOWN_ISR_ID,LOGIC_ID,0,0,SUPRESSION_TIME};
		MainScheduler.attachMessage(message);
	}



}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
// ISR para el botón de arriba del Booster Pack
//////////////////////////////////////////////////////////////////////////////////////////////
// Interrupción que se realiza cada vez que se presione el botón de arriba del BoosterPack
// se encarga de enviar un msj a la lógica del juego para indicar esta condición. No es necesario
// enviar datos entonces se envía un NULL pointer.
//////////////////////////////////////////////////////////////////////////////////////////////
void BUTTON_UP_ISR(void) {

	if(GPIO_getInterruptStatus(BUTTON_UP_PORT, BUTTON_UP_PIN)) {
		GPIO_disableInterrupt(BUTTON_UP_PORT, BUTTON_DOWN_PIN);
		GPIO_clearInterruptFlag(BUTTON_UP_PORT, BUTTON_UP_PIN);
		//GPIO_toggleOutputOnPin(RGB_RED_PORT,RGB_RED_PIN);

		MSG message = {BUTTON_UP_ISR_ID,LOGIC_ID,0,0,SUPRESSION_TIME};
		MainScheduler.attachMessage(message);
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////



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
// ADC_MEM0. Cuando estos sucede, la interrupción recoge los resultados de conversión y los
// envía en msjs a la logica del juego para que este refleje los cambios. La bandera de
// interrupción es limpiada hasta que el scheduler procesa los msjs y limpia la cola de msjs.
// Así se evita un sobre envío de msjs.
//////////////////////////////////////////////////////////////////////////////////////////////
void ADC14_IRQHandler(void) {

	//GPIO_toggleOutputOnPin(RGB_RED_PORT,RGB_RED_PIN);

    // Si la conversión fue completada
	// Extraiga los datos de la memoria del ADC.
	*aDataFromADC = ADC14_getResult(ADC_MEM0);

	// Envíe el msj a la pantalla para reflejar el cambio.
	MSG changeLeftRacket = {ADC_ISR_ID,LOGIC_ID,aDataFromADC,0,SUPRESSION_TIME};
	MainScheduler.attachMessage(changeLeftRacket);

	// Deshabilite la interrupción.
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
// joystick en la posición Y y los convierta en valores digitales.
//////////////////////////////////////////////////////////////////////////////////////////////
void setupADC() {

	MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

	// Configure las memorias.
	MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);

	// Configure como entrada al ADC el puerto del joystick para leer las posiciones de Y.
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);

	/* Initializing ADC (ADCOSC/64/8) */
	MAP_ADC14_enableModule();
	ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_64, ADC_DIVIDER_8,0);

	/* Configurar ADC Memory en single sample mode para la memoria ADC0 */
	MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
	MAP_ADC14_configureConversionMemory(ADC_MEM0,
	            ADC_VREFPOS_AVCC_VREFNEG_VSS,
	            ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

	// Habilitando interrupciones cuando se concluya una conversión en ADC0.
	MAP_ADC14_enableInterrupt(ADC_INT0);

	// Habilitando la interrupción del ADC.
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
// Configuraciones para permitir que el botón de arriba del BoosterPack funcione con
// interrupciones cuando el usuario presiona el botón.
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
// Configuraciones para permitir que el botón de abajo del BoosterPack funcione con
// interrupciones cuando el usuario presiona el botón.
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
