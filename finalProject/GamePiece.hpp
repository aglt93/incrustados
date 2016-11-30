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

#define UP 1

#define LIMITSX_SIZE 2
#define LIMITSY_SIZE 2

class GamePiece : public Task
{
    public:
		GamePiece();
		//GamePiece(int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount, int i_iPosX, int i_iPosY, int i_iSize, int i_iDirectionX, int i_iDirectionY);
		//GamePiece(int i_iTaskID, bool i_bPeriodicTask, int LED_PORT, int LED_PIN, uint64_t i_u64FinalCount);

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
/*
        void setLimitsX(int* i_iLimitsX);
        int* getLimitsX();

        void setLimitsY(int* i_iLimitsY);
        int* getLimitsY();
*/

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
