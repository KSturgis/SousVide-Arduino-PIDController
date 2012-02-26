#include "TaskQueue.h"
#include <Arduino.h>

TaskListItem::TaskListItem() {
	Process=NULL;
	RepeatEvery_ms=0;
	NextExec_ms=0;
}

TaskQueue* TaskQueue::_instance=NULL;
TaskQueue* TaskQueue::get_Instance() {
	if(_instance==NULL) {
		_instance = new TaskQueue();
	}
	return _instance;
}


TaskQueue::TaskQueue() {
	numTasks=0;
	for(int i=0;i<MAX_NUMTASKS;i++) {
		TaskItems[i]=NULL;
	}
}





int32_t TaskQueue::Loop() {
	int32_t currentTime = millis();
	TaskListItem* next=get_NextTask();


	if(next==NULL) {
		//No tasks defined
		Serial.println(F("no tasks"));
		Serial.println(get_NumTasks());
		return -1;
	}
	
	if(currentTime>=next->NextExec_ms) {
		//Execute Next Task
		next->Process();
		//Reschedule task
		next->NextExec_ms += next->RepeatEvery_ms;
	}

	// get the number of millis until the next loop is needed
	// this will put the mcu in a low power state until 
	// needed
	next=get_NextTask();
	if(next==NULL) {
		//No tasks defined
		return -1;
	}

	//return the number of ms till the next task
	int32_t delta = next->NextExec_ms-currentTime-1;
	if(delta<0) { delta=0; }
	return delta;
}

TaskListItem* TaskQueue::get_NextTask() {
	TaskListItem* nextTask=NULL;
	if(numTasks>0) {
		nextTask=TaskItems[0];
		for(uint8_t idx=1; idx<numTasks; idx++) {
			if(TaskItems[idx]->NextExec_ms < nextTask->NextExec_ms) {
				nextTask=TaskItems[idx];
			}
		}
	}
	return nextTask;
}

void TaskQueue::EnqueueTask(TaskProcess* process, int32_t firstExec_ms,  int32_t repeatEveryUs) {
	if(numTasks>=MAX_NUMTASKS) {
		//exceed number of tasks
		return;
	}

	TaskListItem* item = new TaskListItem();	
	item->Process = process;
	item->RepeatEvery_ms=repeatEveryUs;
	item->NextExec_ms = millis()+firstExec_ms;


	TaskItems[numTasks++] = item;
}
