#include "Task.hpp"

Task::Task()
{
    //m_u8TaskID = m_u8NextTaskID;
    //m_u8NextTaskID++;
	m_bPeriodicTask = true;
    m_bIsFinished = false;
}

void Task::SetTaskCurrentCount(uint64_t i_u64CurrentCount) {

	m_u64CurrentCount = i_u64CurrentCount;

}


void Task::SetTaskFinalCount(uint64_t i_u64FinalCount) {

	m_u64FinalCount = i_u64FinalCount;

}


uint64_t Task::GetTaskCurrentCount() {

	return m_u64CurrentCount;

}

uint64_t Task::GetTaskFinalCount() {

	return m_u64FinalCount;
}
