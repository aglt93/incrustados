/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef BALL_HPP_
#define BALL_HPP_
#define __NOP __nop
#include "msp.h"
#include "GamePiece.hpp"


class Ball : public GamePiece
{
    public:
		Ball(int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount, int i_iPosX,
				int i_iPosY, int i_iSize, int i_iDirectionX, int i_iDirectionY,
				int* i_iLimitsX, int* i_iLimitsY);

		virtual MSG run(void);
		virtual void ProcessMessage(MSG i_message);

    protected:

    private:

};

#endif /* LED_HPP_ */
