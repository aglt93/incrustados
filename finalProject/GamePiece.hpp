/*
 * GamePiece.hpp
 *
 *  Created on: Dic, 2016
 *      Author: aglt93
 */

#ifndef GAMEPIECE_HPP_
#define GAMEPIECE_HPP_
#define __NOP __nop
#include "msp.h"


//////////////////////////////////////////////////////////////////////////////////////////////
#define NO_MOVE 0
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 1
#define MOVE_RIGHT 2

#define LIMITSX_SIZE 2
#define LIMITSY_SIZE 2
//////////////////////////////////////////////////////////////////////////////////////////////


/* *****************************************************************************************
 * Clase GamePiece
 * *****************************************************************************************
 *
 * ***************
 * Descripción
 * ***************
 * Clase que modela la pieza de un videojuego. Esto implica modelar sus posiciones en X y Y,
 * las direcciones de su movimiento, ya sea que se encuentra estático, yendo a la izquierda o la derecha
 * o arriba y abajo. Además conocer sus límites de movimiento. Y que todas estas variables sean
 * configurables.
 *
 * ***************
 * Atributos
 * ***************
 *
 * int m_iPosX: Posición en X del centro de la pieza.
 * int m_iPosY: Posición en Y del centro de la pieza.
 *
 * int m_iLastPosX: Posición anterior en X del centro de la pieza.
 * int m_iLastPosY: Posición anterior en Y del centro de la pieza.
 *
 *
 * int m_iDirectionX: Dirección en X del centro de la pieza.
 * int m_iDirectionY: Dirección en Y del centro de la pieza.
 *
 * int m_iLimitsX [LIMITSX_SIZE]: Límites en X del centro de la pieza.
 * int m_iLimitsY [LIMITSY_SIZE]: Límites en Y del centro de la pieza.
 *
 * bool m_bChangeY: Su valor en true indica que ocurrió un cambio en Y de la posición del centro de la pieza.
 * bool m_bChangeX: Su valor en true indica que ocurrió un cambio en X de la posición del centro de la pieza.
 *
 *
 * ***************
 * Métodos
 * ***************
 *
 * GamePiece(): Constructor de la clase.
 *
 * void setPosX(int i_iPosX): Método para configurar la posición en X del centro de la pieza.
 *
 * int getPosX(): Método para obtener la posición en X del centro de la pieza.
 *
 * void setPosY(int i_iPosY): Método para configurar la posición en Y del centro de la pieza.
 *
 * int getPosY(): Método para obtener la posición en Y del centro de la pieza.
 *
 * void setDirectionX(int i_iDirectionX): Método para configurar la dirección en X del centro de la pieza.
 * 										  Sus valores pueden ser NO_MOVE, MOVE_LEFT o MOVE_RIGHT.
 *
 * int getDirectionX(): Método para obtener la dirección en X del centro de la pieza.
 *
 * void setDirectionY(int i_iDirectionY): Método para configurar la dirección en Y del centro de la pieza.
 *  								      Sus valores pueden ser NO_MOVE, MOVE_UP o MOVE_DOWN.
 *
 * int getDirectionY(): Método para obtener la dirección en Y del centro de la pieza.
 *
 * void CheckChangeX(): Método para comprobar si ocurrió un cambio en la posición de
 * 						X del centro de la pieza. Si occurió un cambio se configura m_bChangeX = true.
 *
 * void CheckChangeY(): Método para comprobar si ocurrió un cambio en la posición de
 * 						Y del centro de la pieza. Si occurió un cambio se configura m_bChangeY = true.
 *
 * ******************************************************************************************
 */
class GamePiece
{
    public:
		GamePiece();

        void setPosX(int i_iPosX);
        int getPosX();

        void setPosY(int i_iPosY);
        int getPosY();

        void setSize(int i_iSize);
        int getSize();

        void setDirectionX(int i_iDirectionX);
        int getDirectionX();

        void setDirectionY(int i_iDirectionY);
        int getDirectionY();

		void CheckChangeX();
		void CheckChangeY();

        //
        int m_iPosX;
		int m_iPosY;

		int m_iLastPosX;
		int m_iLastPosY;


		int m_iDirectionX;
		int m_iDirectionY;

		int m_iLimitsX [LIMITSX_SIZE];
		int m_iLimitsY [LIMITSY_SIZE];

		bool m_bChangeY;
		bool m_bChangeX;

    protected:


    private:


};

#endif /* GAME_PIECE_HPP_ */
