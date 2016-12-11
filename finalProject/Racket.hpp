/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef RACKET_HPP_
#define RACKET_HPP_
#define __NOP __nop
#include "msp.h"
#include "GamePiece.hpp"

#define SCREEN_CENTER 63


class Racket : public GamePiece
{
    public:
		Racket(){};
		Racket(int i_iPosX,int i_iPosY, int i_iSize, int i_iDirectionX, int i_iDirectionY,
				int* i_iLimitsX, int* i_iLimitsY);

		void CheckLimitsY();


    protected:

    private:

};

#endif /* LED_HPP_ */
