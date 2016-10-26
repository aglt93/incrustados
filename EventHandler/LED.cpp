#include "LED.hpp"
#include <driverlib.h>

#define RGB_BLUE_PORT GPIO_PORT_P5
#define RGB_BLUE_PIN GPIO_PIN6

LED::LED()
{
    //ctor
	GPIO_setAsOutputPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
	GPIO_setOutputLowOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
	m_u64CurrentCount = 0;
	m_u64FinalCount = 0;

}

LED::LED(uint64_t i_u64FinalCount)
{
    //ctor
	GPIO_setAsOutputPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
	GPIO_setOutputLowOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);
	m_u64CurrentCount = 0;
	m_u64FinalCount = i_u64FinalCount;

}

uint8_t LED::run(void)
{
    //#########################
    // Blink code
    //#########################

	GPIO_toggleOutputOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);

    return(NO_ERR);
}
