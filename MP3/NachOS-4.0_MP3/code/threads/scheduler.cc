// scheduler.cc
//	Routines to choose the next thread to run, and to dispatch to
//	that thread.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would
//	end up calling FindNextToRun(), and that would put us in an
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.
//	Might need to be improved in later assignments.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "scheduler.h"

#include "copyright.h"
#include "debug.h"
#include "main.h"

static int L1Cmp(Thread *newThread, Thread *cmpThread);
static int L2Cmp(Thread *newThread, Thread *cmpThread);

//----------------------------------------------------------------------
// Scheduler::Scheduler
// 	Initialize the list of ready but not running threads.
//	Initially, no ready threads.
//----------------------------------------------------------------------

Scheduler::Scheduler() {
    // readyList = new List<Thread *>;  // old version
    L1 = new SortedList<Thread *>(L1Cmp);
    L2 = new SortedList<Thread *>(L2Cmp);
    L3 = new List<Thread *>;
    toBeDestroyed = NULL;
}

//----------------------------------------------------------------------
// Scheduler::~Scheduler
// 	De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler() {
    // delete readyList;  // old version
    delete L1;
    delete L2;
    delete L3;
}

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
// 	Mark a thread as ready, but not running.
//	Put it on the ready list, for later scheduling onto the CPU.
//
//	"thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------

void Scheduler::ReadyToRun(Thread *thread) {
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    DEBUG(dbgThread, "Putting thread on ready list: " << thread->getName());
    // cout << "Putting thread on ready list: " << thread->getName() << endl;
    thread->setStatus(READY);
    thread->TimeUpdate_ToReady(kernel->stats->totalTicks);

    // readyList->Append(thread);
    DEBUG(dbgThread, ":D, " << thread->getID() << ": "
                            << thread->rem_approximatedBurstTime);
    // Multilevel feedback queue
    if (thread->priority <= 49) {
        L3->Append(thread);
        DEBUG(dbgZ, "[A] Tick [" << kernel->stats->totalTicks << "]: Thread ["
                                 << thread->getID()
                                 << "] is inserted into queue L[" << 3 << "]");
        thread->inWhichQueue = 3;
    } else if (thread->priority <= 99) {
        L2->Insert(thread);
        DEBUG(dbgZ, "[A] Tick [" << kernel->stats->totalTicks << "]: Thread ["
                                 << thread->getID()
                                 << "] is inserted into queue L[" << 2 << "]");
        thread->inWhichQueue = 2;
    } else {
        L1->Insert(thread);
        DEBUG(dbgZ, "[A] Tick [" << kernel->stats->totalTicks << "]: Thread ["
                                 << thread->getID()
                                 << "] is inserted into queue L[" << 1 << "]");
        thread->inWhichQueue = 1;
    }
    // CheckPreempt_btMultipleReadyQueue();
    // CheckPreempt_inSingleReadyQueue();
    DEBUG(dbgThread, ":OO");
}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
// 	Return the next thread to be scheduled onto the CPU.
//	If there are no ready threads, return NULL.
// Side effect:
//	Thread is removed from the ready list.
//----------------------------------------------------------------------

Thread *Scheduler::FindNextToRun() {
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    // if (readyList->IsEmpty()) {
    //     return NULL;
    // } else {
    //     return readyList->RemoveFront();
    // }

    // Multilevel feedback queue
    if (!L1->IsEmpty()) {
        DEBUG(dbgZ, "[B] Tick [" << kernel->stats->totalTicks << "]: Thread ["
                                 << L1->Front()->getID()
                                 << "] is removed from queue L[" << 1 << "]");
        return L1->RemoveFront();
    } else if (!L2->IsEmpty()) {
        DEBUG(dbgZ, "[B] Tick [" << kernel->stats->totalTicks << "]: Thread ["
                                 << L2->Front()->getID()
                                 << "] is removed from queue L[" << 2 << "]");
        return L2->RemoveFront();
    } else if (!L3->IsEmpty()) {
        DEBUG(dbgZ, "[B] Tick [" << kernel->stats->totalTicks << "]: Thread ["
                                 << L3->Front()->getID()
                                 << "] is removed from queue L[" << 3 << "]");
        return L3->RemoveFront();
    } else {
        return NULL;
    }
}

