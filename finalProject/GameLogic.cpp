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

	m_iBallStatus = NO_HIT;
	m_iFsmState = GAME_INIT_STATE;

	m_iWinnerCounterScreen = INIT_WINNER_SCREEN_COUNTER;

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
	MainBall = Ball(BALL_INIT_LEFT_POS_X,BALL_INIT_LEFT_POS_Y,3,NO_MOVE,NO_MOVE,pBallLimitsX,pBallLimitsY);

}

void BallMoveX(Ball* i_Ball) {

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


void BallMoveY(Ball* i_Ball) {

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


void BallControlX(Ball* i_Ball,int m_iBallStatus){

	switch(m_iBallStatus) {

		case HIT_LEFT_RACKET:
			i_Ball->m_iPosX = i_Ball->m_iLimitsX[0] + 1;
			i_Ball->m_iDirectionX = MOVE_RIGHT;
			break;

		case HIT_RIGHT_RACKET:
			i_Ball->m_iPosX = i_Ball->m_iLimitsX[1] - 1;
			i_Ball->m_iDirectionX = MOVE_LEFT;
			break;

		case HIT_LEFT_WALL:
			i_Ball->m_iPosX = i_Ball->m_iLimitsX[0] + 1;
			i_Ball->m_iDirectionX = NO_MOVE;
			break;

		case HIT_RIGHT_WALL:
			i_Ball->m_iPosX = i_Ball->m_iLimitsX[1] - 1;
			i_Ball->m_iDirectionX = NO_MOVE;
			break;

		case NO_HIT:
			i_Ball->m_iDirectionX = i_Ball->m_iDirectionX;
			break;
	}

}

void BallControlY(Ball* i_Ball, int i_iBallStatus){

	switch(i_iBallStatus) {

		case HIT_TOP_WALL:
			i_Ball->m_iPosY = i_Ball->m_iLimitsY[0];
			i_Ball->m_iDirectionY = MOVE_DOWN;
			break;

		case HIT_BOTTOM_WALL:
			i_Ball->m_iPosY = i_Ball->m_iLimitsY[1];
			i_Ball->m_iDirectionY = MOVE_UP;
			break;

		case HIT_LEFT_WALL:
			i_Ball->m_iPosY = SCREEN_CENTER;
			i_Ball->m_iDirectionY = NO_MOVE;
			break;

		case HIT_RIGHT_WALL:
			i_Ball->m_iPosY = SCREEN_CENTER;
			i_Ball->m_iDirectionY = NO_MOVE;
			break;

		case NO_HIT:
			i_Ball->m_iDirectionY = i_Ball->m_iDirectionY;
			break;
	}


}

void BallFastnessControl(GameLogic* Game, Ball* i_Ball, int i_iBallStatus){

	if (i_iBallStatus == HIT_LEFT_RACKET || i_iBallStatus == HIT_RIGHT_RACKET){

		if (Game->GetTaskFinalCount() > 1){

			int value = Game->GetTaskFinalCount() - 1;
			Game->SetTaskFinalCount(value);
		}

		else {Game->SetTaskFinalCount(1);}

	}

	else if (i_iBallStatus == HIT_LEFT_WALL || i_iBallStatus == HIT_RIGHT_WALL) {

		Game->SetTaskFinalCount(LOGIC_PERIOD);

	}

}


void RacketLeftControl(Racket* i_RacketLeft, int i_iBallStatus){

	if(i_iBallStatus == HIT_LEFT_WALL || i_iBallStatus == HIT_RIGHT_WALL) {

		i_RacketLeft->m_iPosX = RACKET_LEFT_POS_X;
		i_RacketLeft->m_iPosY = RACKET_LEFT_POS_Y;

		i_RacketLeft->CheckChangeX();
		i_RacketLeft->CheckChangeY();

		i_RacketLeft->m_iDirectionX = NO_MOVE;
		i_RacketLeft->m_iDirectionY = NO_MOVE;
	}
}


void RacketRightControl(Racket* i_RacketRight, int i_iBallStatus) {

	if(i_iBallStatus == HIT_LEFT_WALL || i_iBallStatus == HIT_RIGHT_WALL) {

		i_RacketRight->m_iPosX = RACKET_RIGHT_POS_X;
		i_RacketRight->m_iPosY = RACKET_RIGHT_POS_Y;

		i_RacketRight->CheckChangeX();
		i_RacketRight->CheckChangeY();

		i_RacketRight->m_iDirectionX = NO_MOVE;
		i_RacketRight->m_iDirectionY = NO_MOVE;
	}

}


void GameLogic::GameControl() {

	int RacketLeftLowerRange = RacketLeft.m_iPosY + RACKET_LENGTH/2 + 3;
	int RacketLeftUpperRange = RacketLeft.m_iPosY - RACKET_LENGTH/2 - 3;

	int RacketRightLowerRange = RacketRight.m_iPosY + RACKET_LENGTH/2 + 3;
	int RacketRightUpperRange = RacketRight.m_iPosY - RACKET_LENGTH/2 - 3;

	//	Collision with left racket.
	if(MainBall.m_iPosX <= MainBall.m_iLimitsX[0]
			&& RacketLeftUpperRange <= MainBall.m_iPosY
			&& RacketLeftLowerRange >= MainBall.m_iPosY){

		m_iBallStatus = HIT_LEFT_RACKET;

	}

	//	Collision with right racket.
	else if(MainBall.m_iPosX >= MainBall.m_iLimitsX[1]
			&& RacketRightUpperRange <= MainBall.m_iPosY
			&& RacketRightLowerRange >= MainBall.m_iPosY){

		m_iBallStatus = HIT_RIGHT_RACKET;

	}

	//	Collision with left wall.
	else if(MainBall.m_iPosX <= MainBall.m_iLimitsX[0]
			&& (RacketLeftUpperRange >= MainBall.m_iPosY
			||  RacketLeftLowerRange <= MainBall.m_iPosY)){

		m_iBallStatus = HIT_LEFT_WALL;

	}
	//	Collision with right wall.
	else if(MainBall.m_iPosX >= MainBall.m_iLimitsX[1]
			&& (RacketRightUpperRange >= MainBall.m_iPosY
			||  RacketRightLowerRange <= MainBall.m_iPosY)){

		m_iBallStatus = HIT_RIGHT_WALL;

	}

	//	Collision with top wall.
	else if(MainBall.m_iPosY <= MainBall.m_iLimitsY[0]){

		m_iBallStatus = HIT_TOP_WALL;

	}

	//	Collision with buttom wall.
	else if(MainBall.m_iPosY >= MainBall.m_iLimitsY[1]){

		m_iBallStatus = HIT_BOTTOM_WALL;

	}

	//
	else{

		m_iBallStatus = NO_HIT;

	}

}

void GameLogic::ScoreControl() {

	if (m_iBallStatus == HIT_LEFT_RACKET) {

		m_iFsmState = GAME_RUNNING_STATE;

	}

	else if (m_iBallStatus == HIT_RIGHT_RACKET) {

		m_iFsmState = GAME_RUNNING_STATE;

	}

	else if (m_iBallStatus == HIT_TOP_WALL) {

		m_iFsmState = GAME_RUNNING_STATE;

	}

	else if (m_iBallStatus == HIT_BOTTOM_WALL) {

		m_iFsmState = GAME_RUNNING_STATE;

	}

	else if (m_iBallStatus == HIT_LEFT_WALL){

		m_iRacketRightScore++;
		WinnerControl();
		if (m_iGameMode == PLAYER_2_WON_SCREEN){
			m_iFsmState = WINNER_STATE;
		}
		else{
			m_iFsmState = GAME_INIT_STATE;
		}

	}

	else if (m_iBallStatus == HIT_RIGHT_WALL) {

		m_iRacketLeftScore++;
		WinnerControl();
		if (m_iGameMode == PLAYER_1_WON_SCREEN){
			m_iFsmState = WINNER_STATE;
		}
		else{
			m_iFsmState = GAME_INIT_STATE;
		}
	}

	else if (m_iBallStatus == NO_HIT) {
		m_iFsmState = GAME_RUNNING_STATE;
	}

}

void GameLogic::WinnerControl() {

	///////////////////////////////////////////
	if(m_iRacketLeftScore == WINNER_SCORE){
		m_iGameMode = PLAYER_1_WON_SCREEN;
	}
	else if(m_iRacketRightScore == WINNER_SCORE){
		m_iGameMode = PLAYER_2_WON_SCREEN;
	}
	else{
		m_iGameMode = NO_WINNER_YET;
	}


}

void GameLogic::FirstMoveControl(){

	if(MainBall.m_iDirectionX == NO_MOVE && MainBall.m_iDirectionY == NO_MOVE){

		if(RacketLeft.m_iDirectionY != NO_MOVE
				&& MainBall.m_iPosX == BALL_INIT_LEFT_POS_X){

			MainBall.m_iDirectionX = MOVE_RIGHT;
			MainBall.m_iDirectionY = RacketLeft.m_iDirectionY;
		}

		else if (RacketRight.m_iDirectionY != NO_MOVE
				&& MainBall.m_iPosX == BALL_INIT_RIGHT_POS_X){

			MainBall.m_iDirectionX = MOVE_LEFT;
			MainBall.m_iDirectionY = RacketRight.m_iDirectionY;
		}

	}
}

MSG GameLogic::run(void) {

	switch(m_iFsmState) {

		case GAME_INIT_STATE:

			m_iFsmState = GAME_RUNNING_STATE;
			FirstMoveControl();

			break;

		case GAME_RUNNING_STATE:

			m_iFsmState = GAME_RUNNING_STATE;
			FirstMoveControl();
			//Ball Movement
			BallMoveX(&MainBall);
			BallMoveY(&MainBall);
			// Game Control
			GameControl();
			// Ball Control
			BallControlX(&MainBall,m_iBallStatus);
			BallControlY(&MainBall,m_iBallStatus);
			BallFastnessControl(this,&MainBall,m_iBallStatus);
			// Racket Control
			RacketLeftControl(&RacketLeft,m_iBallStatus);
			RacketRightControl(&RacketRight,m_iBallStatus);
			// Score control
			ScoreControl();

			break;

		case WINNER_STATE:

			m_iRacketLeftScore = 0;
			m_iRacketRightScore = 0;
			m_iWinnerCounterScreen++;
			if (m_iWinnerCounterScreen >= FINAL_WINNER_SCREEN_COUNTER){

				m_iFsmState = GAME_INIT_STATE;
				m_iWinnerCounterScreen = 0;

			}

			break;

	}

	// Screen Control
	MSG msgToScreen = {-1,-1,0,0,1};

	if(RacketLeft.m_bChangeY || RacketRight.m_bChangeY || MainBall.m_bChangeX || MainBall.m_bChangeY) {

		msgToScreen.source = m_iTaskID;
		msgToScreen.destination = SCREEN_ID;
		msgToScreen.data = this;
		msgToScreen.currentCount = 0;
		msgToScreen.finalCount = 1;
	}

	return msgToScreen;

}

MSG GameLogic::ProcessMessage(MSG i_Message){

	switch(i_Message.source){

		case ADC_ISR_ID:
			int* l_iADCvalue = (int*) i_Message.data;

			int LastRacketLeftPosY = RacketLeft.m_iPosY;
			RacketLeft.m_iPosY = (float) ((-79* (*l_iADCvalue) + 1678900)/16300);

			if(LastRacketLeftPosY > RacketLeft.m_iPosY){
				RacketLeft.m_iDirectionY = MOVE_UP;
			}

			else if (LastRacketLeftPosY < RacketLeft.m_iPosY){
				RacketLeft.m_iDirectionY = MOVE_DOWN;

			}

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
