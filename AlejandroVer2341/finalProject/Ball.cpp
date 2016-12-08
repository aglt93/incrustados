#include "Ball.hpp"
#include <driverlib.h>

#define LED_RED_PORT GPIO_PORT_P2
#define LED_RED_PIN GPIO_PIN4

Ball::Ball (int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount,
		int i_iPosX, int i_iPosY, int i_iSize, int i_iDirectionX, int i_iDirectionY,
		int* i_iLimitsX, int* i_iLimitsY)
{
    //ctor
	m_iTaskID = i_iTaskID;
	m_bPeriodicTask = i_bPeriodicTask;

	m_u64CurrentCount = 0;
	m_u64FinalCount = i_u64FinalCount;

	//
	m_iPosX = i_iPosX;
	m_iPosY = i_iPosY;
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

MSG Ball::run() {

	GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);

	///////////////////////////////////////////
	if(m_iDirectionX == NO_MOVE){
		m_iPosX = m_iPosX;
	}

	else if(m_iDirectionX == MOVE_LEFT){
		m_iPosX--;
	}

	else if(m_iDirectionX == MOVE_RIGHT){
		m_iPosX++;
	}
	///////////////////////////////////////////

	///////////////////////////////////////////
	if(m_iDirectionY == NO_MOVE){
		m_iPosY = m_iPosY;
	}

	else if(m_iDirectionY == MOVE_UP){
		m_iPosY--;
	}

	else if(m_iDirectionY == MOVE_DOWN){
		m_iPosY++;
	}
	///////////////////////////////////////////

	int BallPositions[2] = {m_iPosX,m_iPosY};
	int* pBallPositions = BallPositions;

	MSG nullMSG = {BALL_ID,SCREEN_ID,pBallPositions,0,1};
	return nullMSG;
}



void Ball::ProcessMessage(MSG i_Message){

	switch(i_Message.source){

		case RACKET_LEFT_ID:

			break;

		case RACKET_RIGHT_ID:
			break;

	}

}