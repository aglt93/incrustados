//////////////////////////////////////////////////////////////////////////////////////////////
#include "Servo.hpp"
#include <driverlib.h>
#include "task_ids.hpp"
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
#define DUTY_CYCLE_90		7600
#define DUTY_CYCLE_MINUS_90	1600
#define DEGREE_DIFF_10		300
#define PERIOD_20_MS		60000
//////////////////////////////////////////////////////////////////////////////////////////////
extern "C" {

	Timer_A_PWMConfig pwmConfig = {
									TIMER_A_CLOCKSOURCE_SMCLK,
									TIMER_A_CLOCKSOURCE_DIVIDER_16,
									PERIOD_20_MS,
									TIMER_A_CAPTURECOMPARE_REGISTER_2,
									TIMER_A_OUTPUTMODE_RESET_SET,
									DUTY_CYCLE_MINUS_90
								  };
}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
Servo::Servo(int i_iTaskID, bool i_bPeriodicTask, int i_iServoPort,int i_iServoPin)
{
    // Se configuran los atributos de la clase.
	m_iTaskID = i_iTaskID;
	m_iServoPort = i_iServoPort;
	m_iServoPin = i_iServoPin;
	m_bPeriodicTask = i_bPeriodicTask;

	// Se configura el puerto del servo como PWM.
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(m_iServoPort, m_iServoPin, GPIO_PRIMARY_MODULE_FUNCTION);

	// Se configura el PWM para tener una configuración inicial.
	MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

	// Se pone como valores iniciales de la cuenta ceros, no son necesarios debido
	// a que el task no es periodico.
	m_u64CurrentCount = 0;
	m_u64FinalCount = 0;

}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
MSG Servo::run(void) {

	MSG nullMSG = {-1,-1,0,0,1};
	return nullMSG;

}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
void Servo::ProcessMessage(MSG i_Message) {

	int l_iSourceTask = i_Message.source;
	int* l_pDataTask = (int*) i_Message.data;

	// Dependiendo del task que haya enviado el msj se ejecuta el procesamiento correspondiente.
	switch(l_iSourceTask) {

        // Caso ADC
        case ADC_ISR_ID:

        	// Se convierte el valor de ADC a un valor procesable por el servo.
        	int l_iServoValue = ADCtoServoValueConv(*l_pDataTask);

        	// Se cambia el valor del servo en grados.
        	if(pwmConfig.dutyCycle >= DUTY_CYCLE_90)
				pwmConfig.dutyCycle = DUTY_CYCLE_MINUS_90;
			else
				pwmConfig.dutyCycle = DUTY_CYCLE_MINUS_90 + l_iServoValue;

        	// Se reconfigura el PWM para reflejar el cambio.
			MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

            break;

	};

}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
int Servo::ADCtoServoValueConv(int i_iADCvalue){

	int l_iServoValue;

	l_iServoValue = (float) (i_iADCvalue-5050)/1.2;

	return l_iServoValue;

}
//////////////////////////////////////////////////////////////////////////////////////////////
