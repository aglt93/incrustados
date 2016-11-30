#include "GamePiece.hpp"
#include <driverlib.h>
/*
GamePiece::GamePiece(int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount, int i_iPosX, int i_iPosY, int i_iSize, int i_iDirectionX, int i_iDirectionY)
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


}
*/

GamePiece::GamePiece(){


}
/*
GamePiece::GamePiece(int i_iTaskID, bool i_bPeriodicTask, int LED_PORT,int LED_PIN, uint64_t i_u64FinalCount)
{
    //ctor
	m_iTaskID = i_iTaskID;
	m_iLedPort = LED_PORT;
	m_iLedPin = LED_PIN;
	m_bPeriodicTask = i_bPeriodicTask;

	GPIO_setAsOutputPin(m_iLedPort,m_iLedPin);
	GPIO_setOutputLowOnPin(m_iLedPort,m_iLedPin);

	m_u64CurrentCount = 0;
	m_u64FinalCount = i_u64FinalCount;

}
*/





void GamePiece::setPosX(int i_iPosX){

	m_iPosX = i_iPosX;

}

int GamePiece::getPosX(){

	return m_iPosX;
}





void GamePiece::setPosY(int i_iPosY){

	m_iPosY = i_iPosY;

}

int GamePiece::getPosY(){

	return m_iPosY;

}






void GamePiece::setSize(int i_iSize){

	m_iSize = i_iSize;

}

int GamePiece::getSize(){

	return m_iSize;
}







void GamePiece::setDirectionX(int i_iDirectionX){

	m_iDirectionX = i_iDirectionX;
}

int GamePiece::getDirectionX(){

	return m_iDirectionX;
}






void GamePiece::setDirectionY(int i_iDirectionY){

	m_iDirectionY = i_iDirectionY;
}

int GamePiece::getDirectionY(){

	return m_iDirectionY;
}


/*

void GamePiece::setLimitsX(int* i_iLimitsX){

	m_iLimitsX = i_iLimitsX;

}


int* GamePiece::getLimitsX() {

	return &m_iLimitsX;
}



void GamePiece::setLimitsY(int* i_iLimitsY){

	m_iLimitsY = i_iLimitsY;

}


int* GamePiece::getLimitsY() {

	return &m_iLimitsY;
}
*/
