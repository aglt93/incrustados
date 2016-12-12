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

	m_iRacketLeftScore = 0;
	m_iRacketRightScore = 0;
	m_iGameMode = 0;

//	m_iBallStatus = NO_HIT;

	int RacketLeftLimitsX[2] = {RACKET_LEFT_LIMIT_X_LEFT,RACKET_LEFT_LIMIT_X_RIGHT};
	int *pRacketLeftLimitsX = RacketLeftLimitsX;

	int RacketLeftLimitsY[2] = {RACKET_LEFT_LIMIT_Y_UP,RACKET_LEFT_LIMIT_Y_DOWN};
	int *pRacketLeftLimitsY = RacketLeftLimitsY;

	int RacketRightLimitsX[2] = {RACKET_RIGHT_LIMIT_X_LEFT,RACKET_RIGHT_LIMIT_X_RIGHT};
	int *pRacketRightLimitsX = RacketRightLimitsX;

	int RacketRightLimitsY[2] = {RACKET_RIGHT_LIMIT_Y_UP,RACKET_RIGHT_LIMIT_Y_DOWN};
	int *pRacketRightLimitsY = RacketRightLimitsY;

	int BallLimitsX[2] = {BALL_LIMIT_X_LEFT,BALL_LIMIT_X_RIGHT};
	int *pBallLimitsX = BallLimitsX;

	int BallLimitsY[2] = {BALL_LIMIT_Y_UP,BALL_LIMIT_Y_DOWN};
	int *pBallLimitsY = BallLimitsY;
	//

	RacketLeft = Racket(RACKET_LEFT_POS_X,RACKET_LEFT_POS_Y,8,NO_MOVE,NO_MOVE,
			pRacketLeftLimitsX,pRacketLeftLimitsY);
	RacketRight = Racket(RACKET_RIGHT_POS_X,RACKET_RIGHT_POS_Y,8,NO_MOVE,NO_MOVE,
			pRacketRightLimitsX,pRacketRightLimitsY);
	MainBall = Ball(BALL_INIT_POS_X,BALL_INIT_POS_Y,3,MOVE_RIGHT,MOVE_UP,pBallLimitsX,pBallLimitsY);

}

void BallControlX(Racket* i_RacketLeft, Racket* i_RacketRight, Ball* i_Ball) {

	///////////////////////////////////////////
	if(i_Ball->m_iDirectionX == NO_MOVE){
		i_Ball->m_iPosX = i_Ball->m_iPosX;
	}

	else if(i_Ball->m_iDirectionX == MOVE_LEFT){
		i_Ball->m_iPosX--;
	}

	else if(i_Ball->m_iDirectionX == MOVE_RIGHT){
		i_Ball->m_iPosX++;
	}



	///////////////////////////////////////////
	i_Ball->CheckLimitsX();
	i_Ball->CheckChangeX();
}


void BallControlY(Racket* i_RacketLeft, Racket* i_RacketRight, Ball* i_Ball) {

	///////////////////////////////////////////
	if(i_Ball->m_iDirectionY == NO_MOVE){
		i_Ball->m_iPosY = i_Ball->m_iPosY;
	}

	else if(i_Ball->m_iDirectionY == MOVE_UP){
		i_Ball->m_iPosY--;
	}

	else if(i_Ball->m_iDirectionY == MOVE_DOWN){
		i_Ball->m_iPosY++;
	}
	///////////////////////////////////////////
	i_Ball->CheckLimitsY();
	i_Ball->CheckChangeY();
}


void ResetRacketLeft(Racket* i_RacketLeft){
	i_RacketLeft->m_iPosX = RACKET_LEFT_POS_X;
	i_RacketLeft->m_iPosY = RACKET_LEFT_POS_Y;
	i_RacketLeft->CheckChangeX();
	i_RacketLeft->CheckChangeY();
}


void ResetRacketRight(Racket* i_RacketRight) {

	i_RacketRight->m_iPosX = RACKET_RIGHT_POS_X;
	i_RacketRight->m_iPosY = RACKET_RIGHT_POS_Y;
	i_RacketRight->CheckChangeX();
	i_RacketRight->CheckChangeY();

}

