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
#include "u_logo_100_100.h"
}
//////////////////////////////////////////////////////////////////////////////////////////////

#include "Screen.hpp"
#include "Task.hpp"
#include "task_ids.hpp"
//////////////////////////////////////////////////////////////////////////////////////////////
#define FIRST_PIXEL 0
#define LAST_PIXEL 127
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


    Graphics_drawImage(&g_sContext, &tu_logo8BPP_COMP_RLE8, 14, 20); //Correct

	Graphics_drawStringCentered(&g_sContext, (int8_t *)"PING PONGDDED", AUTO_STRING_LENGTH, 63, 63, OPAQUE_TEXT);


	//
	RacketLeftPosX = 120;
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

	/* Se extrae la referencia del puntero al vector de datos del acelerometro. */
	int* l_pMsgData = (int*) i_Message.data;

	bool changeScreen = false;

	//
	int LastRacketLeftPosY = RacketLeftPosY;
	RacketLeftPosY = *l_pMsgData;
	RacketLeft.xMin = 128 - 4 - RacketLeftPosX;
	RacketLeft.xMax = 128 + 4 - RacketLeftPosX;
	RacketLeft.yMin = RacketLeftPosY - 24;
	RacketLeft.yMax = RacketLeftPosY + 24;

	//
	Ball.xMin = 63 - 3;
	Ball.xMax = 63 + 3;
	Ball.yMin = 63 - 3;
	Ball.yMax = 63 + 3;

//	racket2.xMin = x1;
//	racket2.xMax = x2;
//	racket2.yMin = y1;
//	racket2.yMax = y2;

	if (LastRacketLeftPosY != RacketLeftPosY) {
		changeScreen = true;
	}

	if (changeScreen) {

		/* Initializes graphics context */
		Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);


		/* Draw Title, x-axis, gradation & labels */
		Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);
		Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

		Graphics_clearDisplay(&g_sContext);

		printFigure(Ball);
		printFigure(RacketLeft);
		//printFigure(RacketRight);
	}


	/* Se ejecuta la funcion que actualiza las lineas en la pantalla. */

//	printFigure(0,l_iScreenValue,*(l_pDataTask));

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

//////////////////////////////////////////////////////////////////////////////////////////////
/* Los datos del acelerometro se traducen a coordenadas en el eje y de la pantalla.
 * por medio de una ecuacion lineal con offset.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
int Screen::ADCtoScreenValueConv(int i_iADCvalue){

	int l_iServoValue;

	/* Se transforma el valor del ADC a lineas de acuerdo a la ecuacion de una recta */
	l_iServoValue = (float) (i_iADCvalue-5000)/51.8;

	return l_iServoValue;

}
//////////////////////////////////////////////////////////////////////////////////////////////
