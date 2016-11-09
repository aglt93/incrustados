/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef SERVO_HPP_
#define SERVO_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"


class Servo : public Task
{
    public:
        Servo(int i_iTaskID, bool i_bPeriodicTask, int SERVO_PORT, int SERVO_PIN);
        virtual uint8_t run(void);
    protected:
    private:
        int m_iServoPort;
        int m_iServoPin;
        int m_i;
};

#endif /* LED_HPP_ */