//----------------------------------------------------------------------
// Scheduler::Run
// 	Dispatch the CPU to nextThread.  Save the state of the old thread,
//	and load the state of the new thread, by calling the machine
//	dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//	already been changed from running to blocked or ready (depending).
// Side effect:
//	The global variable kernel->currentThread becomes nextThread.
//
//	"nextThread" is the thread to be put into the CPU.
//	"finishing" is set if the current thread is to be deleted
//		once we're no longer running on its stack
//		(when the next thread starts running)
//----------------------------------------------------------------------

void Scheduler::Run(Thread *nextThread, bool finishing) {
    Thread *oldThread = kernel->currentThread;

    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (finishing) {  // mark that we need to delete current thread
        ASSERT(toBeDestroyed == NULL);
        toBeDestroyed = oldThread;
    }

    if (oldThread->space != NULL) {  // if this thread is a user program,
        oldThread->SaveUserState();  // save the user's CPU registers
        oldThread->space->SaveState();
    }

    oldThread->CheckOverflow();  // check if the old thread
                                 // had an undetected stack overflow

    kernel->currentThread = nextThread;  // switch to the next thread
    nextThread->setStatus(RUNNING);      // nextThread is now running
    nextThread->TimeUpdate_ToRun(kernel->stats->totalTicks);
    DEBUG(dbgThread, "Updated startRunning time: "
                         << kernel->currentThread->timeStamp_startRunning);

    DEBUG(dbgThread, "Switching from: " << oldThread->getName()
                                        << " to: " << nextThread->getName());
    // This is a machine-dependent assembly language routine defined
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".

    DEBUG(dbgThread, "Before SWITCH(), oldThread "
                         << oldThread->getName()
                         << " status: " << oldThread->getStatus());
    SWITCH(oldThread, nextThread);
    DEBUG(dbgThread, "After SWITCH(), oldThread "
                         << oldThread->getName()
                         << " status: " << oldThread->getStatus());

    // we're back, running oldThread

    // interrupts are off when we return from switch!
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    DEBUG(dbgThread, "Now in thread: " << oldThread->getName());

    CheckToBeDestroyed();  // check if thread we were running
                           // before this one has finished
                           // and needs to be cleaned up

    if (oldThread->space != NULL) {     // if there is an address space
        oldThread->RestoreUserState();  // to restore, do it.
        oldThread->space->RestoreState();
    }
}

//----------------------------------------------------------------------
// Scheduler::CheckToBeDestroyed
// 	If the old thread gave up the processor because it was finishing,
// 	we need to delete its carcass.  Note we cannot delete the thread
// 	before now (for example, in Thread::Finish()), because up to this
// 	point, we were still running on the old thread's stack!
//----------------------------------------------------------------------

void Scheduler::CheckToBeDestroyed() {
    if (toBeDestroyed != NULL) {
        delete toBeDestroyed;
        toBeDestroyed = NULL;
    }
}

//----------------------------------------------------------------------
// Scheduler::Print
// 	Print the scheduler state -- in other words, the contents of
//	the ready list.  For debugging.
//----------------------------------------------------------------------
void Scheduler::Print() {
    cout << "Ready list contents:\n";
    // readyList->Apply(ThreadPrint);
}

int Scheduler::Aging() {
    ListIterator<Thread *> *iterator;
    Thread *thread;
    for (int i = 1; i <= 3; i++) {
        if (i == 1) {
            iterator = new ListIterator<Thread *>(L1);
        } else if (i == 2) {
            iterator = new ListIterator<Thread *>(L2);
        } else {
            iterator = new ListIterator<Thread *>(L3);
        }
        for (; !iterator->IsDone(); iterator->Next()) {
            thread = iterator->Item();
            if (kernel->stats->totalTicks - thread->timeStamp_startReady >=
                1500) {
                int old_priority = thread->priority;
                thread->priority =
                    (thread->priority + 10 > 149) ? 149 : thread->priority + 10;
                thread->timeStamp_startReady = kernel->stats->totalTicks;
                DEBUG(dbgZ, "[C] Tick [" << kernel->stats->totalTicks
                                         << "]: Thread [" << thread->getID()
                                         << "] changes its priority from ["
                                         << old_priority << "] to ["
                                         << thread->priority << "]");
            }
        }
        delete iterator;
    }
    return ReArrange();
    // DEBUG(dbgThread, "U_U " << kernel->stats->totalTicks);
}

