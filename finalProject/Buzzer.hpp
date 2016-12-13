/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef BUZZER_HPP_
#define BUZZER_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include "MusicEnv.hpp"

/* *****************************************************************************************
 * Clase Buzzer
 * *****************************************************************************************
 *
 * ***************
 * Descripción
 * ***************
 * Clase encargada de ejecutar los procedimientos necesarios para sonar melodias en el Piezo
 * del BoosterPack. Se ejecuta de forma periodica y reproduce la melodía que se encuentra en el
 * arreglo Melody con un tempo para cada nota en su posición homóloga en el arreglo Tempo.
 *
 * ***************
 * Atributos
 * ***************
 * int m_iBuzzerPort: Puerto del buzzer.
 *
 * int m_iBuzzerPin: Pin del buzzer.
 *
 * int m_iMelodyIndex: Indice que indica en que nota de la melodía se encuentra la reproducción.
 *
 * int m_iTempoCounter: Contador que permite realizar temporización de las notas en negras, corcheas, etc.
 *
 * static const int Melody[MELODY_LENGTH]: Arreglo que contiene la melodía de la música. Cada posicion
 * 										   contiene la frecuencia de la nota, si el valor es 0 corresponde
 * 										   a un silencio.
 *
 * static const int Tempo[MELODY_LENGTH]: Arreglo que contiene la duración de cada nota de la melodía.
 *
 *
 * ***************
 * Métodos
 * ***************
 * Buzzer(int i_iTaskID, bool i_bPeriodicTask, int i_iBuzzerPort,
 * 		  int i_iBuzzerPin, uint64_t i_u64FinalCount): Constructor de la clase, configura los puertos,
 * 		  y la periocidad de ejecución de la tarea.
 *
 * virtual MSG run(void): Método que se ejecuta cada vez que se ejecuta el task en el scheduler. Reproduce
 * 						  correctamente la melodía que se encuentra en el arreglo Melody.
 *
 * ******************************************************************************************
 */
class Buzzer : public Task
{
    public:
        Buzzer(int i_iTaskID, bool i_bPeriodicTask, int i_iBuzzerPort, int i_iBuzzerPin, uint64_t i_u64FinalCount);
        virtual MSG run(void);
        virtual MSG ProcessMessage(MSG i_Message);
    protected:
    private:
        int m_iBuzzerPort;
        int m_iBuzzerPin;
        int m_iMelodyIndex;
        int m_iTempoCounter;
        static const int Melody[MELODY_LENGTH];
        static const int Tempo[MELODY_LENGTH];
};

#endif /* LED_HPP_ */
