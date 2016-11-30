#include "Racket.hpp"
#include <driverlib.h>

Racket::Racket (int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount,
		int i_iPosX, int i_iPosY, int i_iSize, int i_iDirectionX, int i_iDirectionY,
		int* i_iLimitsX, int* i_iLimitsY)
{
    //ctor
	m_iTaskID = i_iTaskID;
	m_bPeriodicTask = i_bPeriodicTask;

	m_u64CurrentCount = 0;
	m_u64FinalCount = i_u64FinalCount;

	//
	m_iPosX = UP;//i_iPosX;
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

uint8_t Racket::run() {

	return NO_ERR;
}



void Racket::ProcessMessage(MSG i_Message){

}