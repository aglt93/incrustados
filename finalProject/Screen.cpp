//////////////////////////////////////////////////////////////////////////////////////////////
#define __NOP __nop
//////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
{
#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <string.h>
//#include "u_logo_100_100.h"
}

//////////////////////////////////////////////////////////////////////////////////////////////
#include <string>     // std::string, std::to_string
#include <sstream>
#include <string>

#include "Screen.hpp"
#include "Task.hpp"
#include "task_ids.hpp"
#include "game_env.hpp"
#include "GameLogic.hpp"
#include "Racket.hpp"
#include "Ball.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////


#define RGB_RED_PORT GPIO_PORT_P2
#define RGB_RED_PIN GPIO_PIN6
#define RGB_BLUE_PORT GPIO_PORT_P5
#define RGB_BLUE_PIN GPIO_PIN6
#define RGB_GREEN_PORT GPIO_PORT_P2
#define RGB_GREEN_PIN GPIO_PIN4


//////////////////////////////////////////////////////////////////////////////////////////////
/* Graphic library context */
Graphics_Context g_sContext;


//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Constructor de la pantalla para configuraciones iniciales.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
Screen::Screen(int i_iTaskID, bool i_bPeriodicTask)
{
	m_iTaskID = i_iTaskID;
	m_bPeriodicTask = i_bPeriodicTask;

	//ctor
	Graphics_Context g_sContext;

    /* Initializes display */
	Crystalfontz128x128_Init();

	/* Set default screen orientation */
	Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

	/* Initializes graphics context */
	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);


	/* Draw Title, x-axis, gradation & labels */
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);


	Graphics_clearDisplay(&g_sContext);

//
//    Graphics_drawImage(&g_sContext, &tu_logo8BPP_COMP_RLE8, 14, 20); //Correct
//
//	Graphics_drawStringCentered(&g_sContext, (int8_t *)"PING PONGDDED", AUTO_STRING_LENGTH, 63, 63, OPAQUE_TEXT);

//
//	RacketLeftPosX = 8;
//	RacketRightPosX = 120;

}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
MSG Screen::run(void)
{
	MSG nullMSG = {-1,-1,0,0,1};
	return nullMSG;

}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Esta funcion de referencia los punteros para obtener los valores del acelerometro
 * y ser procedados por los metodos para imprimir la pantalla.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
MSG Screen::ProcessMessage(MSG i_Message) {

	GameLogic* MainLogic = (GameLogic*) i_Message.data;

//	switch(MainLogic->m_iGameMode) {
//
//		case GAME_RUNNING_SCREEN:

			getFigureChange(i_Message);
			printPongTable();
			MSG nullMsg = printFigure();
			printPongScore(i_Message);
//			return nullMsg;

//			break;
//
//		case PLAYER_1_WON_SCREEN:
//		case PLAYER_2_WON_SCREEN:
//			printPongWinner(i_Message);
			return nullMsg;

//			break;
//
//	}

}
//////////////////////////////////////////////////////////////////////////////////////////////


