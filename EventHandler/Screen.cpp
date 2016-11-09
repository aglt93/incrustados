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


/* Graphic library context */
Graphics_Context g_sContext;

Screen::Screen(int Screen_PORT,int Screen_PIN,uint64_t i_u64FinalCount)
{
    //ctor
	Graphics_Context g_sContext;

	/* Initializes display */
	Crystalfontz128x128_Init();

	/* Set default screen orientation */
	Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

	/* Initializes graphics context */
	Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);

	/* Draw Title, x-axis, gradation & labels */
	Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SANDY_BROWN);
	Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_STEEL_BLUE);
	GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
	Graphics_clearDisplay(&g_sContext);

	int ys = 0;
	for ( ys=64; ys<128; ys++ ){
		Graphics_drawLineH(&g_sContext, 0, 127, ys);
	}

}

uint8_t Screen::run(void)
{
    //#########################
    // Blink code
    //#########################

	GPIO_toggleOutputOnPin(m_iScreenPort,m_iScreenPin);

    return(NO_ERR);
}