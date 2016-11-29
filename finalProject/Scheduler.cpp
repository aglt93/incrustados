//////////////////////////////////////////////////////////////////////////////////////////////
#include "Scheduler.hpp"
#include "task_ids.hpp"
#include <driverlib.h>
//////////////////////////////////////////////////////////////////////////////////////////////


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

	for (int i = 0; i < mMessageIndex; i++) {

		MSG newMSG = MessageQueue[i];

		if (newMSG.source != -1 && newMSG.destination != -1) {
			if (newMSG.destination != SCHEDULER_ID) {
				Task* newTask = ID_LUT[newMSG.destination];
				newTask->ProcessMessage(newMSG);
			}

			else {

				processMessage(newMSG);

			}
		}
	}

	// Se limpia la cola de msjs.
	clearMessageQueue();

	// Se limpia la bandera de interrupción del ADC.
	uint64_t g_u64Status = MAP_ADC14_getEnabledInterruptStatus();
	MAP_ADC14_clearInterruptFlag(g_u64Status);

	return;


}
//////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::attachMessage(MSG i_messageToAttach) {

	MessageQueue[mMessageIndex] = i_messageToAttach;

	if (mMessageIndex == 255) {

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

	for (int i = 0; i < mMessageIndex; i++) {

		MSG nullMSG = {-1,-1,0};
		MessageQueue[i] = nullMSG;

	}

	mMessageIndex = 0;

	return;

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
