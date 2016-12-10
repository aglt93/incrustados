/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef BUTTON_HPP_
#define BUTTON_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"


class Button : public Task
{
    public:
        Button(int i_iTaskID, bool i_bPeriodicTask, int BUTTON_PORT,int BUTTON_PIN,uint64_t i_u64FinalCount);
        virtual MSG run(void);
        virtual MSG ProcessMessage(MSG i_Message);
    protected:
    private:
        int m_iButtonPort;
        int m_iButtonPin;
        virtual MSG SendMessage();


};

#endif /* BUTTON_HPP_ */
