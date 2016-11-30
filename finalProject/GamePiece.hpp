/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef GAMEPIECE_HPP_
#define GAMEPIECE_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"

#define NO_MOVE 0
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 1
#define MOVE_RIGHT 2

#define LIMITSX_SIZE 2
#define LIMITSY_SIZE 2

class GamePiece : public Task
{
    public:
		GamePiece();

		virtual uint8_t run(void);
		virtual void ProcessMessage(MSG i_message);

        void setPosX(int i_iPosX);
        int getPosX();

        void setPosY(int i_iPosY);
        int getPosY();

        void setSize(int i_iSize);
        int getSize();

        void setDirectionX(int i_iDirectionX);
        int getDirectionX();

        void setDirectionY(int i_iDirectionY);
        int getDirectionY();

    protected:
        int m_iPosX;
		int m_iPosY;
		int m_iSize;
		int m_iDirectionX;
		int m_iDirectionY;
		int m_iLimitsX [LIMITSX_SIZE];
		int m_iLimitsY [LIMITSY_SIZE];

    private:


};

#endif /* LED_HPP_ */
