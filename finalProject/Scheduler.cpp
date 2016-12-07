//////////////////////////////////////////////////////////////////////////////////////////////
#include "Scheduler.hpp"
#include "task_ids.hpp"
#include <driverlib.h>
//////////////////////////////////////////////////////////////////////////////////////////////

#define BUTTON_DOWN_PORT GPIO_PORT_P3
#define BUTTON_DOWN_PIN GPIO_PIN5

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

    while(NextTaskSlot < (NUMBER_OF_SLOTS-1U))
    {
        NextTask = static_cast<Task *> (NextSchedule[NextTaskSlot]);
        if(NextTask != ((uintptr_t) 0))
        {
            NextTask->run();
            NextTaskSlot++;
        }
        else
        {
            break;//NextTaskSlot++;
        }
    }
    return l_u8ReturnCode;
}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Scheduler::CalculateNextSchedule(void)
{
    uint8_t NextTaskSlot = 0U;
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

	for (int i=0; i<=NUMBER_OF_SLOTS;i++) {

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

	//GPIO_toggleOutputOnPin(RGB_BLUE_PORT,RGB_BLUE_PIN);

	for (int i = 0; i < mMessageIndex; i++) {

		MSG newMSG = MessageQueue[i];

		if (newMSG.source != -1 && newMSG.destination != -1) {

			MessageQueue[i].currentCount = MessageQueue[i].currentCount +1;
			newMSG.currentCount++;

			if (newMSG.currentCount >= newMSG.finalCount) {

				if (newMSG.destination != SCHEDULER_ID) {
					Task* newTask = ID_LUT[newMSG.destination];
					newTask->ProcessMessage(newMSG);
				}

				else {

					processMessage(newMSG);

				}


				if(newMSG.source == BUTTON_DOWN_ISR_ID) {
					clearButtonDown = true;
				}

			}
		}
	}

	// Se limpia la cola de msjs.
	clearMessageQueue();

	// Se limpia la bandera de interrupción del ADC.
	MAP_ADC14_enableInterrupt(ADC_INT0);


	if(clearButtonDown) {
		GPIO_clearInterruptFlag(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
		GPIO_enableInterrupt(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN);
		clearButtonDown = false;
	}

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

	mMessageIndex = 0;

	for (int i=0; i < newIndex; i++) {

		MessageQueue[i] = newMessageQueue[i];
		mMessageIndex++;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::processMessage(MSG i_MSG) {

	int l_iSourceTask = i_MSG.source;


	switch(l_iSourceTask) {

        // Caso ADC
        case ADC_ISR_ID:

            int* TaskID = (int*) i_MSG.data;
            Task* TaskToAdd = ID_LUT[*TaskID];
            NextSchedule[NextScheduleSlot] = TaskToAdd;
            NextScheduleSlot++;
            break;

	};

	return;

}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
uint8_t Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(NO_ERR);
}
//////////////////////////////////////////////////////////////////////////////////////////////
