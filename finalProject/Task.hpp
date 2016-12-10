#ifndef TASK_HPP_
#define TASK_HPP_

#define __NOP __nop
#include "msp.h"

#define NO_ERR 0
#define RET_ERR 1


/*
 * Struct que describe la estructura de un mensaje para comunicarse entre tasks, con el
 * scheduler o con interrupciones. Está conformado por 2 ints que correspondes a los IDs
 * del task, el scheduler o la interrupción, que envía y el destino del msj estos IDs
 * se encuentran en el archivo task_ids.hpp. Además se incluye un puntero genérico el cual
 * corresponde a un puntero a los datos que desean ser transmitidos entre tareas.
 */
struct MSG {
	int source;
	int destination;
	void* data;
	int currentCount;
	int finalCount;
};



/* *****************************************************************************************
 * Clase Task
 * *****************************************************************************************
 *
 *
 * ***************
 * Descripción
 * ***************
 * La clase modela una tarea general que puede ser corrida por el scheduler del RTOS. Así
 * las tareas más específicas, heredan de esta permitiendo modularidad y escalabilidad al
 * código. Cada tarea como mínimo debe contar con atributos para su ID, su periocidad y
 * prioridad. Así también con los métodos para que la tarea puede configurar estos valores
 * y enviar msjs a otros tasks.
 *
 *
 * ***************
 * Atributos
 * ***************
 * m_u8Priority: Variable que permite definir la prioridad de la tarea para que el scheduler
 * 				 pueda ejecutar las tareas de mayor prioridad inicialmente.
 *
 * m_bIsFinished: Variable para definir si el task fue ejecutado o no dentro de un frame, esto
 * 			      permite saber si el task debe ser ejecutado en el siguiente frame.
 *
 * m_u64CurrentCount: Valor que permite conocer, cuando una tarea es periodica, cuanto falta
 * 					  para que la tarea sea ejecutada.
 *
 * m_u64FinalCount: Valor que permite conocer, la periocidad de la tarea. Por ejemplo si este
 * 					valor es 200, significa que la tarea será ejecutada cada 200 ciclos de ejecución
 *
 * m_bPeriodicTask: Valor para definir si una task es periodica o no. True indica que es periodica
 * 					False que no lo es.
 *
 * m_iTaskID: Valor para indicar un int ID único para la tarea.
 *
 *
 * ***************
 * Métodos
 * ***************
 * Task: Constructor de la clase. Configura la periocidad en true y m_bIsFinished en false.
 *
 * run: Método que se ejecuta cuanto se agrega la tarea en ciclo de ejecución.
 *
 * IsTaskFinished: Retorna el valor del m_bIsFinished.
 *
 * GetTaskPriority: Retorna el valor de prioridad asignado a la tarea.
 *
 * SetTaskPriority: Configura el valor de prioridad asignado a la tarea.
 *
 * SetTaskCurrentCount: Configura el valor de la cuenta actual de ciclos de ejecución para
 * 						determinar cuando el task debe ser ejecutado nuevamente.
 *
 * SetTaskFinalCount: Configura el valor que indica la cantidad de ciclos de espera antes de
 * 					  ejecuta la tarea nuevamente.
 *
 * GetTaskCurrentCount: Retorna el valor de la cuenta actual de ciclos de ejecución para
 * 						determinar cuando el task debe ser ejecutado nuevamente.
 *
 * GetTaskFinalCount: Retorna el valor que indica la cantidad de ciclos de espera antes de
 * 					  ejecuta la tarea nuevamente.
 *
 * ProcessMessage: Método para procesar un msj enviado a la tarea.
 *
 * SendMessage: Método para enviar un msj a una tarea, al scheduelr o a una interrupción.
 *
 * ******************************************************************************************
 */
class Task
{
	public:
		Task();
		int m_iTaskID;
		virtual MSG		    run(void);
		bool                IsTaskFinished(void){return m_bIsFinished;};
		uint8_t             GetTaskPriority(void) {return m_u8Priority;};
		void                SetTaskPriority(uint8_t i_u8NewPriority){m_u8Priority = i_u8NewPriority;};
		void				SetTaskCurrentCount(uint64_t i_u64CurrentCount);
		void				SetTaskFinalCount(uint64_t i_u64FinalCount);
		uint64_t			GetTaskCurrentCount();
		uint64_t			GetTaskFinalCount();
		virtual MSG			ProcessMessage(MSG i_message){MSG nullMsg = {-1,-1,0,0,1}; return nullMsg;};
		virtual MSG			SendMessage(){MSG nullMsg = {-1,-1,0,0,1}; return nullMsg;};
		bool				m_bPeriodicTask;

	private:
	   uint8_t m_u8Priority;

	protected:
	   bool m_bIsFinished;
	   uint64_t m_u64CurrentCount;
	   uint64_t m_u64FinalCount;

};



#endif /* TASK_HPP_ */
