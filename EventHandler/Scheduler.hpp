/*
 * Scheduler.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef TASKS_SCHEDULER_HPP_
#define TASKS_SCHEDULER_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"

#define NUMBER_OF_SLOTS 255
#define NULL            0


/* *****************************************************************************************
 * Clase Scheduler
 * *****************************************************************************************
 *
 * ***************
 * Descripción
 * ***************
 * La clase es la base del RTOS. Se encarga de la administración de las tareas (tasks)
 * del OS para que las mismas se corran en el momento indicado, sean ejecutadas de manera
 * correcta y además permite la comunicación entre las diferentes tareas del sistema.
 *
 * ***************
 * Atributos
 * ***************
 * ticks: Corresponde a la cuenta del scheduler para mantener un período de ejecución de 1ms.
 * 		  La cuenta es alterada por la interrupción del Timer32 en el main.
 *
 * MessageQueue: Cola de mensajes. Aquí es donde se almacenan los msjs enviados entre tareas
 * 				 hasta que son procesados por el scheduler al final de un ciclo de ejecución
 * 				 (frame).
 *
 * ID_LUT: Corresponde a un arreglo de punteros a tareas donde la posición del puntero en el
 * 		   arreglo corresponde al ID único de la misma que permite que las otras tareas y el
 * 		   scheduler la identique de las demás.
 *
 * mOpenSlots: Cantidad de espacios abiertos en el arreglo Schedule.
 *
 * mNextSlot: Valor de la siguiente posición disponible en el arreglo Schedule.
 *
 * Schedule: Arreglo de punteros a tareas donde se encuentran todas las tareas que cuenta el OS.
 *
 * NextSchedule: Arreglo de punteros a tareas donde se encuentran las tareas que van a ser
 * 				 ejecutadas en el siguiente ciclo de ejecución.
 *
 * mMessageIndex: Índice que indica la posición del último mensaje en la cola de msjs (Message
 * 				  Queue).
 *
 * NextScheduleSlot: Valor de la siguiente posición disponible en el arreglo Schedule.
 *
 *
 *
 * ***************
 * Métodos
 * ***************
 *
 * Scheduler: Constructor de la clase.
 *
 * attach: Método para agregar un nuevo puntero a una tarea al arreglo de Schedule.
 *
 * run: Método que se ejecuta cada ciclo de ejecución para ejecutar todas las tareas
 * 		que se encuentran en el arreglo NextSchedule.
 *
 * CalculateNextSchedule: Método para agregar al arreglo NextSchedule los punteros a las
 * 						  tareas que van a ser ejecutados en el siguiente frame.
 *
 * ProcessMessageQueue: Método encargado de procesar y enviar los msjs a los tasks
 * 						correspondientes, estos msjs son los almacenados en el arreglo
 * 						MessageQueue. Así también si un msj es dirigido al scheduler,
 * 						se encarga de llamar al método correspondiente para que sea procesado.
 *
 * attachMessage: Método encargado de agregar msjs a la cola MessageQueue cuando una tarea
 * 				  o interrupción envía un msj a un task, al scheduler o a otra interrupción.
 *
 * clearMessageQueue: Método encargado de limpiar la cola de msjs cuando el scheduler
 * 					  ha finalizado el procesamiento de la misma.
 *
 * clearNextScheduler: Método encargado de limpiar NextScheduler, cuando ha terminado el frame.
 *
 * processMessage: Método encargado de procesar los msjs dirigidos al scheduler.
 *
 * SortScheduleByPriority: Método encargado de acomodar los tasks en NextSchedule
 * 						   según su prioridad.
 ******************************************************************************************
 */
class Scheduler {

	public:
		Scheduler();
		uint64_t ticks;
		uint8_t attach(Task * i_ToAttach);
		uint8_t run(void);
		uint8_t CalculateNextSchedule(void);
		void	ProcessMessageQueue();
		MSG		MessageQueue[NUMBER_OF_SLOTS];
		void	attachMessage(MSG i_messageToAttach);
		void	clearMessageQueue();
		void	clearNextScheduler();
		Task*	ID_LUT[NUMBER_OF_SLOTS];
		void	processMessage(MSG i_MSG);
	private:
		uint8_t mOpenSlots;
		uint8_t mNextSlot;
		Task * Schedule[NUMBER_OF_SLOTS];
		Task * NextSchedule[NUMBER_OF_SLOTS];
		uint8_t SortScheduleByPriority(Task * i_pSchedule);
		uint8_t mMessageIndex;
		uint8_t NextScheduleSlot;
};




#endif /* TASKS_SCHEDULER_HPP_ */
