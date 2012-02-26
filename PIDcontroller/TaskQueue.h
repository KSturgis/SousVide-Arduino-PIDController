/*****************************************************************************
    SousVide PIDController
    Copyright ©2012 Ken Sturgis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#pragma once
#include <inttypes.h>

//set the number of tasks to alloc memory for
#define MAX_NUMTASKS 6


/**Function executed as part of every Task **/
typedef void TaskProcess(void);

struct TaskListItem;

/**
Task Queue

Handles are executed periodically while the system is running.

Tasks are added to the system using EnqueueTask().
Tasks are executed during calls to Loop().

* Tasks are executed after during the Loop() call, the first time loop
is called after the scheduled time.  
* Tasks are garanteed to run, but the schedule they are run on is not
garanteed.
* If two tasks are scheduled to run during the same _ms the task that
was added to the queue first will run first.
* Tasks are executed synchronusly and will must complete processing before the
next task is called
* Tasks should have an execution time much shorter than their scheduled 
repeat time, and the repeat time should be appropriate.
* Long running tasks will delay other scheduled tasks
* The TaskQueue relies on millis() which counts the number of milliseconds
the processor has been running.  This counter will overflow after ~50 days
causing the system to crash.

**/
class TaskQueue {
public:
	/** Singleton acces to the TaskQueue **/
	static TaskQueue* get_Instance();

	/** Adds a task to the TaskQueue
		The process will run for the first time @ currentTime + firstExec_mc
		The process will repeat every repeatEvery_ms after the first execution
	**/
	void EnqueueTask(TaskProcess* process, int32_t firstExec_ms, int32_t repeatEvery_ms);

	/** Called from the arduino loop, will execute any tasks in the taskQueue
	that are ready to be executed.

	Returns the number of ms until the next task will be ready to execute.
	We suggest you sleep the microsontroller for this number of ms **/
	int32_t  Loop();

	/** Retunrs the number of tasks in the task queue **/
	uint8_t get_NumTasks() { return numTasks; }

	/** Returns a pointer to the task in the queue that should be 
	executed next **/
	TaskListItem* get_NextTask();

private:
	static TaskQueue* _instance;
	TaskQueue();

	/** Number of tasks that are currently in the TaskItems **/
	uint8_t numTasks;
	TaskListItem* TaskItems[MAX_NUMTASKS];
};

/** Data type 
used to store items in the task queue.

Nothing to see here, move along
**/
struct TaskListItem {
	TaskListItem();

	TaskProcess* Process;
	int32_t RepeatEvery_ms;
	int32_t NextExec_ms;
};
