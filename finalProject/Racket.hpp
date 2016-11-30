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


class Racket : public GamePiece
{
    public:
		Racket(int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount, int i_iPosX,
				int i_iPosY, int i_iSize, int i_iDirectionX, int i_iDirectionY,
				int* i_iLimitsX, int* i_iLimitsY);

		//GamePiece(int i_iTaskID, bool i_bPeriodicTask, int LED_PORT, int LED_PIN, uint64_t i_u64FinalCount);

		virtual uint8_t run(void);
		virtual void ProcessMessage(MSG i_message);

    protected:

    private:

};

#endif /* LED_HPP_ */