int Scheduler::ReArrange() {
    ListIterator<Thread *> *iterator;
    Thread *thread;
    SortedList<Thread *> *new_L1;
    SortedList<Thread *> *new_L2;
    List<Thread *> *new_L3;
    new_L1 = new SortedList<Thread *>(L1Cmp);
    new_L2 = new SortedList<Thread *>(L2Cmp);
    new_L3 = new List<Thread *>;

    for (int i = 1; i <= 3; i++) {
        if (i == 1) {
            iterator = new ListIterator<Thread *>(L1);
        } else if (i == 2) {
            iterator = new ListIterator<Thread *>(L2);
        } else {
            iterator = new ListIterator<Thread *>(L3);
        }
        for (; !iterator->IsDone(); iterator->Next()) {
            thread = iterator->Item();
            if (thread->priority > 99) {
                new_L1->Insert(thread);
                if (thread->inWhichQueue != 1) {
                    thread->inWhichQueue = 1;
                }
            } else if (thread->priority > 49) {
                new_L2->Insert(thread);
                if (thread->inWhichQueue != 2) {
                    thread->inWhichQueue = 2;
                }
            } else {
                new_L3->Append(thread);
                if (thread->inWhichQueue != 3) {
                    thread->inWhichQueue = 3;
                }
            }
        }
        delete iterator;
    }
    delete L1;
    delete L2;
    delete L3;
    L1 = new_L1;
    L2 = new_L2;
    L3 = new_L3;
    int doYeild1 = CheckPreempt_btMultipleReadyQueue();
    int doYeild2 = CheckPreempt_inSingleReadyQueue();
    return (doYeild1 || doYeild2);
}

int Scheduler::CheckYield(Thread *currThread) {
    int doYield = 1;
    if (currThread->inWhichQueue == 1) {
        return !doYield;
    } else if (currThread->inWhichQueue == 2) {
        return !doYield;
    } else {
        if (kernel->stats->totalTicks - currThread->timeStamp_startRunning >=
            100) {
            return doYield;
        } else {
            return !doYield;
        }
    }
}

bool Scheduler::CheckPreempt_btMultipleReadyQueue() {
    bool contextSwitch = false;
    if ((!L1->IsEmpty()) && (kernel->currentThread->inWhichQueue > 1)) {
        contextSwitch = true;
    }
    if ((!L2->IsEmpty()) && (kernel->currentThread->inWhichQueue > 2)) {
        contextSwitch = true;
    }
    return contextSwitch;
}

bool Scheduler::CheckPreempt_inSingleReadyQueue() {
    bool contextSwitch = false;
    DEBUG(dbgThread, "totalRunning: "
                         << kernel->currentThread->totalRunningTime
                         << ", rem_appr: "
                         << kernel->currentThread->curr_approximatedBurstTime -
                                (kernel->stats->totalTicks -
                                 kernel->currentThread->timeStamp_startRunning +
                                 kernel->currentThread->totalRunningTime));
    if (kernel->currentThread->inWhichQueue == 1 && !L1->IsEmpty()) {
        if (kernel->currentThread->curr_approximatedBurstTime -
                (kernel->stats->totalTicks -
                 kernel->currentThread->timeStamp_startRunning +
                 kernel->currentThread->totalRunningTime) >
            L1->Front()->rem_approximatedBurstTime) {
            contextSwitch = true;
        } else if (kernel->currentThread->curr_approximatedBurstTime -
                           (kernel->stats->totalTicks -
                            kernel->currentThread->timeStamp_startRunning +
                            kernel->currentThread->totalRunningTime) ==
                       L1->Front()->rem_approximatedBurstTime &&
                   kernel->currentThread->getID() > L1->Front()->getID()) {
            contextSwitch = true;
        }
    }
    return contextSwitch;
}

