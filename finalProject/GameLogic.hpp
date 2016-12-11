/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef GAMELOGIC_HPP_
#define GAMELOGIC_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include "Racket.hpp"
#include "Ball.hpp"

#define NO_MOVE 0
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 1
#define MOVE_RIGHT 2

#define LIMITSX_SIZE 2
#define LIMITSY_SIZE 2

class GameLogic : public Task
{
    public:
		GameLogic (int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount);

		virtual MSG run(void);
		virtual MSG ProcessMessage(MSG i_message);
		Racket RacketLeft;
		Racket RacketRight;
		Ball   MainBall;


    protected:

    private:

};

#endif /* LED_HPP_ */
