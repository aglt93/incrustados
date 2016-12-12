#include "Ball.hpp"
#include <driverlib.h>

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
			// hit by left racket?
			m_iPosX = m_iLimitsX[0];
			m_iDirectionX = MOVE_RIGHT;
			break;

		case HIT_RIGHT_RACKET:
			// hit by right racket?
			m_iPosX = m_iLimitsX[1];
			m_iDirectionX = MOVE_LEFT;
			break;

		case HIT_LEFT_WALL:
			// hit by left racket?
			m_iPosX = m_iLimitsX[0] + RACKET_LENGTH/2 ;
			m_iDirectionX = MOVE_RIGHT;
			break;

		case HIT_RIGHT_WALL:
			// hit by right racket?
			m_iPosX = m_iLimitsX[1] - RACKET_LENGTH/2 ;
			m_iDirectionX = MOVE_LEFT;
			break;

		case NO_HIT:
			// hit by right racket?
			m_iBallStatus = NO_HIT;
			m_iDirectionX = m_iDirectionX;
			break;
	}

}

void Ball::CheckLimitsY() {

//    // hit top wall?
//	if (m_iPosY < m_iLimitsY[0]) {// UP
//
//		m_iPosY = m_iLimitsY[0];
//		m_iDirectionY = MOVE_DOWN;
//
//	}
//    // hit bottom wall?
//	else if (m_iPosY > m_iLimitsY[1]) {// DOWN
//
//		m_iPosY = m_iLimitsY[1];
//		m_iDirectionY = MOVE_UP;
//	}


	switch(m_iBallStatus) {

		case HIT_TOP_WALL:
			// hit by left racket?
			m_iPosY = m_iLimitsY[0];
			m_iDirectionY = MOVE_DOWN;
			break;

		case HIT_BOTTOM_WALL:
			// hit by right racket?
			m_iPosY = m_iLimitsY[1];
			m_iDirectionY = MOVE_UP;
			break;

		case HIT_LEFT_WALL:
			m_iPosY = SCREEN_CENTER;
			m_iDirectionY = NO_MOVE;
			break;

		case HIT_RIGHT_WALL:
			m_iPosY = SCREEN_CENTER;
			m_iDirectionY = NO_MOVE;
			break;

		case NO_HIT:
			// hit by right racket?
			m_iBallStatus = NO_HIT;
			m_iDirectionY = m_iDirectionY;
			break;
	}
}



