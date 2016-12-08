#include "LED.hpp"
#include <driverlib.h>

LED::LED(int i_iTaskID, bool i_bPeriodicTask, int LED_PORT,int LED_PIN)
{
    //ctor
	m_iTaskID = i_iTaskID;
	m_iLedPort = LED_PORT;
	m_iLedPin = LED_PIN;
	m_bPeriodicTask = i_bPeriodicTask;

	GPIO_setAsOutputPin(m_iLedPort,m_iLedPin);
	GPIO_setOutputLowOnPin(m_iLedPort,m_iLedPin);

	m_u64CurrentCount = 0;
	m_u64FinalCount = 0;

}

LED::LED(int i_iTaskID, bool i_bPeriodicTask, int LED_PORT,int LED_PIN, uint64_t i_u64FinalCount)
{
    //ctor
	m_iTaskID = i_iTaskID;
	m_iLedPort = LED_PORT;
	m_iLedPin = LED_PIN;
	m_bPeriodicTask = i_bPeriodicTask;

	GPIO_setAsOutputPin(m_iLedPort,m_iLedPin);
	GPIO_setOutputLowOnPin(m_iLedPort,m_iLedPin);

	m_u64CurrentCount = 0;
	m_u64FinalCount = i_u64FinalCount;

}

MSG LED::run(void)
{
    //#########################
    // Blink code
    //#########################

	GPIO_toggleOutputOnPin(m_iLedPort,m_iLedPin);
	MSG nullMSG = {-1,-1,0,0,1};
	return nullMSG;
}
