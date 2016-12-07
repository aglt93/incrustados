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
int8_t racket1_center = 63;

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


}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Screen::run(void)
{
    return(NO_ERR);
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
	int* l_pDataTask = (int*) i_Message.data;

	/* Se convierte el valor del acelerometro en lineas para la pantalla. */
//    int l_iScreenValue = ADCtoScreenValueConv(*(l_pDataTask+1));

	/* Initializes graphics context */
	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);


	/* Draw Title, x-axis, gradation & labels */
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

	Graphics_Rectangle ball, racket1, racket2;

	int8_t x1, y1, x2, y2;

//	int8_t racket1_center = 63;
	int8_t racket2_center = 63;

	int8_t racket1_move = 0;
	int8_t racket2_move = 0;

	racket1_move = *(l_pDataTask);

	x1 = 120 - 4;
	x2 = 120 + 4;
//	y1 = 63 + 4*8 -24 = 71
//	y2 = 63 + 4*8 +24 = 119
	y1 = racket1_center + racket1_move*8 - 24;
	y2 = racket1_center + racket1_move*8 + 24;



	ball.xMin = 63 - 3;
	ball.xMax = 63 + 3;
	ball.yMin = 63 - 3;
	ball.yMax = 63 + 3;

	racket1.xMin = 128 - x1;
	racket1.xMax = 128 - x2;
	racket1.yMin = y1;
	racket1.yMax = y2;

//	racket2.xMin = x1;
//	racket2.xMax = x2;
//	racket2.yMin = y1;
//	racket2.yMax = y2;

	Graphics_clearDisplay(&g_sContext);

	printFigure(ball);
	printFigure(racket1);
//	printFigure(racket2);

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
//int Screen::ADCtoScreenValueConv(int i_iADCvalue){
//
//	int l_iServoValue;
//
//	/* Se transforma el valor del ADC a lineas de acuerdo a la ecuacion de una recta */
//	l_iServoValue = (float) (i_iADCvalue-5000)/51.8;
//
//	return l_iServoValue;
//
//}
//////////////////////////////////////////////////////////////////////////////////////////////