void GameLogic::scoreControl(Racket* i_RacketLeft, Racket* i_RacketRight, Ball* i_Ball) {

	int RacketLeftLowerRange = i_RacketLeft->m_iPosY - RACKET_LENGTH/2;
	int RacketLeftUpperRange = i_RacketLeft->m_iPosY + RACKET_LENGTH/2;

	int RacketRightLowerRange = i_RacketRight->m_iPosY - RACKET_LENGTH/2;
	int RacketRightUpperRange = i_RacketRight->m_iPosY + RACKET_LENGTH/2;

	//	Collision with left racket.
	if(i_Ball->m_iPosX == i_Ball->m_iLimitsX[0]
			&& i_Ball->m_iPosY < RacketLeftUpperRange
			&& i_Ball->m_iPosY > RacketLeftLowerRange){

		i_Ball->m_iPosX++;
		i_Ball->m_iBallStatus = HIT_LEFT_RACKET;

	}

	//	Collision with right racket.
	else if(i_Ball->m_iPosX == i_Ball->m_iLimitsX[1]
			&& i_Ball->m_iPosY < RacketRightUpperRange
			&& i_Ball->m_iPosY > RacketRightLowerRange){

		i_Ball->m_iPosX--;
		i_Ball->m_iBallStatus = HIT_RIGHT_RACKET;

	}

	//	Collision with left wall.
	else if(i_Ball->m_iPosX == i_Ball->m_iLimitsX[0]
			&& (i_Ball->m_iPosY > RacketLeftUpperRange
			|| i_Ball->m_iPosY < RacketLeftLowerRange)){

		i_Ball->m_iBallStatus = HIT_LEFT_WALL;

		ResetRacketLeft(i_RacketLeft);
		ResetRacketRight(i_RacketRight);

		m_iRacketRightScore++;

	}
	//	Collision with right wall.
	else if(i_Ball->m_iPosX == i_Ball->m_iLimitsX[1]
			&& (i_Ball->m_iPosY > RacketLeftUpperRange
			|| i_Ball->m_iPosY < RacketRightLowerRange)){

		i_Ball->m_iBallStatus = HIT_RIGHT_WALL;

		ResetRacketLeft(i_RacketLeft);
		ResetRacketRight(i_RacketRight);
		//ResetBall();

		m_iRacketLeftScore++;

	}

	//	Collision with top wall.
	else if(i_Ball->m_iPosY < i_Ball->m_iLimitsY[0]){

		i_Ball->m_iBallStatus = HIT_TOP_WALL;

	}

	//	Collision with buttom wall.
	else if(i_Ball->m_iPosY > i_Ball->m_iLimitsY[1]){

		i_Ball->m_iBallStatus = HIT_BOTTOM_WALL;

	}

	//
	else{
		i_Ball->m_iBallStatus =	NO_HIT;
	}

}

void GameLogic::winnerControl() {

	///////////////////////////////////////////
	if(m_iRacketLeftScore == WINNER_SCORE){
		m_iGameMode = PLAYER_1_WON_SCREEN;
		m_iRacketLeftScore = 0;
	}
	else if(m_iRacketRightScore == WINNER_SCORE){
		m_iGameMode = PLAYER_2_WON_SCREEN;
		m_iRacketRightScore = 0;
	}
	else{
		m_iGameMode = GAME_RUNNING_SCREEN;
	}


}

MSG GameLogic::run(void) {

	//Ball Control
	BallControlX(&RacketLeft, &RacketRight, &MainBall);
	BallControlY(&RacketLeft, &RacketRight, &MainBall);
	scoreControl (&RacketLeft, &RacketRight, &MainBall);
	winnerControl();
	// Screen Control
	MSG msgToScreen = {-1,-1,0,0,1};

	if(RacketLeft.m_bChangeY || RacketRight.m_bChangeY || MainBall.m_bChangeX || MainBall.m_bChangeY) {

		msgToScreen.source = m_iTaskID;
		msgToScreen.destination = SCREEN_ID;
		msgToScreen.data = this;
		msgToScreen.currentCount = 0;
		msgToScreen.finalCount = 1;
	}

	// Score control


	// Winner Control


	return msgToScreen;
}

MSG GameLogic::ProcessMessage(MSG i_Message){

	switch(i_Message.source){

		case ADC_ISR_ID:
			int* l_iADCvalue = (int*) i_Message.data;
			RacketLeft.m_iPosY = (float) ((-79* (*l_iADCvalue) + 1678900)/16300);
			RacketLeft.CheckLimitsY();
			RacketLeft.CheckChangeY();
			break;

		case BUTTON_UP_ISR_ID:
			RacketRight.m_iPosY -= RacketRight.m_iSize;
			RacketRight.m_iDirectionY = MOVE_UP;
			RacketRight.CheckLimitsY();
			RacketRight.CheckChangeY();
			break;

		case BUTTON_DOWN_ISR_ID:
			RacketRight.m_iPosY += RacketRight.m_iSize;
			RacketRight.m_iDirectionY = MOVE_DOWN;
			RacketRight.CheckLimitsY();
			RacketRight.CheckChangeY();
			break;


	}

	MSG nullMsg = {-1,-1,0,0,1};
	return nullMsg;
}
