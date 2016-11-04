#ifndef TASK_HPP_
#define TASK_HPP_

#define __NOP __nop
#include "msp.h"

#define NO_ERR 0
#define RET_ERR 1

struct MSG {
	int source;
	int destination;
	void* data;
};


class Task
{
	public:
		Task();
		int m_iTaskID;
		virtual uint8_t     run(void){return(0);};
		bool                IsTaskFinished(void){return m_bIsFinished;};
		uint8_t             GetTaskPriority(void) {return m_u8Priority;};
		void                SetTaskPriority(uint8_t i_u8NewPriority){m_u8Priority = i_u8NewPriority;};
		void				SetTaskCurrentCount(uint64_t i_u64CurrentCount);
		void				SetTaskFinalCount(uint64_t i_u64FinalCount);
		uint64_t			GetTaskCurrentCount();
		uint64_t			GetTaskFinalCount();
		virtual void		ProcessMessage(MSG i_message){return;};
		virtual MSG			SendMessage(){MSG nullMsg = {0,0,0}; return nullMsg;};
		bool				m_bPeriodicTask;

	private:
	   static uint8_t m_u8NextTaskID;
	   uint8_t m_u8Priority;
	protected:
	   bool m_bIsFinished;
	   uint64_t m_u64CurrentCount;
	   uint64_t m_u64FinalCount;

};



#endif /* TASK_HPP_ */
