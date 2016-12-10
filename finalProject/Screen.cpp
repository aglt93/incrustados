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
//#include "u_logo_100_100.h"
}
//////////////////////////////////////////////////////////////////////////////////////////////

#include "Screen.hpp"
#include "Task.hpp"
#include "task_ids.hpp"
#include "game_env.hpp"
#include "Racket.hpp"
#include "Ball.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////

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

	/* Se extrae la referencia del puntero al del periferico. */

	//

	figure_change = getFigureChange(i_Message);

	MSG figure_msg = printFigure(figure_change);
//
	printPongScore();

	printPongWinner();
	return figure_msg;
}
//////////////////////////////////////////////////////////////////////////////////////////////


Figure_Change Screen::getFigureChange(MSG i_Message){

//

	figure_change.racket_left_change = false;
	figure_change.racket_right_change = false;
	figure_change.ball_change = false;

	
	switch(i_Message.source) {
	//switch (TaskFromMsg->m_iTaskID) {

		case LOGIC_ID:
			break;

		case RACKET_LEFT_ID:
			Racket* RacketLeft = (Racket*) i_Message.data;
			RacketLeftPosY = RacketLeft->m_iPosY;
			figure_change.racket_left_change = true;
			break;

		case RACKET_RIGHT_ID:
			Racket* RacketRight = (Racket*) i_Message.data;
			RacketRightPosY = RacketRight->m_iPosY;
			figure_change.racket_right_change = true;
			break;

		case BALL_ID:
			Ball* MainBall = (Ball*) i_Message.data;
			BallPosX = MainBall->BallPositions[0];
			BallPosY = MainBall->BallPositions[1];
			figure_change.ball_change = true;
			break;

	}

	return figure_change;
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

	net.xMin = SCREEN_CENTER-1;
	net.xMax = SCREEN_CENTER+1;
	net.yMin = SCREEN_HEIGHT-SCREEN_HEIGHT;
	net.yMax = SCREEN_HEIGHT;

	Graphics_fillRectangle(&g_sContext, &net);

	net.xMin = SCREEN_WIDTH-SCREEN_WIDTH;
	net.xMax = SCREEN_WIDTH;
	net.yMin = SCREEN_HEIGHT-SCREEN_HEIGHT;
	net.yMax = SCREEN_HEIGHT;

	Graphics_drawRectangle(&g_sContext, &net);

}

//////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Esta funcion imprime en la pantalla la figura deseada en los puntos especificados por
 *  x1, y1, x2, y2.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
MSG Screen::printFigure(Figure_Change figure_change )
{

	if( figure_change.racket_left_change || figure_change.racket_right_change  || figure_change.ball_change ) {

		/* Initializes graphics context */
		Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

		Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

		//
		if (figure_change.racket_left_change) {

			// Se limpia la figura anterior.
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
		if (figure_change.racket_right_change) {

			// Se limpia la figura anterior.
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
		if (figure_change.ball_change) {

			// Se limpia la figura anterior.
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
void Screen::printPongScore()
{
	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
	//
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
	Graphics_drawStringCentered(&g_sContext, (int8_t *)"P1:", AUTO_STRING_LENGTH, SCORE_POS_P1_X, SCORE_POS_P1_Y, TRANSPARENT_TEXT);
	//
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
	Graphics_drawStringCentered(&g_sContext, (int8_t *)"P2:", AUTO_STRING_LENGTH, SCORE_POS_P2_X, SCORE_POS_P2_Y, OPAQUE_TEXT);
}

//////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Esta funcion imprime en la pantalla la figura deseada en los puntos especificados por
 *  x1, y1, x2, y2.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
void Screen::printPongWinner()
{
	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
	//
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

    int p1_score = 1;
    int p2_score = 5;

    if (p1_score == WINNER_SCORE) {

    	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    	Graphics_drawStringCentered(&g_sContext, (int8_t *)"P1 AK7", AUTO_STRING_LENGTH, SCREEN_CENTER, SCREEN_CENTER, OPAQUE_TEXT);

    }
    if (p2_score == WINNER_SCORE) {

    	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    	Graphics_drawStringCentered(&g_sContext, (int8_t *)"P2 AK7", AUTO_STRING_LENGTH, SCREEN_CENTER, SCREEN_CENTER, OPAQUE_TEXT);

    }
	printPongTable();

}

