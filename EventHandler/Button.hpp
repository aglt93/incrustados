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
        Button(int i_iTaskID, int BUTTON_PORT,int BUTTON_PIN,uint64_t i_u64FinalCount);
        virtual uint8_t run(void);
        bool m_bRunTask;
        int m_iMsgClass;
    protected:
    private:
        int m_iButtonPort;
        int m_iButtonPin;
        virtual MSG SendMessage();
        virtual void ProcessMessage(MSG* i_Message);

};

#endif /* BUTTON_HPP_ */
