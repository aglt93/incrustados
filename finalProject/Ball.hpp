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
#include "task_ids.hpp"
#include "game_env.hpp"


class Ball : public GamePiece
{
    public:
		Ball(){};
		Ball(int i_iPosX, int i_iPosY, int i_iSize, int i_iDirectionX, int i_iDirectionY,
				int* i_iLimitsX, int* i_iLimitsY);

		void CheckLimitsX();
		void CheckLimitsY();
		int m_iBallStatus;

    protected:

    private:


};

#endif /* LED_HPP_ */
