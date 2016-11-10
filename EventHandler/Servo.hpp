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
        Servo(int i_iTaskID, bool i_bPeriodicTask, int servo_port, int servo_pin);
        virtual uint8_t run(void);
        virtual void ProcessMessage(MSG i_Message);
        int ADCtoServoValueConv(int i_iADCvalue);
    protected:
    private:
        int m_iServoPort;
        int m_iServoPin;
        int m_i;

};

#endif /* LED_HPP_ */
