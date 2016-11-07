/*
 * main.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

// Port and pin values to the button.
#define BUTTON_PORT GPIO_PORT_P3
#define BUTTON_PIN GPIO_PIN5
//
// Port and pin values for Accelerometer.
#define ACCELEROMETER_PORT GPIO_PORT_P3
#define ACCELEROMETER_PIN GPIO_PIN5
//
#define RGB_BLUE_PORT GPIO_PORT_P5
#define RGB_BLUE_PIN GPIO_PIN6
#define RGB_GREEN_PORT GPIO_PORT_P2
#define RGB_GREEN_PIN GPIO_PIN4
#define LED_RED_PORT GPIO_PORT_P2
#define LED_RED_PIN GPIO_PIN6
#define RGB_RED_PORT GPIO_PORT_P2
#define RGB_RED_PIN GPIO_PIN6

//
#define NOT_PERIODIC_TASK false
#define PERIODIC_TASK true

// Define de task ID for each task.
#define SCHEDULER_ID 0
#define BUTTON_ID 1
#define LED1_ID 2
#define LED2_ID 3
#define ACCELEROMETER_ID 4


void Setup(void);



#endif /* MAIN_HPP_ */
