#include "Servo.hpp"
#include <driverlib.h>

Servo::Servo(int i_iTaskID, bool i_bPeriodicTask, int LED_PORT,int LED_PIN)
{
    //ctor
	m_iTaskID = i_iTaskID;
	m_iServoPort = LED_PORT;
	m_iServoPin = LED_PIN;
	m_bPeriodicTask = i_bPeriodicTask;

	GPIO_setAsOutputPin(m_iServoPort,m_iServoPin);
	GPIO_setOutputLowOnPin(m_iServoPort,m_iServoPin);

	m_u64CurrentCount = 0;
	m_u64FinalCount = 0;

}

uint8_t Servo::run(void)
{
    //#########################
    // Blink code
    //#########################

	GPIO_toggleOutputOnPin(m_iServoPort,m_iServoPin);

    return(NO_ERR);
}
