/*
 * blink.h
 *
 *  Created on: Aug 17, 2016
 *      Author: eortiz
 */
// 1 SEC MAIN KVALLECI 20:08 13/9/16

#ifndef BLINK_H_
#define BLINK_H_

#define LAMP_POWER15

#define T32_SEC_COUNT 11720
//
#define BUTTON_PORT GPIO_PORT_P3
#define BUTTON_PIN GPIO_PIN5
//
#define RGB_RED_PORT GPIO_PORT_P2
#define RGB_RED_PIN GPIO_PIN6
#define RGB_GREEN_PORT GPIO_PORT_P2
#define RGB_GREEN_PIN GPIO_PIN4
#define RGB_BLUE_PORT GPIO_PORT_P5
#define RGB_BLUE_PIN GPIO_PIN6
#define LED_RED_PORT GPIO_PORT_P1
#define LED_RED_PIN GPIO_PIN0
//
#define SEC_COUNT_LIMIT 10
#define LUX_LIMIT 50
//
#define TURN_ON_LAMP 0
#define TURN_OFF_LAMP 1
#define TOGGLE_LAMP 2
//
#define SMART_MODE true
#define MANUAL_MODE false
//
#define MIC_PORT GPIO_PORT_P4
#define MIC_PIN GPIO_PIN3
//
#define MAX_SAMPLE 5

void Setup(void);
void Blink3(void);
bool secCount(void);
void SmartMode(void);
void SetLamp(int i_iState);
bool GetLampState (void);
void ManualMode(void);
float SenseLight(void);
bool SenseSound(void);
void ResetT32_1(void);

#endif /* BLINK_H_ */
