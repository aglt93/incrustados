//////////////////////////////////////////////////////////////////////////////////////////////
#define __NOP __nop
//////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
{
	#include <driverlib.h>
	#include <grlib.h>
	#include "Crystalfontz128x128_ST7735.h"
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
	Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_DOWN);

	/* Initializes graphics context */
	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

	/* Draw Title, x-axis, gradation & labels */
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SANDY_BROWN);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_STEEL_BLUE);
	GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
	Graphics_clearDisplay(&g_sContext);

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
    int l_iScreenValue = ADCtoScreenValueConv(*(l_pDataTask+1));

	/* Initializes graphics context */
	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);


	/* Draw Title, x-axis, gradation & labels */
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SANDY_BROWN);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_STEEL_BLUE);

	/* Se ejecuta la funcion que actualiza las lineas en la pantalla. */
	printScreen(0,l_iScreenValue,*(l_pDataTask));

}
//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Esta funcion actualiza las lineas de la patanlla conforme el acelerometro detecta cambios.
 * Los datos del acelerometro se traducen a coordenadas en el eje y de la pantalla.
 */
//////////////////////////////////////////////////////////////////////////////////////////////
void Screen::printScreen(uint8_t l_u16Initial, uint8_t l_u16Final, uint16_t l_u8UpOrDown)
{

	/* 0 0 0 grad All GRAPHICS_COLOR_SANDY_BROWN
	   0 64 45 grad Half GRAPHICS_COLOR_SANDY_BROWN Half GRAPHICS_COLOR_STEEL_BLUE
	   0 127 90 grad All GRAPHICS_COLOR_STEEL_BLUE
	*/

	uint8_t l_u8Ys = 0;

	/* Se utilizan los datos del acelerometro en el eje Y para determinar si
	 * el dispositivo fue volteado hacia arriba.
	 */

	if (l_u8UpOrDown > 11300) {

		/* Determina la configuracion actual de la pantalla.
		 * Se actualiza solo si es diferente de la actualizacion.
		 */

		if (Lcd_Orientation != LCD_ORIENTATION_UP){

			Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

		}
		else

			l_u8Ys = 0;

			for ( l_u8Ys=FIRST_PIXEL; l_u8Ys<LAST_PIXEL+1; l_u8Ys++ ){

				if(l_u8Ys<l_u16Final+1){

					/*
					 * Imprime la tierra del horizonte
					 */
					Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SANDY_BROWN );
					Graphics_drawLineH(&g_sContext, 0, 127, l_u8Ys);

				}
				else

					/*
					 * Imprime las lineas pendientes del cielo del horizonte.
					 */
					Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_STEEL_BLUE );
					Graphics_drawLineH(&g_sContext, 0, 127, l_u8Ys);


			}
	}

	/* Se utilizan los datos del acelerometro en el eje Y para determinar si
	 * el dispositivo fue volteado hacia abajo.
	 */

	else if (l_u8UpOrDown < 11300) {

		if (Lcd_Orientation != LCD_ORIENTATION_DOWN){

			Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_DOWN);

		}
		else

			l_u8Ys = 0;

			for ( l_u8Ys=FIRST_PIXEL; l_u8Ys<LAST_PIXEL+1; l_u8Ys++ ){

				if(l_u8Ys<l_u16Final+1){

					/*
					 * Imprime la tierra del horizonte
					 */
					Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SANDY_BROWN );
					Graphics_drawLineH(&g_sContext, 0, 127, l_u8Ys);

				}

				else

					/*
					 * Imprime las lineas pendientes del cielo del horizonte.
					 */
					Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_STEEL_BLUE );
					Graphics_drawLineH(&g_sContext, 0, 127, l_u8Ys);

			}
	}

	else

		l_u8Ys = 0;

		for ( l_u8Ys=FIRST_PIXEL; l_u8Ys<LAST_PIXEL+1; l_u8Ys++ ){

			if(l_u8Ys<l_u16Final+1){

				/*
				 * Imprime la tierra del horizonte
				 */
				Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SANDY_BROWN );
				Graphics_drawLineH(&g_sContext, 0, 127, l_u8Ys);

			}
			else

				/*
				 * Imprime las lineas pendientes del cielo del horizonte.
				*/
				Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_STEEL_BLUE );
				Graphics_drawLineH(&g_sContext, 0, 127, l_u8Ys);

		}

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
