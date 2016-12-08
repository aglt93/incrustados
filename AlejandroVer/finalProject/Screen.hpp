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
        virtual MSG run(void);
        virtual void ProcessMessage(MSG i_Message);
        virtual void printFigure(Graphics_Rectangle figure);
        virtual int ADCtoScreenValueConv(int i_iADCvalue);


    protected:
    private:
        Graphics_Rectangle RacketLeft;
    	int RacketLeftPosX;
    	int RacketLeftPosY;

    	Graphics_Rectangle RacketRight;
    	int RacketRightPosX;
    	int RacketRightPosY;

    	Graphics_Rectangle Ball;
    	int BallPosX;
    	int BallPosY;


};
//////////////////////////////////////////////////////////////////////////////////////////////

#endif /* Screen_HPP_ */