// void Scheduler::CheckPreempt_btMultipleReadyQueue() {
//     bool contextSwitch = false;
//     if ((!L1->IsEmpty()) && (kernel->currentThread->inWhichQueue > 1)) {
//         contextSwitch = true;
//     }
//     if ((!L2->IsEmpty()) && (kernel->currentThread->inWhichQueue > 2)) {
//         contextSwitch = true;
//     }
//     if (contextSwitch) {
//         DEBUG(dbgZ, "OuO");
//         kernel->currentThread->TimeUpdate_RunningToReady(
//             kernel->stats->totalTicks);
//         Thread *nextThread = FindNextToRun();
//         if (nextThread != NULL) {
//             DEBUG(dbgZ, "[E] Tick ["
//                             << kernel->stats->totalTicks << "]: Thread ["
//                             << nextThread->getID()
//                             << "] is now selected for execution, thread ["
//                             << kernel->currentThread->getID()
//                             << "] is replaced, and it has executed ["
//                             << kernel->currentThread->totalRunningTime
//                             << "] ticks");
//             ReadyToRun(kernel->currentThread);
//             Run(nextThread, FALSE);
//         }
//     }
// }

// void Scheduler::CheckPreempt_inSingleReadyQueue() {
//     bool contextSwitch = false;
//     DEBUG(dbgThread, "totalRunning: "
//                          << kernel->currentThread->totalRunningTime
//                          << ", rem_appr: "
//                          << kernel->currentThread->curr_approximatedBurstTime -
//                                 (kernel->stats->totalTicks -
//                                  kernel->currentThread->timeStamp_startRunning +
//                                  kernel->currentThread->totalRunningTime));
//     if (kernel->currentThread->inWhichQueue == 1 && !L1->IsEmpty()) {
//         if (kernel->currentThread->curr_approximatedBurstTime -
//                 (kernel->stats->totalTicks -
//                  kernel->currentThread->timeStamp_startRunning +
//                  kernel->currentThread->totalRunningTime) >
//             L1->Front()->rem_approximatedBurstTime) {
//             contextSwitch = true;
//         } else if (kernel->currentThread->curr_approximatedBurstTime -
//                            (kernel->stats->totalTicks -
//                             kernel->currentThread->timeStamp_startRunning +
//                             kernel->currentThread->totalRunningTime) ==
//                        L1->Front()->rem_approximatedBurstTime &&
//                    kernel->currentThread->getID() > L1->Front()->getID()) {
//             contextSwitch = true;
//         }
//     }
//     if (contextSwitch) {
//         DEBUG(dbgZ, "OwO");
//         DEBUG(dbgThread, "do context switching");
//         kernel->currentThread->TimeUpdate_RunningToReady(
//             kernel->stats->totalTicks);
//         Thread *nextThread = FindNextToRun();
//         if (nextThread != NULL) {
//             DEBUG(dbgZ, "[E] Tick ["
//                             << kernel->stats->totalTicks << "]: Thread ["
//                             << nextThread->getID()
//                             << "] is now selected for execution, thread ["
//                             << kernel->currentThread->getID()
//                             << "] is replaced, and it has executed ["
//                             << kernel->currentThread->totalRunningTime
//                             << "] ticks");
//             ReadyToRun(kernel->currentThread);
//             Run(nextThread, FALSE);
//         }
//     }
// }

static int L1Cmp(Thread *newThread, Thread *cmpThread) {
    if (newThread->rem_approximatedBurstTime <
        cmpThread->rem_approximatedBurstTime) {
        return -1;
    } else if (newThread->rem_approximatedBurstTime ==
               cmpThread->rem_approximatedBurstTime) {
        if (newThread->getID() < cmpThread->getID()) {
            return -1;
        } else {
            return 1;
        }
    } else {
        return 1;
    }
}

static int L2Cmp(Thread *newThread, Thread *cmpThread) {
    if (newThread->priority > cmpThread->priority) {
        return -1;
    } else if (newThread->priority == cmpThread->priority) {
        if (newThread->getID() < cmpThread->getID()) {
            return -1;
        } else {
            return 1;
        }
    } else {
        return 1;
    }
}
