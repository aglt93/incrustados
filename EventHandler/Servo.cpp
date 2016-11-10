#include "Servo.hpp"
#include <driverlib.h>
#include "task_ids.hpp"

#define DUTY_CYCLE_90		6648
#define DUTY_CYCLE_MINUS_90	1500
#define DEGREE_DIFF_10		480
#define PERIOD_20_MS		61440

extern "C" {

	Timer_A_PWMConfig pwmConfig = {
									TIMER_A_CLOCKSOURCE_SMCLK,
									TIMER_A_CLOCKSOURCE_DIVIDER_6,
									PERIOD_20_MS,
									TIMER_A_CAPTURECOMPARE_REGISTER_2,
									TIMER_A_OUTPUTMODE_RESET_SET,
									DUTY_CYCLE_MINUS_90
								  };
}


Servo::Servo(int i_iTaskID, bool i_bPeriodicTask, int i_iServoPort,int i_iServoPin)
{
    //ctor
	m_iTaskID = i_iTaskID;
	m_iServoPort = i_iServoPort;
	m_iServoPin = i_iServoPin;
	m_bPeriodicTask = i_bPeriodicTask;

	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(m_iServoPort, m_iServoPin, GPIO_PRIMARY_MODULE_FUNCTION);
	Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

	m_u64CurrentCount = 0;
	m_u64FinalCount = 2000;

}

uint8_t Servo::run(void)
{
    //#########################
    // Blink code
    //#########################
	if(pwmConfig.dutyCycle >= DUTY_CYCLE_90)
		pwmConfig.dutyCycle = DUTY_CYCLE_MINUS_90;
	else
		pwmConfig.dutyCycle += DEGREE_DIFF_10;


	MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

    return(NO_ERR);
}


void Servo::ProcessMessage(MSG i_Message) {

	int l_iSourceTask = i_Message.source;
	int* l_pDataTask = (int*) i_Message.data;


	switch(l_iSourceTask) {

        // Caso boton
        case PORT3_ISR_ID:

        	int l_iServoValue = ADCtoServoValueConv(*l_pDataTask);

        	if(pwmConfig.dutyCycle >= DUTY_CYCLE_90)
				pwmConfig.dutyCycle = DUTY_CYCLE_MINUS_90;
			else
				pwmConfig.dutyCycle += l_iServoValue;


			MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

            break;

	};

}


int Servo::ADCtoServoValueConv(int i_iADCvalue){

	int l_iServoValue;

	l_iServoValue = i_iADCvalue;
	// Falta hacer la conversion del valor ADC al degree
	return l_iServoValue;

}
