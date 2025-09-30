# Pthread Programming Assignment

## Purpose
This project focuses on multi-threaded programming using POSIX threads (pthreads). Students will implement a producer-consumer pipeline system with thread synchronization, load balancing, and performance optimization using mutex locks, condition variables, and thread-safe data structures.

## Key Objectives
- Master pthread programming and thread management
- Implement thread-safe data structures and synchronization primitives
- Develop producer-consumer patterns with multiple stages
- Implement dynamic load balancing and performance monitoring
- Understand thread synchronization and coordination mechanisms

## To-Do List

### Core Implementation
- [ ] **Thread-Safe Queue (TSQueue)**
  - ✅ Implement circular buffer with thread safety
  - ✅ Add mutex locks and condition variables
  - ✅ Implement blocking enqueue/dequeue operations
  - ✅ Handle buffer overflow and underflow conditions

- [ ] **Producer-Consumer Pipeline**
  - ✅ Implement `Reader` class for input processing
  - ✅ Implement `Producer` classes for data transformation
  - ✅ Implement `Writer` class for output generation
  - ✅ Coordinate pipeline stages with proper synchronization

### Advanced Features
- [ ] **Consumer Controller**
  - Implement dynamic thread management
  - Add load balancing based on queue sizes
  - Implement performance monitoring and adjustment
  - Add configurable thresholds for scaling

- [ ] **Transformer Engine**
  - ✅ Implement data transformation logic
  - Add support for various transformation operations
  - Optimize transformation performance
  - Handle different data types and formats

### Performance Optimization
- [ ] **Thread Management**
  - Optimize number of producer threads
  - Implement efficient thread creation and destruction
  - Add thread pool management
  - Monitor and adjust thread performance

- [ ] **Synchronization Efficiency**
  - Minimize lock contention
  - Optimize condition variable usage
  - Implement efficient signaling mechanisms
  - Reduce unnecessary context switching

### Testing and Validation
- [ ] **Correctness Testing**
  - Test with various input sizes and patterns
  - Verify output correctness under concurrent access
  - Test thread synchronization under heavy loads
  - Validate pipeline integrity and data consistency

- [ ] **Performance Benchmarking**
  - Measure throughput under different configurations
  - Analyze bottlenecks in the pipeline
  - Compare performance with different thread counts
  - Test scalability with varying workloads

### Configuration Options
- [ ] **Adjustable Parameters**
  - Queue sizes (reader, worker, writer)
  - Thread counts for each stage
  - Consumer controller thresholds
  - Monitoring and adjustment periods

## Project Structure
```
NTHU-OS-Pthreads/
├── ts_queue.hpp           # Thread-safe queue implementation
├── producer.hpp           # Producer thread class
├── consumer_controller.hpp # Dynamic load balancer
├── reader.hpp             # Input reader thread
├── writer.hpp             # Output writer thread
├── transformer.hpp        # Data transformation engine
├── item.hpp              # Data item structure
├── thread.hpp            # Thread base class
├── main.cpp              # Main pipeline coordinator
└── tests/                # Test cases and benchmarks
```

## Build and Run
```bash
cd NTHU-OS-Pthreads
make clean && make
./main <n> <input_file> <output_file>

# Example:
./main 1000 input.txt output.txt
```

## Test Cases
- `ex00.txt` through `ex10.txt`: Various test scenarios
- Small and large input files (num_100.txt, num_1000.txt)
- Performance benchmarks with different configurations
- Stress testing with high concurrency

## Key Concepts
- **Thread Synchronization**: Mutex locks, condition variables, semaphores
- **Producer-Consumer Pattern**: Multi-stage pipeline processing
- **Thread-Safe Programming**: Race condition prevention, atomic operations
- **Load Balancing**: Dynamic thread management based on queue occupancy
- **Performance Optimization**: Minimizing contention, efficient resource usage
- **Concurrent Data Structures**: Thread-safe queues, shared resources

## Learning Outcomes
- Proficiency in pthread programming
- Understanding of thread synchronization mechanisms
- Experience with concurrent system design
- Knowledge of performance optimization techniques
- Skills in debugging multi-threaded applications