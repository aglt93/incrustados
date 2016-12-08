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
void Screen::ProcessMessage(MSG i_Message) {

	bool changeScreen = false;

	/* Se extrae la referencia del puntero al del periferico. */

	int* l_pMsgData = (int*) i_Message.data;

	int LastRacketLeftPosY  = RacketLeftPosY;
	int LastRacketRightPosY = RacketRightPosY;

	switch (i_Message.source) {

		case RACKET_LEFT_ID:
			LastRacketLeftPosY = RacketLeftPosY;
			RacketLeftPosY = *l_pMsgData;
			break;

		case RACKET_RIGHT_ID:
			LastRacketRightPosY = RacketRightPosY;
			RacketRightPosY = *l_pMsgData;
			break;

		case BALL_ID:
//			int LastBallRightPosY = BallRightPosY;
//			BallRightPosY = *l_pMsgData;
//			int LastBallRightPosY = BallRightPosY;
//			BallRightPosY = *l_pMsgData;
			break;

	}



	/* Draw Title, x-axis, gradation & labels */
	if (LastRacketLeftPosY != RacketLeftPosY || LastRacketRightPosY != RacketRightPosY) {

		changeScreen = true;
	}

	if (changeScreen) {
		/* Initializes graphics context */
		Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

		Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

		printFigure(Ball);
		printFigure(RacketLeft);
		printFigure(RacketRight);

	}

	RacketLeft.xMin = RACKET_LEFT_LIMIT_X_LEFT;
	RacketLeft.xMax = RACKET_LEFT_LIMIT_X_RIGHT;
	RacketLeft.yMin = RacketLeftPosY - 24;
	RacketLeft.yMax = RacketLeftPosY + 24;
	//
	RacketRight.xMin = RACKET_RIGHT_LIMIT_X_LEFT;
	RacketRight.xMax = RACKET_RIGHT_LIMIT_X_RIGHT;
	RacketRight.yMin = RacketRightPosY - 24;
	RacketRight.yMax = RacketRightPosY + 24;
	//
	Ball.xMin = 63 - 3;
	Ball.xMax = 63 + 3;
	Ball.yMin = 63 - 3;
	Ball.yMax = 63 + 3;

	if (changeScreen) {
		Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);

		printFigure(Ball);
		printFigure(RacketLeft);
		printFigure(RacketRight);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Esta funcion imprime en la pantalla la figura deseada en los puntos especificados por
 *  x1, y1, x2, y2.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
void Screen::printFigure(Graphics_Rectangle figure)
{

	Graphics_fillRectangle(&g_sContext, &figure);

}
//////////////////////////////////////////////////////////////////////////////////////////////
