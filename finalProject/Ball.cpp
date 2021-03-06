#include "Ball.hpp"
#include <driverlib.h>
#include <stdlib.h>
#define LED_RED_PORT GPIO_PORT_P2
#define LED_RED_PIN GPIO_PIN4

Ball::Ball (int i_iPosX, int i_iPosY, int i_iDirectionX, int i_iDirectionY,
		int* i_iLimitsX, int* i_iLimitsY)
{
	//
	m_iPosX = i_iPosX;
	m_iPosY = i_iPosY;

	// Para que sea diferente!
	m_iLastPosX = m_iPosX-1;
	m_iLastPosY = m_iPosY-1;

	//
	m_bChangeY = true;
	m_bChangeX = true;

	//
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

	if (m_iPosX < m_iLimitsX[0]) {// LEFT

		m_iPosX = m_iLimitsX[0];

	}

	else if (m_iPosX > m_iLimitsX[1]) {// RIGHT

		m_iPosX = m_iLimitsX[1];

	}

}

void Ball::CheckLimitsY() {

	if (m_iPosY < m_iLimitsY[0]) {// UP

		m_iPosY = m_iLimitsY[0];

	}

	else if (m_iPosY > m_iLimitsY[1]) {// DOWN

		m_iPosY = m_iLimitsY[1];

	}
}



