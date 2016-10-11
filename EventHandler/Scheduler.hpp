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

extern "C"
{
	struct TaskAndCounterController {
		uint64_t currentCount;
		uint64_t finalCount;
		Task* task;
	};
}

class Scheduler
{
public:
    Scheduler();
    uint64_t ticks;
    uint8_t attach(Task * i_ToAttach);
    uint8_t attach(Task * i_ToAttach, uint64_t i_u64TickInterval);
    uint8_t run(void);
private:
    uint8_t mOpenSlots;
    uint8_t mNextSlot;
    TaskAndCounterController Schedule[NUMBER_OF_SLOTS];
    TaskAndCounterController NextSchedule[NUMBER_OF_SLOTS];
    uint8_t CalculateNextSchedule(void);
    uint8_t SortScheduleByPriority(Task * i_pSchedule);
};






#endif /* TASKS_SCHEDULER_HPP_ */
