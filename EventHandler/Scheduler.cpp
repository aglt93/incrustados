#include "Scheduler.hpp"

Scheduler::Scheduler()
{
    mOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    mNextSlot = 0;
    for(uint8_t index = 0; index < (NUMBER_OF_SLOTS-1U); index++)
    {
        Schedule[index] = (uintptr_t) 0;
    }
    return;
}

uint8_t Scheduler::attach(Task * i_ToAttach)
{
    uint8_t l_ErrorCode = NO_ERR;
    if((mOpenSlots>0) && (mNextSlot < NUMBER_OF_SLOTS))
    {
        Schedule[mNextSlot] = i_ToAttach;
        mOpenSlots--;
        mNextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}


uint8_t Scheduler::attach(Task * i_ToAttach, uint64_t i_u64TickInterval)
{
    uint8_t l_ErrorCode = NO_ERR;
    if((mOpenSlots>0) && (mNextSlot < NUMBER_OF_SLOTS))
    {
        Schedule[mNextSlot] = i_ToAttach;
        mOpenSlots--;
        mNextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}


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

uint8_t Scheduler::CalculateNextSchedule(void)
{
    uint8_t NextTaskSlot = 0U;
    uint8_t NextScheduleSlot = 0U;
    Task * NextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;
    uint64_t l_u64CurrentCount;
    uint64_t l_u64FinalCount;

    for (int i=0; i<=NUMBER_OF_SLOTS;i++) {

    	if (NextSchedule[i] == NULL) {
    		break;
    	}

    	NextSchedule[i] = NULL;
    }


    while(NextTaskSlot < (NUMBER_OF_SLOTS-1U)) {

        NextTask = static_cast<Task *> (Schedule[NextTaskSlot]);

        l_u64CurrentCount = NextTask->GetTaskCurrentCount() + 1;
        l_u64FinalCount = NextTask->GetTaskFinalCount();
        NextTask->SetTaskCurrentCount(l_u64CurrentCount);

        if(NextTask == ((uintptr_t) 0)){
        	break;
        }


        else if ((l_u64CurrentCount >= l_u64FinalCount)) {

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

uint8_t Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(NO_ERR);
}



void Scheduler::ProcessMessageQueue() {

	return;




}