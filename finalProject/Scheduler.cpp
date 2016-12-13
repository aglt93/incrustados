//////////////////////////////////////////////////////////////////////////////////////////////
#include "Scheduler.hpp"
#include "task_ids.hpp"
#include <driverlib.h>
//////////////////////////////////////////////////////////////////////////////////////////////

#define BUTTON_DOWN_PORT GPIO_PORT_P3
#define BUTTON_DOWN_PIN GPIO_PIN5
#define BUTTON_UP_PORT GPIO_PORT_P5
#define BUTTON_UP_PIN GPIO_PIN1


#define RGB_BLUE_PORT GPIO_PORT_P5
#define RGB_BLUE_PIN GPIO_PIN6
//////////////////////////////////////////////////////////////////////////////////////////////
Scheduler::Scheduler()
{
    mOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    mNextSlot = 0;
    mMessageIndex = 0;
    NextScheduleSlot = 0U;

    for(uint8_t index = 0; index < (NUMBER_OF_SLOTS-1U); index++)
    {
        Schedule[index] = (uintptr_t) 0;
    }



    // Se limpia la cola de msjs inicialmente poniendo en cada
    // posicion un nullMsg.
    MSG nullMSG = {-1,-1,0,0,1};

    for (int i = 0; i < NUMBER_OF_SLOTS; i++) {

    	MessageQueue[i] = nullMSG;

    }

    return;
}
//////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Scheduler::attach(Task * i_ToAttach)
{

	// Se agrega un task al arreglo Schedule
	// y al ID_LUT para poder mapear los IDs de los tasks
	// a sus punteros.
    uint8_t l_ErrorCode = NO_ERR;
    if((mOpenSlots>0) && (mNextSlot < NUMBER_OF_SLOTS))
    {
        Schedule[mNextSlot] = i_ToAttach;
        ID_LUT[i_ToAttach->m_iTaskID] = i_ToAttach;
        mOpenSlots--;
        mNextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Scheduler::run(void)
{
    uint8_t NextTaskSlot = 0U;
    Task * NextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;


    // Se ejecuta cada uno de los tasks contenidos en NextSchedule
    // si el valor retornado es un MSG válido se agrega a la cola de msjs
    // y cuando se llega un Task que es null pointer se sale del ciclo.
    while(NextTaskSlot < (NUMBER_OF_SLOTS-1U))
    {
        NextTask = static_cast<Task *> (NextSchedule[NextTaskSlot]);

        if(NextTask != ((uintptr_t) 0))
        {
        	MSG messageFromTask = NextTask->run();
        	if (messageFromTask.source != -1 || messageFromTask.destination != -1) {
				this->attachMessage(messageFromTask);
			}

            NextTaskSlot++;
        }
        else
        {
            break;
        }
    }
    return l_u8ReturnCode;
}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Scheduler::CalculateNextSchedule(void)
{
	int NextTaskSlot = 0;
    Task * NextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;
    uint64_t l_u64CurrentCount;
    uint64_t l_u64FinalCount;

    while(NextTaskSlot < (NUMBER_OF_SLOTS-1U)) {

        NextTask = static_cast<Task *> (Schedule[NextTaskSlot]);

        l_u64CurrentCount = NextTask->GetTaskCurrentCount() + 1;
        l_u64FinalCount = NextTask->GetTaskFinalCount();
        NextTask->SetTaskCurrentCount(l_u64CurrentCount);

        if(NextTask == ((uintptr_t) 0)){
        	break;
        }


        else if ((l_u64CurrentCount >= l_u64FinalCount) && (NextTask->m_bPeriodicTask)) {

			NextTask->SetTaskCurrentCount(0);
			NextSchedule[NextScheduleSlot] = NextTask;
			NextTaskSlot++;
			NextScheduleSlot++;
		}

		else {

			NextTaskSlot++;

		}
    }

    return l_u8ReturnCode;
}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::clearNextScheduler() {

	for (int i=0; i<NextScheduleSlot;i++) {

		if (NextSchedule[i] == NULL) {
			break;
		}

		NextSchedule[i] = NULL;
	}

	NextScheduleSlot = 0U;

	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::ProcessMessageQueue() {

	bool clearButtonDown = false;
	bool clearADC = false;
	bool clearButtonUp = false;

	for (int i = 0; i < mMessageIndex; i++) {

		MSG newMSG = MessageQueue[i];

		if (newMSG.source != -1 && newMSG.destination != -1) {

			// Se cambia la cuenta actual de proceso del msj.
			MessageQueue[i].currentCount = MessageQueue[i].currentCount +1;
			newMSG.currentCount++;

			// Si la cuenta actual es igual o mayor que la cuenta final
			// implica que el msj debe ser procesado.
			if (newMSG.currentCount >= newMSG.finalCount) {

				////////////////////////////////////////////////////////////////////////
				// Si el msj va dirigido al scheduler entonces se procesa con su método
				// para procesar msjs en caso contrario, se llama al método que procesa
				// el msj del task correspondiente.
				////////////////////////////////////////////////////////////////////////
				if (newMSG.destination != SCHEDULER_ID) {

					Task* newTask = ID_LUT[newMSG.destination];
					MSG messageFromTask = newTask->ProcessMessage(newMSG);

					if (messageFromTask.source != -1 || messageFromTask.destination != -1) {
						this->attachMessage(messageFromTask);
					}
				}

				else {

					processMessage(newMSG);

				}
				////////////////////////////////////////////////////////////////////////

				////////////////////////////////////////////
				// Si uno de los msjs provenía de una
				// interrupción se verifica cual era y se
				// solicita rehabilitar tal interrupción.
				////////////////////////////////////////////
				switch (newMSG.source) {

					case BUTTON_DOWN_ISR_ID:
						clearButtonDown = true;
						break;

					case BUTTON_UP_ISR_ID:
						clearButtonUp = true;
						break;

					case ADC_ISR_ID:
						clearADC = true;
						break;

				}
				////////////////////////////////////////////


			}
		}
	}


	////////////////////////////////////////////////////////////////////////
	// Se limpia la cola de msjs.
	clearMessageQueue();
	////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////
	// Se rehabilitan las interrupciones si el msj enviado por las mismas
	// ya fue procesado.
	////////////////////////////////////////////////////////////////////////
	if(clearButtonDown) {
		GPIO_clearInterruptFlag(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
		GPIO_enableInterrupt(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
		clearButtonDown = false;
	}

	if(clearButtonUp) {
			GPIO_clearInterruptFlag(BUTTON_UP_PORT, BUTTON_UP_PIN);
			GPIO_enableInterrupt(BUTTON_UP_PORT, BUTTON_UP_PIN);
			clearButtonDown = false;
		}

	if(clearADC){
		MAP_ADC14_enableInterrupt(ADC_INT0);
		clearADC = true;
	}
	////////////////////////////////////////////////////////////////////////

	return;


}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::attachMessage(MSG i_messageToAttach) {

	MessageQueue[mMessageIndex] = i_messageToAttach;

	if (mMessageIndex == NUMBER_OF_SLOTS) {

		mMessageIndex = 0;

	}

	else {

		mMessageIndex++;

	}

	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::clearMessageQueue() {


	int newIndex = 0;
	MSG messageToClear;
	MSG newMessageQueue[NUMBER_OF_SLOTS];
	MSG nullMSG = {-1,-1,0,0,1};

	//////////////////////////////////////////////////////////////////////////
	// Se limpia la cola de msjs poniendo en nullMsg los msjs que ya fueron
	// procesados. Cuando un msj no ha sido procesado porque todavía debe
	// esperar cierta cantidad de frames, se guarda en un arreglo temporal.
	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < mMessageIndex; i++) {

		messageToClear = MessageQueue[i];

		if (messageToClear.currentCount < messageToClear.finalCount){

			newMessageQueue[newIndex] = messageToClear;
			newIndex++;

			MessageQueue[i] = nullMSG;

		}

		else {

			MessageQueue[i] = nullMSG;

		}


	}
	//////////////////////////////////////////////////////////////////////////


	// Se reinicia el indice de msjs.
	mMessageIndex = 0;


	//////////////////////////////////////////////////////////////////////////
	// Se transfieren los msjs pendientes a la cola de msjs.
	//////////////////////////////////////////////////////////////////////////
	for (int i=0; i < newIndex; i++) {

		MessageQueue[i] = newMessageQueue[i];
		mMessageIndex++;
	}
	//////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::processMessage(MSG i_MSG) {

}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(NO_ERR);
}
//////////////////////////////////////////////////////////////////////////////////////////////
