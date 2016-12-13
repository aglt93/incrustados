#include "Racket.hpp"
#include <driverlib.h>
#include "task_ids.hpp"


Racket::Racket (int i_iPosX, int i_iPosY, int i_iDirectionX, int i_iDirectionY,
		int* i_iLimitsX, int* i_iLimitsY) {

	// Se inicializan las posiciones del centro de la raqueta.
	m_iPosX = i_iPosX;
	m_iPosY = i_iPosY;

	// Se configura la posición anterior en un valor de las posiciones iniciales - 1
	// para que sean diferentes inicialmente.
	m_iLastPosX = m_iPosX-1;
	m_iLastPosY = m_iPosY-1;

	// Los valores se acaban de inicializar y por tanto ocurrieron cambios en X y Y.
	m_bChangeY = true;
	m_bChangeX = true;

	// Las direcciones se inicializan.
	m_iDirectionX = i_iDirectionX;
	m_iDirectionY = i_iDirectionY;


	// Se inicializan los límites.
	for (int i = 0; i<LIMITSX_SIZE;i++) {

		m_iLimitsX[i] = i_iLimitsX[i];

	}

	for (int i = 0; i<LIMITSY_SIZE;i++) {

		m_iLimitsY[i] = i_iLimitsY[i];

	}

}


void Racket::CheckLimitsY(){


	if(m_iPosY < m_iLimitsY[0]) {// UP

		m_iPosY = m_iLimitsY[0];

	}

	else if(m_iPosY > m_iLimitsY[1]){// DOWN

		m_iPosY = m_iLimitsY[1];
	}
}
