/*
 * Racket.hpp
 *
 *  Created on: Dic, 2016
 *      Author: aglt93
 */

#ifndef RACKET_HPP_
#define RACKET_HPP_
#define __NOP __nop
#include "msp.h"
#include "GamePiece.hpp"


/* *****************************************************************************************
 * Clase Racket
 * *****************************************************************************************
 *
 * ***************
 * Descripción
 * ***************
 * Clase que modela una raqueta de ping pong. Hereda de la clase GamePiece ya que constituye una pieza
 * del juego.
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
 *
 * Racket(): Constructor por defecto de la clase.
 *
 * Racket(int i_iPosX,int i_iPosY, int i_iDirectionX, int i_iDirectionY,
 *		  int* i_iLimitsX, int* i_iLimitsY): Constructor de la clase, permite asignar los valores iniciales
 *		  de posiciones, direcciones y límites de la raqueta.
 *
 * void CheckLimitsY(): Método que permite verificar si la posición en Y de la raqueta excede o no
 * 						los límtes de la misma, en tal caso trunca el valor de la posición en Y en su
 * 						límite superior o inferior según corresponda.
 *
 * ******************************************************************************************
 */
class Racket : public GamePiece
{
    public:
		Racket(){};
		Racket(int i_iPosX,int i_iPosY, int i_iDirectionX, int i_iDirectionY,
				int* i_iLimitsX, int* i_iLimitsY);

		void CheckLimitsY();


    protected:

    private:

};

#endif /* RACKET_HPP_ */
