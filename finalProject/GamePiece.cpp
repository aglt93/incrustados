#include "GamePiece.hpp"
#include <driverlib.h>


GamePiece::GamePiece(){


}


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








void GamePiece::CheckChangeY() {

	if(m_iLastPosY != m_iPosY) {
		m_iLastPosY = m_iPosY;
		m_bChangeY = true;
	}

	else {
		m_bChangeY = false;
	}
}

void GamePiece::CheckChangeX() {

	if(m_iLastPosX != m_iPosX) {
		m_iLastPosX = m_iPosX;
		m_bChangeX = true;
	}

	else {
		m_bChangeX = false;
	}
}
