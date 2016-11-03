/*
 * Screen.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef Screen_HPP_
#define Screen_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include "Crystalfontz128x128_ST7735.h"


class Screen : public Task
{
    public:
        Screen(int Screen_PORT,int Screen_PIN);
        Screen(int Screen_PORT,int Screen_PIN,uint64_t i_u64FinalCount);
        virtual uint8_t run(void);
    protected:
    private:
        int m_iScreenPort;
        int m_iScreenPin;
};

#endif /* Screen_HPP_ */
