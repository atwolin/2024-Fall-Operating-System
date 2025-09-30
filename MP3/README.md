# MP3: NachOS Virtual Memory Management

## Purpose
This project implements a virtual memory system in NachOS, including demand paging, page replacement algorithms, and translation lookaside buffers (TLB). Students will understand how modern operating systems manage memory efficiently through virtual memory techniques.

## Key Objectives
- Implement demand paging with page fault handling
- Develop page replacement algorithms (FIFO, LRU, etc.)
- Implement Translation Lookaside Buffer (TLB) management
- Add support for memory mapping and shared memory
- Understand virtual memory performance optimizations

## To-Do List

### Virtual Memory Core
- [ ] **Page Fault Handling**
  - Implement page fault exception handler
  - Add demand paging mechanism
  - Implement page loading from executable/swap file
  - Handle invalid memory access exceptions

- [ ] **Page Replacement Algorithms**
  - Implement FIFO (First-In-First-Out) replacement
  - Implement LRU (Least Recently Used) replacement
  - Add page reference and dirty bit tracking
  - Implement efficient page selection for eviction

### TLB Management
- [ ] **TLB Implementation**
  - Implement TLB miss handling
  - Add TLB entry insertion and replacement
  - Implement TLB invalidation on context switch
  - Optimize TLB lookup performance

- [ ] **Memory Statistics**
  - Track page fault frequency
  - Monitor TLB hit/miss ratios
  - Collect memory usage statistics
  - Implement performance measurement tools

### Advanced Features
- [ ] **Swap Space Management**
  - Implement swap file creation and management
  - Add page swapping to/from disk
  - Implement swap space allocation algorithms
  - Handle swap space optimization

- [ ] **Memory Mapping**
  - Implement memory-mapped files
  - Add shared memory support
  - Handle copy-on-write semantics
  - Implement memory protection mechanisms

### Testing
- [ ] Test page fault handling with large programs
- [ ] Verify page replacement algorithm correctness
- [ ] Test TLB performance under various workloads
- [ ] Benchmark memory system performance
- [ ] Test memory isolation and protection

### Files to Modify
- `code/userprog/addrspace.h` and `addrspace.cc`
- `code/machine/translate.h` and `translate.cc`
- `code/userprog/exception.cc`
- `code/vm/` directory (if exists)
- `code/machine/machine.h` and `machine.cc`

## Build and Run
```bash
cd NachOS-4.0_MP3
make run
# Inside container:
bash build_nachos.sh -j
make clean; make <test>
../build.linux/nachos -x <test>
```

## Test Programs
- `matmult.c`: Matrix multiplication (memory intensive)
- `sort.c`: Sorting algorithm (various memory patterns)
- `LotOfAdd.c`: Large computation test
- Custom programs to stress virtual memory system

## Key Concepts
- Virtual memory address translation
- Demand paging and page fault handling
- Page replacement algorithms and policies
- Translation Lookaside Buffer (TLB) management
- Memory hierarchy and performance optimization
- Swap space and secondary storage management