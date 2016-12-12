#include "Ball.hpp"
#include <driverlib.h>
#include <stdlib.h>
#define LED_RED_PORT GPIO_PORT_P2
#define LED_RED_PIN GPIO_PIN4

Ball::Ball (int i_iPosX, int i_iPosY, int i_iSize, int i_iDirectionX, int i_iDirectionY,
		int* i_iLimitsX, int* i_iLimitsY)
{
	//
	m_iPosX = i_iPosX;
	m_iPosY = i_iPosY;

	m_iBallStatus = NO_HIT;

	// Para que sea diferente!
	m_iLastPosX = m_iPosX-1;
	m_iLastPosY = m_iPosY-1;

	//
	m_bChangeY = true;
	m_bChangeX = true;

	//
	m_iSize = i_iSize;
	m_iDirectionX = i_iDirectionX;
	m_iDirectionY = i_iDirectionY;


	for (int i = 0; i<LIMITSX_SIZE;i++) {

		m_iLimitsX[i] = i_iLimitsX[i];

	}

	for (int i = 0; i<LIMITSY_SIZE;i++) {

		m_iLimitsY[i] = i_iLimitsY[i];

	}

}


void Ball::CheckLimitsX() {


	switch(m_iBallStatus) {

		case HIT_LEFT_RACKET:
			m_iPosX = m_iLimitsX[0] + 1;
			m_iDirectionX = MOVE_RIGHT;
			break;

		case HIT_RIGHT_RACKET:
			m_iPosX = m_iLimitsX[1] - 1;
			m_iDirectionX = MOVE_LEFT;
			break;

		case HIT_LEFT_WALL:
			m_iPosX = m_iLimitsX[0] + RACKET_LENGTH/RACKET_THICKNESS;
			m_iDirectionX = MOVE_RIGHT;
			break;

		case HIT_RIGHT_WALL:
			m_iPosX = m_iLimitsX[1] - RACKET_LENGTH/RACKET_THICKNESS;
			m_iDirectionX = MOVE_LEFT;
			break;

		case NO_HIT:
			m_iBallStatus = NO_HIT;
			m_iDirectionX = m_iDirectionX;
			break;
	}

}

int roll(int min, int max)
{
   // x is in [0,1[
   double x = rand()/static_cast<double>(RAND_MAX+1);

   // [0,1[ * (max - min) + min is in [min,max[
   int that = min + static_cast<int>( x * (max - min) );

   return that;
}

void Ball::CheckLimitsY() {

	switch(m_iBallStatus) {

		case HIT_TOP_WALL:
			m_iPosY = m_iLimitsY[0];
			m_iDirectionY = MOVE_DOWN;
			break;

		case HIT_BOTTOM_WALL:
			m_iPosY = m_iLimitsY[1];
			m_iDirectionY = MOVE_UP;
			break;

		case HIT_LEFT_WALL:
			m_iPosY = SCREEN_CENTER;
			m_iDirectionY = roll(MOVE_DOWN,MOVE_UP);
			break;

		case HIT_RIGHT_WALL:
			m_iPosY = SCREEN_CENTER;
			m_iDirectionY = roll(MOVE_DOWN,MOVE_UP);
			break;

		case NO_HIT:
			m_iBallStatus = NO_HIT;
			m_iDirectionY = m_iDirectionY;
			break;
	}
}



