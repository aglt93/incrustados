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

typedef struct Figure_Change
{
	bool racket_left_change;  			//!< The minimum X coordinate of the rectangle.
	bool racket_right_change;  			//!< The minimum X coordinate of the rectangle.
	bool ball_change;  			//!< The minimum X coordinate of the rectangle.
} Figure_Change;
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
class Screen : public Task
{
    public:
	    Screen(int i_iTaskID, bool i_bPeriodicTask);
        virtual MSG run(void);
        virtual MSG ProcessMessage(MSG i_Message);
        virtual MSG printFigure(Figure_Change figure_change);
        virtual Figure_Change getFigureChange(MSG i_Message);
        virtual void printPongTable();
        virtual void printPongScore();
        virtual void printPongWinner();

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

    	Graphics_Rectangle net;

    	Figure_Change figure_change;

};
//////////////////////////////////////////////////////////////////////////////////////////////

#endif /* Screen_HPP_ */
