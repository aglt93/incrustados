#include "GameLogic.hpp"
#include <driverlib.h>
#include "task_ids.hpp"

GameLogic::GameLogic (int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount) {

    //ctor
	m_iTaskID = i_iTaskID;
	m_bPeriodicTask = i_bPeriodicTask;

	m_u64CurrentCount = 0;
	m_u64FinalCount = i_u64FinalCount;

}

MSG GameLogic::run(void) {

	MSG nullMsg = {-1,-1,0,0,1};
	return nullMsg;
}

void GameLogic::ProcessMessage(MSG i_Message){

	switch(i_Message.source){

		case RACKET_LEFT_ID:
			break;

		case RACKET_RIGHT_ID:
			break;

		case BALL_ID:
			break;

	}

}
