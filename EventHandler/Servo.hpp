/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef SERVO_HPP_
#define SERVO_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"


/* *****************************************************************************************
 * Clase Scheduler
 * *****************************************************************************************
 *
 * ***************
 * Descripción
 * ***************
 * La clase hereda de la clase Task y se encarga de configurar el PWM en el puerto 2.5 para que
 * al ser conectado un servo con un período de 20ms y en un ciclo de trabajo entre 0.5ms y 2.5ms
 * gire entre 90 y -90 grados respectivamente. Esto se logra mediante la configuración del
 * Timer A en modo de salida RESET_SET para generar la señal de salida PWM. Hace uso del modo
 * TIMER_A_CAPTURECOMPARE_REGISTER_2, donde este modo se encuentra conectado directamente con
 * el puerto 2.5.
 *
 * ***************
 * Atributos
 * ***************
 * m_iServoPort: Puerto donde se desea configurar el servo.
 * m_iServoPin: Pin donde se desea configurar el servo.
 *
 *
 * ***************
 * Métodos
 * ***************
 * Servo: Constructor de la clase, se encarga de realizar la configuración inicial del servo.
 * 		  Este valor corresponde al de 0 grados siendo el valor medio del rango posible.
 *
 * run:
 *
 * ProcessMessage: Se encarga de cambiar la configuración del servo con un valor enviado. Este
 * 				   valor es convertido al valor que cambia el ciclo de trabajo del PWM para
 * 				   que el servo se mueve de una posición a otra, según el valor convertido
 * 				   por el ADC.
 *
 * ADCtoServoValueConv: Método que retorna se encarga de convertir el valor enviado por
 * 						el ADC a un valor que refleja el cambio del ADC en el servo, según el
 * 						ciclo de trabajo. El valor de retorno puede variar entre 1600 y 7600.
 * 						Y estos valores corresponden a los valores que son cargados al Timer A
 * 						para que genere el ciclo de trabajo entre 0.5ms y 2.5ms.
 * ******************************************************************************************
 */
class Servo : public Task
{
    public:
        Servo(int i_iTaskID, bool i_bPeriodicTask, int servo_port, int servo_pin);
        virtual uint8_t run(void);
        virtual void ProcessMessage(MSG i_Message);
        int ADCtoServoValueConv(int i_iADCvalue);
    protected:
    private:
        int m_iServoPort;
        int m_iServoPin;
};

#endif /* LED_HPP_ */
