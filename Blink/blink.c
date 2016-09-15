//////////////////////////////////////////////////////////////////////////////////////////////

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
//////////////////////////////////////////////////////////////////////////////////////////////
// Bool to indicate that the initial blinking has ended.
bool g_bBlink3Done;
// Bool to indicate that 1 second has passed according to the Timer32.
bool g_bSecondPassed;
// Bool to indicate the lamp mode: smart or manual
// TRUE -> SMART_MODE;	FALSE -> MANUAL_MODE
bool g_bModeSelector;	
// bool that indicates the state of the lamp: on or off.
// TRUE -> LAMP_ON;		FALSE -> LAMP_OFF
bool g_bLampState;
// bool to indicate that the button has been pushed
// and the lamp has to be toggled.
bool g_bToggleLamp;
// int to indicate how many seconds have passed since the last
// true turn on condition either in smart or manual mode.
int g_iSecCount;
int g_iSecCountM;
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
/* Statics */
static volatile int g_fSample1SecBuffer[MAX_SAMPLE];
static volatile int g_fSample10msBuffer[MAX_msSAMPLE];
static volatile int g_fResultsBuffer[MAX_msSAMPLE];
static volatile int16_t g_f10msSample;
static volatile int16_t g_u8Count10ms;
static volatile int16_t g_u8Count10msI;
static volatile uint16_t g_u8Count1Sec;
static volatile uint16_t g_u8Count1SecI;
static volatile bool g_b10msPassed;
static volatile bool g_b10msPassed;
static volatile bool g_bSecSamplerInitDone;
static volatile bool g_bSecSamplerMainDone;
static volatile bool g_bSample5SecDone;
static volatile bool g_b1SecPassed;
static volatile bool g_b5SecPassed;
static volatile bool g_bOutlierDetected;
static volatile int16_t g_fSampleAverage;
static volatile int16_t g_fTotalAverage;
static volatile int16_t g_fSumAverage;
static volatile int16_t g_fTotalAverage;
static volatile int32_t g_f10msSumSample;
static volatile uint16_t g_u8Count1SecM;
int n = 0;
int i = 0;
static volatile int g_fSample1SecBufferM[MAX_SAMPLE];
static volatile int g_fSample10msBufferM[MAX_msSAMPLE];
static volatile int g_fResultsBufferM[MAX_msSAMPLE];
static volatile int16_t g_f10msSampleM;
static volatile int16_t g_u8Count10msM;
static volatile int16_t g_u8Count10msIM;
static volatile uint16_t g_u8Count1SecM;
static volatile uint16_t g_u8Count1SecIM;
static volatile bool g_b10msPassedM;
static volatile bool g_b10msPassedM;
static volatile bool g_bSecSamplerInitDoneM;
static volatile bool g_bSecSamplerMainDoneM;
static volatile bool g_bSample5SecDoneM;
static volatile bool g_b1SecPassedM;
static volatile bool g_b5SecPassedM;
static volatile bool g_bOutlierDetectedM;
static volatile int16_t g_fSampleAverageM;
static volatile int16_t g_fTotalAverageM;
static volatile int16_t g_fSumAverageM;
static volatile int16_t g_fTotalAverageM;
static volatile int16_t g_f10msSumSampleM;
static volatile uint16_t counter;
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Timer A Trigger configuration for 10 ms wave.
const Timer_A_UpModeConfig upModeConfig =
{
        TIMER_A_CLOCKSOURCE_ACLK,            // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,       // ACLK/1 = 32Khz
		328,
        TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, // Disable CCR0
        TIMER_A_DO_CLEAR                     // Clear Counter
};
const Timer_A_CompareModeConfig compareConfig =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_SET_RESET,               // Toggle output but
		328
};
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
// ***********************************
// Main loop
// ***********************************
void main(void) {

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Call the setup method to configure all ports, pins and interruptions.
	Setup();
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Init secuence
	//////////////////////////////////////////////////////////////////////////////////////////////
	// While the sequence is not done keep in the loop.
	while (!g_bBlink3Done) {

	   // If the Timer32 second has passed call the Blink3();
	   if(g_bSecondPassed) {
		   g_bSecondPassed = false;
		   // This method in the sixth second
		   // sets g_bBlink3Done = true to indicate
		   // that the init sequence it's done.
		   Blink3();
	   }

	   if(g_b10msPassed){
		   g_b10msPassed = false;
		   msSamplerInit();
			GPIO_setOutputLowOnPin(LED_RED_PORT,LED_RED_PIN);
		   if(g_b1SecPassed){
			   if(!g_bSecSamplerInitDone){
				   g_bSecSamplerInitDone = secSamplerInit();
					GPIO_setOutputHighOnPin(LED_RED_PORT,LED_RED_PIN);
			   }
			   g_u8Count1Sec++;
		   }
	   }
	   // Wait for events. Since the if conditions above depend on
	   // interruptions, in order to save power, the program waits
	   // until the next interruption to check the conditions again.
	   __wfe();
	}
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Until the system is ready the user can turn on the night lamp manually.
	// Therefore until now the interruption is enabled.
	GPIO_clearInterruptFlag(BUTTON_PORT, BUTTON_PIN);
	GPIO_enableInterrupt(BUTTON_PORT, BUTTON_PIN);

	// The main loop has to accessed inmediatly
	// after the init sequence has ended.
	g_bSecondPassed = true;
	// The default mode for the lamp is smart.
	g_bModeSelector = SMART_MODE;
	g_b1SecPassed = false;
	//////////////////////////////////////////////////////////////////////////////////////////////
	g_u8Count1SecM = 0;
	g_b10msPassedM = true;
	g_b1SecPassedM = false;
	g_u8Count10msIM = 0;
	g_f10msSampleM = 0;
	g_f10msSumSampleM=0;
	g_fSumAverageM = 0;
	g_u8Count10msM=0;
	counter = 0;
	g_iSecCountM = 0;
	g_bOutlierDetectedM= false;
	double result = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Main loop
	//////////////////////////////////////////////////////////////////////////////////////////////
	while(1) {

		if(g_b10msPassedM){
			GPIO_setOutputLowOnPin(LED_RED_PORT,LED_RED_PIN);
			// Gets in each 10 ms driven by interrupt
		    g_b10msPassedM = false;
		    // 1-Counts until 1 second, if is not 1 second then put g_b1SecPassed false, else true.
		    // 2-Collects 100 samples, 1 sample each 10 ms until 1 second.
		    g_fSample10msBuffer[g_u8Count10msM]=g_f10msSample;
		}
		if(g_b1SecPassedM){
			GPIO_setOutputLowOnPin(LED2_RED_PORT,LED2_RED_PIN);
			for( i = 0; i < MAX_msSAMPLE; i++ ){
				g_f10msSumSample += g_fSample10msBuffer[i];
			}
			g_fSampleAverage = g_f10msSumSample/100;
			g_f10msSumSample = 0;
			g_fSample1SecBufferM[g_u8Count1SecM]=g_fSampleAverage;
			g_b1SecPassedM = false;
			if (g_u8Count1SecM<MAX_SAMPLE){
				g_u8Count1SecM++;
				g_iSecCount++;
			}
			else{
				g_u8Count1SecM=0;
				g_iSecCount = 0;
				g_b5SecPassedM = true;
			}

//			if (g_bOutlierDetectedM){
//				if(g_iSecCountM<MAX_SAMPLE){
//					float l_fLux;
//
//					l_fLux = SenseLight();
//					if(l_fLux < LUX_LIMIT){
//					SetLamp(TURN_ON_LAMP);
//					g_bOutlierDetectedM = true;
//					}
//				}
//				else{
//					g_iSecCountM = 0;
//					SetLamp(TURN_OFF_LAMP);
//					g_bOutlierDetectedM = false;
//				}
//				g_iSecCountM++;
//			}
			if (g_bModeSelector == SMART_MODE) {
				if (g_bOutlierDetectedM){
					if(g_iSecCountM<MAX_SAMPLE){
						float l_fLux;
						l_fLux = SenseLight();
						if(l_fLux < LUX_LIMIT){
							SetLamp(TURN_ON_LAMP);
							g_bOutlierDetectedM = true;
						}
					}
					else{
						g_iSecCountM = 0;
						SetLamp(TURN_OFF_LAMP);
						g_bOutlierDetectedM = false;
					}
					g_iSecCountM++;
			}
			else {
				ManualMode();
			}
			if(result>1){
				GPIO_setOutputHighOnPin(LED3_RED_PORT,LED3_RED_PIN);
				g_bOutlierDetectedM = true;
				result = 0;
			}
			result = abs(((g_fSampleAverage-g_fTotalAverageM)/(g_fTotalAverageM))*100);
			g_fResultsBufferM[counter] = ((g_fSampleAverage-g_fTotalAverageM)/(g_fTotalAverageM))*100;

		}
		}
		if(g_b5SecPassedM){
			g_b5SecPassedM = false;
			GPIO_setOutputHighOnPin(LED2_RED_PORT,LED2_RED_PIN);
			int i = 0;
			for( i = 0; i < MAX_msSAMPLE; i++ ){
				g_fSumAverageM += g_fSample1SecBufferM[i];
			}
			g_fTotalAverageM = g_fSumAverageM/5;
			g_fSumAverageM = 0;

		}

	   // If a second in the Timer32 has passed check the 
	   // if the user has choosen the smart or the manual
	   // mode and work as indicated.
//	   if (g_bSecondPassed) {
//		   g_bSecondPassed = false;
//			GPIO_setOutputHighOnPin(LED_RED_PORT,LED_RED_PIN);
//		   if (g_bModeSelector == SMART_MODE) {
//			   SmartMode();
//		   }
//		   else {
//			   ManualMode();
//		   }
//	   }

	   // If the user has pushed the button check if 
	   // the lamp has to be turned on or off.
	   // If the lamp is turned on, reset the seconds count
	   // and activate the manual mode.
	   if (g_bToggleLamp) {

			g_bToggleLamp = false;
			
			// Wait to avoid debounce button issues.
			__delay_cycles(100000);

			if (GPIO_getInputPinValue(BUTTON_PORT,BUTTON_PIN) == 0) {
			
				SetLamp(TOGGLE_LAMP);
				g_bModeSelector = !GetLampState();
				ResetT32_1();
				g_iSecCount = 0;

			}

	   }

	   // Wait for the next event to check conditions
	   // in the loop.
	   __wfe();
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
// Blink3
//////////////////////////////////////////////////////////////////////////////////////////////
// void method to set the lamp state in the init sequence.
// If the lamp is in a pair second turn it on.
// If the lamp is in an odd second turn it off.
// If the init sequence has gotten to its end (sixth second)
// indicate the condition setting g_bBlink3Done = true.
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
bool msSamplerInit(void){
	g_u8Count10msI = g_u8Count10ms-1;
	g_fSample10msBuffer[g_u8Count10msI]=g_f10msSample;
	//printf("g_u8Count10ms = %u\n", g_u8Count10ms );

	if (!(g_u8Count10ms<100)){
		//printf("g_f10msSumSample = %i, count %u\n", g_f10msSumSample ,g_u8Count10ms );
		int i = 0;
		for( i = 0; i < 100; i++ ){
			   g_f10msSumSample += g_fSample10msBuffer[i];
		   }
		g_fSampleAverage = g_f10msSumSample/100;
		//printf("g_fSampleAverage = %i, count %u\n", g_fSampleAverage ,g_u8Count10ms );
		g_u8Count10ms = 0;
		g_f10msSumSample = 0;
		g_b1SecPassed = true;
		return true;
	}
	else{
		g_b1SecPassed = false;
		return false;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
bool secSamplerInit(void){

	g_fSample1SecBuffer[g_u8Count1Sec]=g_fSampleAverage;

	if (g_u8Count1Sec==5){

		int i = 0;
		for( i = 0; i < 5; i++ ){
			g_fSumAverage += g_fSample1SecBuffer[i];
		   }
		g_fTotalAverage = g_fSumAverage/5;
		g_fSumAverage =0;
		g_u8Count1Sec = 0;
		g_bSample5SecDone = true;
		return true;
	}
	else{
	g_bSample5SecDone = false;
	return false;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//bool msSamplerMain(void){
//	g_fSample10msBuffer[g_u8Count10msM]=g_f10msSample;
//
//	if (g_u8Count10msM<100){
//		printf("g_u8Count10msM = %u\n", g_u8Count10msM );
//		//printf("g_f10msSumSample = %i, count %u\n", g_f10msSumSample ,g_u8Count10ms );
////		int i = 0;
////		for( i = 0; i < 100; i++ ){
////			   g_f10msSumSample += g_fSample10msBuffer[i];
////		   }
////		g_fSampleAverage = g_f10msSumSample/100;
////		//printf("g_fSampleAverage = %i, count %u\n", g_fSampleAverage ,g_u8Count10ms );
//	}
//
//}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
bool secSamplerMain(void){
	int i;
	for( i = 0; i < 5; i++ ){
	g_fSumAverageM += g_fSample1SecBufferM[i];
	}
	g_fTotalAverageM = g_fSumAverageM/5;
	g_fSumAverageM = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
bool fiveSecCounter(void){
	//printf("g_u8Count1SecM = %u\n", g_u8Count1SecM );
	if(g_u8Count1SecM<5){
		g_b5SecPassedM = false;
		g_u8Count1SecM++;
	}
	else {
	//printf("g_u8Count1SecM = %u\n", g_u8Count1SecM );
	g_b5SecPassedM = true;
	g_u8Count1SecM = 0;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
bool OutlierDetector(void){
	int resultado;
	resultado = ((g_fSampleAverage-g_fTotalAverage)/(g_fTotalAverage))*100;
	g_fResultsBuffer[n++]=resultado;
	if(resultado>10){
		GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
		return true;
	}
		GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
		return ;
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// SenseLight
//////////////////////////////////////////////////////////////////////////////////////////////
// float method that return the lux sensed by the light
// sensor. This method uses the HAL_OPT3001 lib to get this value.
float SenseLight (void) {

	return OPT3001_getLux();
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// Smart Mode
//////////////////////////////////////////////////////////////////////////////////////////////
// void method to controll the automatic mode of the lamp.
void SmartMode (void) {

	float l_fLux;

	l_fLux = SenseLight();

	// According to the light and sound conditions the method
	// decides to turn on the lamp if the light sensed is under
	// the LUX_LIMIT and if the last second sound sensed is 10% louder
	// than the last 5 seconds. This is refered as turn on condition.
	//if (/*l_fLux < LUX_LIMIT &&*/ g_iSecCountM < 5 ) {
	if(l_fLux < LUX_LIMIT){
		SetLamp(TURN_ON_LAMP);
	}
		//ResetT32_1();
//		g_iSecCountM = 0;

	//}

	// Otherwise if it's been 30min without any turn on condition
	// turn off the lamp.
	//else {
//		g_iSecCountM = 0;
//
//		SetLamp(TURN_OFF_LAMP);
//
//	}

	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
// ManualMode
//////////////////////////////////////////////////////////////////////////////////////////////
// void method to control the manual mode of the lamp.
// In the manual mode, if the lamp is on
// just wait until 30min have passed and then
// turn off the lamp and go back to smart mode.
void ManualMode (void) {

	if (g_iSecCount == 4) {

		SetLamp(TURN_OFF_LAMP);
		ResetT32_1();
		g_iSecCount = 0;
		g_bModeSelector = SMART_MODE;

	}
}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// SetLamp
//////////////////////////////////////////////////////////////////////////////////////////////
// void method to set the lamp on, off or toggle its state.
// Depending on the i_iState value the lamp state is change as follows:
//
// TURN_ON_LAMP 	0
// TURN_OFF_LAMP 	1
// TOGGLE_LAMP 		2
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
// ResetT32_1
//////////////////////////////////////////////////////////////////////////////////////////////
// void method to reset the Timer32 to start counting
// down from the begining.
void ResetT32_1 (void) {

	MAP_Timer32_clearInterruptFlag(TIMER32_BASE);
	MAP_Timer32_setCount(TIMER32_BASE,T32_SEC_COUNT);
	MAP_Timer32_enableInterrupt(TIMER32_BASE);
	MAP_Timer32_startTimer(TIMER32_BASE, true);

}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
// GetLampState
//////////////////////////////////////////////////////////////////////////////////////////////
// bool method that returns the lamp state as follows:
//
// TRUE 	-> Lamp on.
// FLASE	-> Lamp off.
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
// BUTTON_ISR
//////////////////////////////////////////////////////////////////////////////////////////////
// Interrupt Service routine for the port 3.
// If the interrupt is triggered by the pin 5 (button pushed)
// clear the interrupt flag and indicate this condition to the main
// program through the g_bToggleLamp = true
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
// TIMER32_ISR
//////////////////////////////////////////////////////////////////////////////////////////////
// Interrupt service routine for the Timer32 1.
// This interruption is triggered when a second has passed.
// It increases the g_iSecCount to control how much time has passed.
// It also indicates to the main program that a second has passed through the
// g_bSecondPassed = true.
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

}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
/* Interrupt when conversion is completed. Clock drives 10 ms.
 * ADC_MEM0 */
void ADC14_IRQHandler(void)
{
    uint64_t l_u64Status;
    l_u64Status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(l_u64Status);
    if (l_u64Status & ADC_INT0)
    {
    	g_f10msSample= MAP_ADC14_getResult(ADC_MEM0);
    	g_f10msSample = abs(g_f10msSample);
    	g_u8Count10ms++;
    	counter++;
    	if (g_u8Count10msM<100){
        	g_u8Count10msM++;
        	g_b10msPassedM = true;
    	}
    	else{
    		g_b1SecPassedM = true;
    		g_u8Count10msM = 0;
    	}
    	g_b10msPassed = true;
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
	//g_checkLightSensorBool = false;
	g_bBlink3Done = false;
	g_bSecondPassed = true;
	g_bModeSelector = SMART_MODE;
	g_bToggleLamp = false;
	g_iSecCount = 0;
	g_iSecCountM = 0;
	//////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////
	int i = 0;
	for (i = 0; i < MAX_SAMPLE-1; i++){
		g_fSample1SecBuffer[i] = 0.0;
	}
	g_f10msSample = 0.0;
	g_fSampleAverage = 0.0;
	g_fSumAverage = 0;
	g_fTotalAverage = 0;
	g_u8Count10ms = 0;
	g_u8Count10msI = 0;
	g_u8Count1Sec = 0;
	g_u8Count1SecI = 0;
	g_b10msPassed = true;
	g_b1SecPassed = false;
	g_b5SecPassed = false;
	g_bSecSamplerInitDone = false;
	g_bSecSamplerMainDone = false;
	g_bSample5SecDone = false;
	g_f10msSumSample = 0.0;
	g_bOutlierDetected = false;
	g_u8Count1SecM = 0;
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
	GPIO_setAsOutputPin(LED2_RED_PORT,LED2_RED_PIN);
	GPIO_setAsOutputPin(LED3_RED_PORT,LED3_RED_PIN);
	// Initial value of LEDs on low.
	GPIO_setOutputLowOnPin(RGB_RED_PORT,RGB_RED_PIN);
	GPIO_setOutputLowOnPin(RGB_GREEN_PORT,RGB_GREEN_PIN);
	GPIO_setOutputLowOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
	GPIO_setOutputLowOnPin(LED_RED_PORT,LED_RED_PIN);
	GPIO_setOutputLowOnPin(LED2_RED_PORT,LED2_RED_PIN);
	GPIO_setOutputLowOnPin(LED3_RED_PORT,LED3_RED_PIN);
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
