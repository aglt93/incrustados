#include "Racket.hpp"
#include <driverlib.h>
#include "task_ids.hpp"


#define LED_RED_PORT GPIO_PORT_P2
#define LED_RED_PIN GPIO_PIN4


Racket::Racket (int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount,
		int i_iPosX, int i_iPosY, int i_iSize, int i_iDirectionX, int i_iDirectionY,
		int* i_iLimitsX, int* i_iLimitsY) {

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

MSG Racket::run() {

	//GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);

//	MSG nullMSG = {-1,-1,0,0,1};
//	return nullMSG;

	MSG ChangeScreen = {m_iTaskID,SCREEN_ID,&m_iPosY,0,1};
	return ChangeScreen;
}



void Racket::ProcessMessage(MSG i_Message){

	switch (i_Message.source){

		case BUTTON_UP_ISR_ID:
			m_iPosY -= m_iSize;
			m_iDirectionY = MOVE_UP;
			break;

		case BUTTON_DOWN_ISR_ID:
			m_iPosY += m_iSize;
			m_iDirectionY = MOVE_DOWN;
			break;

		case ADC_ISR_ID:
			int* l_iADCvalue = (int*) i_Message.data;
			m_iPosY = (float) ((-79* (*l_iADCvalue) + 1678900)/16300);
			break;

	}

	CheckLimitsY();


}

void Racket::CheckLimitsY(){


	if(m_iPosY > m_iLimitsY[0]) {

		m_iPosY = m_iLimitsY[0];

	}

	if(m_iPosY < m_iLimitsY[1]){

		m_iPosY = m_iLimitsY[1];
	}
}