void Screen::getFigureChange(MSG i_Message){

//

	ScreenFigureChanges.RacketLeftChange = false;
	ScreenFigureChanges.RacketRightChange = false;
	ScreenFigureChanges.BallChange = false;

	
	switch(i_Message.source) {

		case LOGIC_ID:

			GameLogic* MainLogic = (GameLogic*) i_Message.data;
			//
			RacketLeftPosY = MainLogic->RacketLeft.m_iPosY;
			ScreenFigureChanges.RacketLeftChange = MainLogic->RacketLeft.m_bChangeY;
			MainLogic->RacketLeft.m_bChangeY = false;

			//
			RacketRightPosY = MainLogic->RacketRight.m_iPosY;
			ScreenFigureChanges.RacketRightChange = MainLogic->RacketRight.m_bChangeY;
			MainLogic->RacketRight.m_bChangeY = false;

			//
			BallPosX = MainLogic->MainBall.m_iPosX;
			BallPosY = MainLogic->MainBall.m_iPosY;
			ScreenFigureChanges.BallChange = (MainLogic->MainBall.m_bChangeX ||
					MainLogic->MainBall.m_bChangeY);
			MainLogic->MainBall.m_bChangeX = false;
			MainLogic->MainBall.m_bChangeY = false;
			break;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Esta funcion imprime en la pantalla la figura deseada en los puntos especificados por
 *  x1, y1, x2, y2.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
void Screen::printPongTable()
{
	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);

	Net.xMin = SCREEN_CENTER-1;
	Net.xMax = SCREEN_CENTER+1;
	Net.yMin = SCREEN_HEIGHT-SCREEN_HEIGHT;
	Net.yMax = SCREEN_HEIGHT;

	Graphics_fillRectangle(&g_sContext, &Net);

	Net.xMin = SCREEN_WIDTH-SCREEN_WIDTH;
	Net.xMax = SCREEN_WIDTH;
	Net.yMin = SCREEN_HEIGHT-SCREEN_HEIGHT;
	Net.yMax = SCREEN_HEIGHT;

	Graphics_drawRectangle(&g_sContext, &Net);

}

//////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Esta funcion imprime en la pantalla la figura deseada en los puntos especificados por
 *  x1, y1, x2, y2.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
MSG Screen::printFigure()
{

	if(ScreenFigureChanges.RacketLeftChange
			|| ScreenFigureChanges.RacketRightChange
			|| ScreenFigureChanges.BallChange) {

		/* Initializes graphics context */
		Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

		//
		if (ScreenFigureChanges.RacketLeftChange) {

			// Se limpia la figura anterior.
			Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
			Graphics_fillRectangle(&g_sContext, &RacketLeft);

			// Se calcula la nueva posición de la figura.
			RacketLeft.xMin = RACKET_LEFT_LIMIT_X_LEFT;
			RacketLeft.xMax = RACKET_LEFT_LIMIT_X_RIGHT;
			RacketLeft.yMin = RacketLeftPosY - RACKET_LENGTH/2;
			RacketLeft.yMax = RacketLeftPosY + RACKET_LENGTH/2;

			// Se dibuja la nueva figura.
			Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
			Graphics_fillRectangle(&g_sContext, &RacketLeft);


		}

		//
		if (ScreenFigureChanges.RacketRightChange) {

			// Se limpia la figura anterior.
			Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
			Graphics_fillRectangle(&g_sContext, &RacketRight);

			// Se calcula la nueva posición de la figura.
			RacketRight.xMin = RACKET_RIGHT_LIMIT_X_LEFT;
			RacketRight.xMax = RACKET_RIGHT_LIMIT_X_RIGHT;
			RacketRight.yMin = RacketRightPosY - RACKET_LENGTH/2;
			RacketRight.yMax = RacketRightPosY + RACKET_LENGTH/2;

			// Se dibuja la nueva figura.
			Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
			Graphics_fillRectangle(&g_sContext, &RacketRight);


		}

		//
		if (ScreenFigureChanges.BallChange) {

			// Se limpia la figura anterior.
			Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
			Graphics_fillRectangle(&g_sContext, &ScreenBall);

			// Se calcula la nueva posición de la figura.
			ScreenBall.xMin = BallPosX - BALL_THICKNESS/2;
			ScreenBall.xMax = BallPosX + BALL_THICKNESS/2;
			ScreenBall.yMin = BallPosY - BALL_THICKNESS/2;
			ScreenBall.yMax = BallPosY + BALL_THICKNESS/2;

			// Se dibuja la nueva figura.
			Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);
			Graphics_fillRectangle(&g_sContext, &ScreenBall);


		}
	}

	MSG nullMsg = {-1,-1,0,0,1};
	return nullMsg;
}

//////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Esta funcion imprime en la pantalla la figura deseada en los puntos especificados por
 *  x1, y1, x2, y2.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
void Screen::printPongScore(MSG i_Message)
{
	GameLogic* MainLogic = (GameLogic*) i_Message.data;

	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
	//
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    char RacketLeftScore[8];
    sprintf(RacketLeftScore, "P1:%2i", MainLogic->m_iRacketLeftScore);
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
	Graphics_drawStringCentered(&g_sContext,
			(int8_t *) RacketLeftScore, AUTO_STRING_LENGTH,
			SCORE_POS_P1_X, SCORE_POS_P1_Y, OPAQUE_TEXT);

	char RacketRightScore[8];
    sprintf(RacketRightScore, "P2:%2i", MainLogic->m_iRacketRightScore);
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
	Graphics_drawStringCentered(&g_sContext,
				(int8_t *) RacketRightScore, AUTO_STRING_LENGTH,
				SCORE_POS_P2_X, SCORE_POS_P2_Y, OPAQUE_TEXT);
}

//////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Esta funcion imprime en la pantalla la figura deseada en los puntos especificados por
 *  x1, y1, x2, y2.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
void Screen::printPongWinner(MSG i_Message)
{



	GameLogic* MainLogic = (GameLogic*) i_Message.data;

    if (MainLogic->m_iGameMode==PLAYER_1_WON_SCREEN) {
    	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
    	//
    	Graphics_clearDisplay(&g_sContext);

    	GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);

    	Graphics_drawStringCentered(&g_sContext, (int8_t *)"P1 AK7", AUTO_STRING_LENGTH, SCREEN_CENTER, SCREEN_CENTER, OPAQUE_TEXT);

    }

    else if (MainLogic->m_iGameMode==PLAYER_2_WON_SCREEN) {
    	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
    	//
    	Graphics_clearDisplay(&g_sContext);

    	GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    	Graphics_drawStringCentered(&g_sContext, (int8_t *)"P2 AK7", AUTO_STRING_LENGTH, SCREEN_CENTER, SCREEN_CENTER, OPAQUE_TEXT);

    }

}

