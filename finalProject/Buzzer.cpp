//////////////////////////////////////////////////////////////////////////////////////////////
#include "Buzzer.hpp"
#include <driverlib.h>
#include "task_ids.hpp"
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
#define PERIOD		0
#define NOTE_OFFSET	3000
//////////////////////////////////////////////////////////////////////////////////////////////
extern "C" {

	Timer_A_PWMConfig pwmConfig = {
									TIMER_A_CLOCKSOURCE_SMCLK,
									TIMER_A_CLOCKSOURCE_DIVIDER_16,
									PERIOD,
									TIMER_A_CAPTURECOMPARE_REGISTER_4,
									TIMER_A_OUTPUTMODE_RESET_SET,
									PERIOD/2
								  };
}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
Buzzer::Buzzer(int i_iTaskID, bool i_bPeriodicTask, int i_iBuzzerPort,int i_iBuzzerPin, uint64_t i_u64FinalCount)
{
    // Se configuran los atributos de la clase.
	m_iTaskID = i_iTaskID;
	m_iBuzzerPort = i_iBuzzerPort;
	m_iBuzzerPin = i_iBuzzerPin;
	m_bPeriodicTask = i_bPeriodicTask;
	m_iMelodyIndex = 0;
	m_iTempoCounter = 0;

	// Se configura el puerto del servo como PWM.
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(m_iBuzzerPort, m_iBuzzerPin, GPIO_PRIMARY_MODULE_FUNCTION);

	// Se configura el PWM para tener una configuración inicial.
	MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);

	// Se pone como valores iniciales de la cuenta ceros, no son necesarios debido
	// a que el task no es periodico.
	m_u64CurrentCount = 0;
	m_u64FinalCount = i_u64FinalCount;

}

const int Buzzer::Melody[MELODY_LENGTH] = {
										  NOTE_E7, NOTE_E7, 0, NOTE_E7,
										  0, NOTE_C7, NOTE_E7, 0,
										  NOTE_G7, 0, 0,  0,
										  NOTE_G6, 0, 0, 0,

										  NOTE_C7, 0, 0, NOTE_G6,
										  0, 0, NOTE_E6, 0,
										  0, NOTE_A6, 0, NOTE_B6,
										  0, NOTE_AS6, NOTE_A6, 0,

										  NOTE_G6, NOTE_E7, NOTE_G7,
										  NOTE_A7, 0, NOTE_F7, NOTE_G7,
										  0, NOTE_E7, 0, NOTE_C7,
										  NOTE_D7, NOTE_B6, 0, 0,

										  NOTE_C7, 0, 0, NOTE_G6,
										  0, 0, NOTE_E6, 0,
										  0, NOTE_A6, 0, NOTE_B6,
										  0, NOTE_AS6, NOTE_A6, 0,

										  NOTE_G6, NOTE_E7, NOTE_G7,
										  NOTE_A7, 0, NOTE_F7, NOTE_G7,
										  0, NOTE_E7, 0, NOTE_C7,
										  NOTE_D7, NOTE_B6, 0, 0
										};


const int Buzzer::Tempo[MELODY_LENGTH] = {
										  12, 12, 12, 12,
										  12, 12, 12, 12,
										  12, 12, 12, 12,
										  12, 12, 12, 12,

										  12, 12, 12, 12,
										  12, 12, 12, 12,
										  12, 12, 12, 12,
										  12, 12, 12, 12,

										  9, 9, 9,
										  12, 12, 12, 12,
										  12, 12, 12, 12,
										  12, 12, 12, 12,

										  12, 12, 12, 12,
										  12, 12, 12, 12,
										  12, 12, 12, 12,
										  12, 12, 12, 12,

										  9, 9, 9,
										  12, 12, 12, 12,
										  12, 12, 12, 12,
										  12, 12, 12, 12,
										};
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
MSG Buzzer::run(void) {

	////////////////////////////////////////////////////////
	m_iTempoCounter++;

	if (m_iTempoCounter >= Tempo[m_iMelodyIndex]) {

		m_iTempoCounter = 0;
		m_iMelodyIndex++;
		MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
	}

	if (m_iMelodyIndex >= MELODY_LENGTH) {m_iMelodyIndex = 0;}
	////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////
	float timerPeriod;

	if (Melody[m_iMelodyIndex] != 0) {
		timerPeriod = (float) 1/Melody[m_iMelodyIndex];
		timerPeriod = timerPeriod*1000;
		timerPeriod = timerPeriod * NOTE_OFFSET;
	}
	else {timerPeriod = 0;}

	pwmConfig.timerPeriod = timerPeriod;
	pwmConfig.dutyCycle = timerPeriod/2;

	////////////////////////////////////////////////////////


	MSG nullMSG = {-1,-1,0,0,1};
	return nullMSG;

}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
MSG Buzzer::ProcessMessage(MSG i_Message) {



}
//////////////////////////////////////////////////////////////////////////////////////////////
