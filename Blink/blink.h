/*
 * blink.h
 *
 *  Created on: Aug 17, 2016
 *      Author: eortiz
 */
// 1 SEC MAIN KVALLECI 20:08 13/9/16

#ifndef BLINK_H_
#define BLINK_H_

// Defines to set the lamp to 5W, 10W or 15W 
//#define LAMP_POWER5
//#define LAMP_POWER10
#define LAMP_POWER15

// Value to raise an interruption of the Timer32
// 
#define T32_SEC_COUNT 11720

// Port and pin values to the button.
#define BUTTON_PORT GPIO_PORT_P3
#define BUTTON_PIN GPIO_PIN5

// Port and pin values for the red, green and blue LEDs of the RGB LED in the MKII.
#define RGB_RED_PORT GPIO_PORT_P2
#define RGB_RED_PIN GPIO_PIN6
#define RGB_GREEN_PORT GPIO_PORT_P2
#define RGB_GREEN_PIN GPIO_PIN4
#define RGB_BLUE_PORT GPIO_PORT_P5
#define RGB_BLUE_PIN GPIO_PIN6
#define LED_RED_PORT GPIO_PORT_P1
#define LED_RED_PIN GPIO_PIN0
#define LED2_RED_PORT GPIO_PORT_P2
#define LED2_RED_PIN GPIO_PIN2
#define LED3_RED_PORT GPIO_PORT_P2
#define LED3_RED_PIN GPIO_PIN1
// Time value in seconds to turn off the lamp if there are no lamp_on conditions.
#define SEC_COUNT_LIMIT 10

// Lux threeshold to turn on the lamp.
#define LUX_LIMIT 50

// Tags for the i_iState of the SetLamp() method.
#define TURN_ON_LAMP 0
#define TURN_OFF_LAMP 1
#define TOGGLE_LAMP 2

// Tags to make the code clearer
#define SMART_MODE true
#define MANUAL_MODE false

// Port and pin values for microphone.
#define MIC_PORT GPIO_PORT_P4
#define MIC_PIN GPIO_PIN3

// 
#define MAX_SAMPLE 5
#define MAX_msSAMPLE 100

/*
 * Void method to configure all ports, pins
 * and interruptions.
 */
void Setup(void);

/*
 * void method to make the RGB led blink 3 times 
 * in the initialization.
 */
void Blink3(void);

bool secCount(void);

/*
 * void method to controll the turn on/off of the lamp
 * according to the light and sound conditions.
 */
void SmartMode(void);

/*
 * void method to turn on, turn off or toggle
 * the lamp according to the i_iState parameter.
 */
void SetLamp(int i_iState);

/*
 * bool method that returns the state of the lamp,
 * in order to know of the lamp is on or off.
 */
bool GetLampState (void);

/*
 * void method to controll the turn on/off of the
 * lamp when the user pushes the button.
 */
void ManualMode(void);

/*
 * float method to get the lux value sensed 
 * by the light sensor.
 */
float SenseLight(void);

/*
 * bool method 
 *
 *
 */
bool SenseSound(void);

/*
 * bool method
 *
 *
 */
bool msSampler(void);

/*
 * bool method
 *
 *
 */
bool secSamplerInit(void);

/*
 * bool method
 *
 *
 */
bool secSamplerMain(void);

/*
 * bool method
 *
 *
 */
bool OutlierDetector(void);

/*
 * void method to restart the timer32 counter.
 */
void ResetT32_1(void);


#endif /* BLINK_H_ */
