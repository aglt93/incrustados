/*
 * Ball.hpp
 *
 *  Created on: Dic, 2016
 *      Author: aglt93
 */

#ifndef BALL_HPP_
#define BALL_HPP_
#define __NOP __nop
#include "msp.h"
#include "GamePiece.hpp"
#include "task_ids.hpp"
#include "game_env.hpp"

/* *****************************************************************************************
 * Clase Ball
 * *****************************************************************************************
 *
 * ***************
 * Descripción
 * ***************
 * Clase que modela una bola de ping pong. Hereda de la clase GamePiece ya que constituye una pieza
 * del juego.
 *
 *
 * ***************
 * Atributos
 * ***************
 * Únicamente los heredados de la clase GamePiece.
 *
 *
 * ***************
 * Métodos
 * ***************
 * Ball(): Constructor por defecto.
 *
 * Ball(int i_iPosX, int i_iPosY, int i_iDirectionX, int i_iDirectionY,
 *				int* i_iLimitsX, int* i_iLimitsY): Constructor de la clase, permite asignar
 *				los valores iniciales de posiciones, direcciones y límites de la bola.
 *
 * void CheckLimitsX(): Método que permite verificar si la posición en X de la bola excede o no
 * 						los límtes de la misma, en tal caso trunca el valor de la posición en Y en su
 * 						límite izquierdo o derecho según corresponda.
 *
 * void CheckLimitsY(): Método que permite verificar si la posición en Y de la bola excede o no
 * 						los límtes de la misma, en tal caso trunca el valor de la posición en Y en su
 * 						límite superior o inferior según corresponda.
 *
 * ******************************************************************************************
 */
class Ball : public GamePiece
{
    public:
		Ball(){};
		Ball(int i_iPosX, int i_iPosY, int i_iDirectionX, int i_iDirectionY,
				int* i_iLimitsX, int* i_iLimitsY);

		void CheckLimitsX();
		void CheckLimitsY();

    protected:

    private:


};

#endif /* LED_HPP_ */
