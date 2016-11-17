/*
 * Screen.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author:
 */
//////////////////////////////////////////////////////////////////////////////////////////////
#ifndef Screen_HPP_
#define Screen_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include "Crystalfontz128x128_ST7735.h"
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
class Screen : public Task
{
    public:
	    Screen(int i_iTaskID, bool i_bPeriodicTask);
        virtual uint8_t run(void);
        virtual void ProcessMessage(MSG i_Message);
        virtual void printScreen(uint8_t x, uint8_t y, uint16_t z);
        virtual int ADCtoScreenValueConv(int i_iADCvalue);


    protected:
    private:

};
//////////////////////////////////////////////////////////////////////////////////////////////

#endif /* Screen_HPP_ */
