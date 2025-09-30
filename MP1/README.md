# MP1: NachOS Thread Management

## Purpose
This project implements basic thread management functionality in the NachOS (Not Another Completely Heuristic Operating System) educational operating system. The focus is on understanding and implementing fundamental thread operations, synchronization primitives, and thread scheduling mechanisms.

## Key Objectives
- Implement thread creation, deletion, and management
- Develop thread synchronization mechanisms (locks, semaphores, condition variables)
- Understand thread scheduling and context switching
- Implement basic thread safety and mutual exclusion

## To-Do List

### Core Implementation
- [ ] **Thread Class Enhancement**
  - Implement `Thread::Fork()` for thread creation
  - Implement `Thread::Finish()` for thread termination
  - Add proper thread state management (READY, RUNNING, BLOCKED)

- [ ] **Synchronization Primitives**
  - Complete `Lock` class implementation
  - Complete `Semaphore` class implementation
  - Complete `Condition` class implementation
  - Ensure proper mutual exclusion and signaling

- [ ] **Scheduler Implementation**
  - Implement `Scheduler::FindNextToRun()`
  - Implement proper thread switching mechanism
  - Add support for different scheduling algorithms

### Testing
- [ ] Test thread creation and termination
- [ ] Test synchronization primitives with multiple threads
- [ ] Verify deadlock prevention mechanisms
- [ ] Test scheduler behavior under various loads

### Files to Modify
- `code/threads/thread.h` and `thread.cc`
- `code/threads/synch.h` and `synch.cc`
- `code/threads/scheduler.h` and `scheduler.cc`

## Build and Run
```bash
cd NachOS-4.0_MP1
make run
# Inside container:
bash build_nachos.sh -j
make clean; make <test>
../build.linux/nachos -e <test>
```

## Key Concepts
- Thread lifecycle management
- Mutual exclusion and critical sections
- Producer-consumer problem
- Reader-writer problem
- Thread synchronization patterns