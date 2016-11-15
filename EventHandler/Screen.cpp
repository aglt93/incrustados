#define __NOP __nop

extern "C"
{
	#include <driverlib.h>
	#include <grlib.h>
	#include "Crystalfontz128x128_ST7735.h"
}
#include <stdio.h>
#include "Screen.hpp"
#include "Task.hpp"
#include "task_ids.hpp"

#define INITIAL_COND_F 0
#define FINAL_COND_F 64
#define FIRST_PIXEL 0
#define LAST_PIXEL 127

/* Graphic library context */
Graphics_Context g_sContext;

Screen::Screen(int i_iTaskID, bool i_bPeriodicTask)
{
	m_iTaskID = i_iTaskID;
	m_bPeriodicTask = i_bPeriodicTask;

	m_u64CurrentCount = 0;
	m_u64FinalCount = 1000;


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

uint8_t Screen::run(void)
{
    return(NO_ERR);
}


void Screen::ProcessMessage(MSG i_Message) {

	int* l_pDataTask = (int*) i_Message.data;

	m_u64CurrentCount = 0;
	m_u64FinalCount = 1000;

    int16_t l_iScreenValue;
//    int16_t l_iPreScreenValue;
//
//    l_iPreScreenValue = l_iScreenValue;

    l_iScreenValue = ADCtoScreenValueConv(*(l_pDataTask+1));

	/* Initializes graphics context */
	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

	/* Draw Title, x-axis, gradation & labels */
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SANDY_BROWN);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_STEEL_BLUE);

	printScreen(0,l_iScreenValue,*(l_pDataTask));


}


/*
 * Method to update screen by accelerometer data.
 */
void Screen::printScreen(uint8_t l_u16Initial, uint8_t l_u16Final, uint16_t l_u8UpOrDown)
{
	/* 0 0 0 grad All GRAPHICS_COLOR_SANDY_BROWN
	 0 64 45 grad Half GRAPHICS_COLOR_SANDY_BROWN Half GRAPHICS_COLOR_STEEL_BLUE
	 0 127 90 grad All GRAPHICS_COLOR_STEEL_BLUE
	*/
	uint8_t ys = 0;

	if (l_u8UpOrDown > 11300) {
		if (Lcd_Orientation != LCD_ORIENTATION_UP){
			Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
		}
		else
			 ys = 0;

			for ( ys=l_u16Initial; ys<LAST_PIXEL+1; ys++ ){

				if(ys<l_u16Final+1){
					Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SANDY_BROWN );
					Graphics_drawLineH(&g_sContext, 0, 127, ys);
				}
				else
					Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_STEEL_BLUE );
					Graphics_drawLineH(&g_sContext, 0, 127, ys);

			}
	}

	else if (l_u8UpOrDown < 11300) {
		if (Lcd_Orientation != LCD_ORIENTATION_DOWN){
			Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_DOWN);
		}
		else
			ys = 0;

			for ( ys=l_u16Initial; ys<LAST_PIXEL+1; ys++ ){

				if(ys<l_u16Final+1){
					Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SANDY_BROWN );
					Graphics_drawLineH(&g_sContext, 0, 127, ys);
				}
				else
					Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_STEEL_BLUE );
					Graphics_drawLineH(&g_sContext, 0, 127, ys);

			}
	}

	else

		ys = 0;

		for ( ys=l_u16Initial; ys<LAST_PIXEL+1; ys++ ){

			if(ys<l_u16Final+1){
				Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SANDY_BROWN );
				Graphics_drawLineH(&g_sContext, 0, 127, ys);
			}
			else
				Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_STEEL_BLUE );
				Graphics_drawLineH(&g_sContext, 0, 127, ys);

		}

}

// Translate Accelerometer data to screen lines.
int Screen::ADCtoScreenValueConv(int i_iADCvalue){

	int16_t l_iServoValue;

	l_iServoValue = (float) (i_iADCvalue-5000)/51.8;

	return l_iServoValue;

}
