/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef LED_HPP_
#define LED_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"


class LED : public Task
{
    public:
        LED(int i_iTaskID, bool i_bPeriodicTask, int LED_PORT, int LED_PIN);
        LED(int i_iTaskID, bool i_bPeriodicTask, int LED_PORT, int LED_PIN, uint64_t i_u64FinalCount);
        virtual uint8_t run(void);
    protected:
    private:
        int m_iLedPort;
        int m_iLedPin;
};

#endif /* LED_HPP_ */
