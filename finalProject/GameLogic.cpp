#include "GameLogic.hpp"
#include <driverlib.h>
#include "task_ids.hpp"

#define RGB_RED_PORT GPIO_PORT_P2
#define RGB_RED_PIN GPIO_PIN6
#define RGB_BLUE_PORT GPIO_PORT_P5
#define RGB_BLUE_PIN GPIO_PIN6
#define RGB_GREEN_PORT GPIO_PORT_P2
#define RGB_GREEN_PIN GPIO_PIN4

GameLogic::GameLogic (int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount) {

    //ctor
	m_iTaskID = i_iTaskID;
	m_bPeriodicTask = i_bPeriodicTask;

	m_u64CurrentCount = 0;
	m_u64FinalCount = i_u64FinalCount;

}

MSG GameLogic::run(void) {

	//GPIO_toggleOutputOnPin(LED_RED_PORT,LED_RED_PIN);
	MSG nullMsg = {-1,-1,0,0,1};
	return nullMsg;
}

MSG GameLogic::ProcessMessage(MSG i_Message){

	MSG changeScreen = {-1,-1,0,0,1};

	switch(i_Message.source){

		case RACKET_LEFT_ID:
			changeScreen.source = i_Message.source;
			changeScreen.destination = SCREEN_ID;
			changeScreen.data = i_Message.data;
			changeScreen.currentCount = 0;
			changeScreen.finalCount = 1;
			break;

		case RACKET_RIGHT_ID:
			changeScreen.source = i_Message.source;
			changeScreen.destination = SCREEN_ID;
			changeScreen.data = i_Message.data;
			changeScreen.currentCount = 0;
			changeScreen.finalCount = 1;
			break;

		case BALL_ID:
			changeScreen.source = i_Message.source;
			changeScreen.destination = SCREEN_ID;
			changeScreen.data = i_Message.data;
			changeScreen.currentCount = 0;
			changeScreen.finalCount = 1;
			break;

	}


	return changeScreen;
}
