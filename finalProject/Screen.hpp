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

struct FigureChange
{
	bool RacketLeftChange;
	bool RacketRightChange;
	bool BallChange;
};
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
class Screen : public Task
{
    public:
	    Screen(int i_iTaskID, bool i_bPeriodicTask);
        virtual MSG run(void);
        virtual MSG ProcessMessage(MSG i_Message);
        MSG printFigure();
        void getFigureChange(MSG i_Message);
        void printPongTable();

        void printPongWinner();
        void printPongScore(MSG i_Message);
    protected:
    private:
        Graphics_Rectangle RacketLeft;
    	int RacketLeftPosX;
    	int RacketLeftPosY;

    	Graphics_Rectangle RacketRight;
    	int RacketRightPosX;
    	int RacketRightPosY;

    	Graphics_Rectangle ScreenBall;
    	int BallPosX;
    	int BallPosY;

    	Graphics_Rectangle Net;

    	FigureChange ScreenFigureChanges;

};
//////////////////////////////////////////////////////////////////////////////////////////////

#endif /* Screen_HPP_ */
