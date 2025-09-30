# MP2: NachOS Process Management and System Calls

## Purpose
This project extends NachOS to support user-level processes and system calls. Students will implement the interface between user programs and the operating system kernel, including process creation, memory management, and basic I/O operations.

## Key Objectives
- Implement system call interface and handling
- Develop process creation and termination mechanisms
- Implement memory management for user processes
- Add support for file I/O operations through system calls
- Understand virtual memory concepts and address translation

## To-Do List

### System Call Implementation
- [ ] **System Call Handler**
  - Implement `ExceptionHandler()` in `exception.cc`
  - Add support for system call numbers and parameter passing
  - Implement proper return value handling

- [ ] **Process Management System Calls**
  - `SC_Halt`: Halt the machine
  - `SC_Exit`: Exit current process
  - `SC_Exec`: Execute a new program
  - `SC_Join`: Wait for child process completion

- [ ] **File I/O System Calls**
  - `SC_Create`: Create a new file
  - `SC_Open`: Open an existing file
  - `SC_Read`: Read from file descriptor
  - `SC_Write`: Write to file descriptor
  - `SC_Close`: Close file descriptor

### Memory Management
- [ ] **Address Space Management**
  - Implement `AddrSpace` constructor for loading executables
  - Implement virtual-to-physical address translation
  - Add proper memory allocation and deallocation
  - Handle page table initialization

- [ ] **Process Creation**
  - Implement `Fork()` system call for process creation
  - Set up child process address space
  - Implement proper parent-child relationship management

### Testing
- [ ] Test basic system calls (halt, exit)
- [ ] Test process creation and execution
- [ ] Test file I/O operations
- [ ] Verify memory isolation between processes
- [ ] Test error handling for invalid system calls

### Files to Modify
- `code/userprog/exception.cc`
- `code/userprog/addrspace.h` and `addrspace.cc`
- `code/userprog/syscall.h`
- `code/machine/machine.h` and `machine.cc`

## Build and Run
```bash
cd NachOS-4.0_MP2
make run
# Inside container:
bash build_nachos.sh -j
make clean; make <test>
../build.linux/nachos -x <test>
```

## Test Programs
- `halt.c`: Test halt system call
- `add.c`: Test basic arithmetic and I/O
- `consoleIO_test*.c`: Test console input/output
- `fileIO_test*.c`: Test file operations

## Key Concepts
- System call interface and implementation
- Process address spaces and memory management
- Virtual memory and address translation
- File descriptors and I/O management
- Process lifecycle (creation, execution, termination)