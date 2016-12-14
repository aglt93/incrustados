/*
 * GameLogic.hpp
 *
 *  Created on: Dic, 2016
 *      Author: aglt93
 */

#ifndef GAMELOGIC_HPP_
#define GAMELOGIC_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include "Racket.hpp"
#include "Ball.hpp"

#define NO_MOVE 0
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 1
#define MOVE_RIGHT 2

#define LIMITSX_SIZE 2
#define LIMITSY_SIZE 2




/* *****************************************************************************************
 * Clase GameLogic
 * *****************************************************************************************
 *
 * ***************
 * Descripción
 * ***************
 *
 * ***************
 * Atributos
 * ***************
 * Racket RacketLeft: Objeto que modela la raqueta izquierda del juego.
 *
 * Racket RacketRight: Objeto que modela la raqueta derecha del juego.
 *
 * Ball MainBall: Objeto que modela la raqueta izquierda del juego.
 *
 * int8_t m_iRacketLeftScore: Entero que contiene el valor del puntaje del jugador que juega con la raqueta
 * 							izquierda.
 *
 * int8_t m_iRacketRightScore: Entero que contiene el valor del puntaje del jugador que juega con la raqueta
 * 							   derecha.
 *
 * int m_iBallStatus: Un entero que permite indicar el estado de la bola pueden ser:
 *
 *					  - HIT_LEFT_RACKET
   					  - HIT_RIGHT_RACKET
					  - HIT_LEFT_WALL
					  - HIT_RIGHT_WALL
					  - NO_HIT
 *
 * int m_iFsmState: Permite conocer en que estado del juego se encuentra:
 *
 * 					- GAME_INIT_STATE: Es cuando inicialmente la bola no está en movimiento y el jugador
 * 									   que tiene la bola no se ha movido.
 * 					- GAME_RUNNING_STATE: Es cuando la bola está en movimiento y el marcador todavía no
 * 						                  es de que algún jugador gano.
 * 					- WINNER_STATE_RACKET_LEFT: Es cuando el jugador de la izquierda llegó al marcador
 * 										        de gane.
 * 					- WINNER_STATE_RACKET_RIGHT: Es cuando el jugador de la derecha llegó al marcador
 * 										        de gane.
 *
 * int m_iWinnerCounterScreen: Corresponde a un entero para conocer la cantidad de ciclos de ejecución
 * 							   que se debe desplegar la pantalla de gane.
 *
 *
 *
 * ***************
 * Métodos
 * ***************
 *
 * void FirstMoveControl(): Permite conocer si ya el jugador inicialmente realizó un omvimiento
 * 							para poner la bola en movimiento.
   void ScoreControl(): Funciona para controlar los puntajes de los jugadores según el estado de la bola.
   void GameControl(): Permite controlar la relación entre las diferentes piezas del juego.
   void GameState():  Permite controlar m_iFsmState para conocer en que estado del juego se está.
 *
 * ******************************************************************************************
 */
class GameLogic : public Task
{
    public:
		GameLogic (int i_iTaskID, bool i_bPeriodicTask, int i_u64FinalCount);

		virtual MSG run(void);
		virtual MSG ProcessMessage(MSG i_message);

		Racket RacketLeft;
		Racket RacketRight;
		Ball   MainBall;
		int8_t m_iRacketLeftScore;
		int8_t m_iRacketRightScore;

		int m_iBallStatus;

		int m_iFsmState;

		int m_iWinnerCounterScreen;

		void FirstMoveControl();
		void ScoreControl();
		void GameControl();
		void GameState();

    protected:


    private:

};

#endif /* GAME_LOGIC_HPP_ */
