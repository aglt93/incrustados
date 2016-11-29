/*
 * main.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

// Port and pin values to the button.
#define BUTTON_DOWN_PORT GPIO_PORT_P3
#define BUTTON_DOWN_PIN GPIO_PIN5

#define BUTTON_UP_PORT GPIO_PORT_P5
#define BUTTON_UP_PIN GPIO_PIN1

//
// Port and pin values for Accelerometer.
#define ACCELEROMETER_PORT GPIO_PORT_P3
#define ACCELEROMETER_PIN GPIO_PIN5
//

#define RGB_RED_PORT GPIO_PORT_P2
#define RGB_RED_PIN GPIO_PIN6
#define RGB_BLUE_PORT GPIO_PORT_P5
#define RGB_BLUE_PIN GPIO_PIN6
#define RGB_GREEN_PORT GPIO_PORT_P2
#define RGB_GREEN_PIN GPIO_PIN4

#define LED_RED_PORT GPIO_PORT_P1
#define LED_RED_PIN GPIO_PIN0
#define LED_BLUE_PORT GPIO_PORT_P2
#define LED_BLUE_PIN GPIO_PIN2
#define LED_GREEN_PORT GPIO_PORT_P2
#define LED_GREEN_PIN GPIO_PIN1


//
#define NOT_PERIODIC_TASK false
#define PERIODIC_TASK true

//
#define SERVO_PORT GPIO_PORT_P2
#define SERVO_PIN GPIO_PIN5


void Setup(void);



#endif /* MAIN_HPP_ */
