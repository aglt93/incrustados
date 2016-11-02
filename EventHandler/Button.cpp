#include "Button.hpp"
#include <driverlib.h>

#define LED_RED_PORT GPIO_PORT_P2
#define LED_RED_PIN GPIO_PIN6

Button::Button(int BUTTON_PORT,int BUTTON_PIN,uint64_t i_u64FinalCount)
{
    //ctor
	m_iButtonPort = BUTTON_PORT;
	m_iButtonPin = BUTTON_PIN;

	GPIO_setAsInputPinWithPullUpResistor(BUTTON_PORT, BUTTON_PIN);

	// Configuring and enabling P2.6 interrupt triggered by a HIGH TO LOW transition

	GPIO_interruptEdgeSelect(BUTTON_PORT, BUTTON_PIN, GPIO_HIGH_TO_LOW_TRANSITION);


	GPIO_clearInterruptFlag(BUTTON_PORT, BUTTON_PIN);
   	GPIO_enableInterrupt(BUTTON_PORT, BUTTON_PIN);

	m_u64CurrentCount = 0;
	m_u64FinalCount = i_u64FinalCount;

	m_bRunTask = false;

}
/*
LED::LED(int LED_PORT,int LED_PIN, uint64_t i_u64FinalCount)
{
    //ctor
	m_iLedPort = LED_PORT;
	m_iLedPin = LED_PIN;

	GPIO_setAsOutputPin(m_iLedPort,m_iLedPin);
	GPIO_setOutputLowOnPin(m_iLedPort,m_iLedPin);

	m_u64CurrentCount = 0;
	m_u64FinalCount = i_u64FinalCount;

}
*/

uint8_t Button::run(void)
{
	if (m_bRunTask) {
		m_bRunTask = false;
		GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
	}


    return(NO_ERR);
}

MSG Button::SendMessage(){

	MSG testMessage = {
			0,
			0,
			0,
	};


	return testMessage;
}


void Button::ProcessMessage(MSG* i_Message){
	return;
}
