# MP4: NachOS File System Implementation

## Purpose
This project implements a complete file system in NachOS, including directory structures, file allocation methods, and file system reliability features. Students will understand how modern file systems organize and manage persistent storage.

## Key Objectives
- Implement hierarchical directory structures
- Develop file allocation and storage management
- Add file system journaling and crash recovery
- Implement file system caching and buffering
- Understand file system performance and reliability

## To-Do List

### Directory System
- [ ] **Hierarchical Directories**
  - Implement multi-level directory structure
  - Add support for absolute and relative paths
  - Implement directory creation and deletion
  - Add directory traversal and listing capabilities

- [ ] **Directory Operations**
  - `mkdir`: Create directories
  - `rmdir`: Remove empty directories
  - `ls`: List directory contents
  - `cd`: Change current working directory
  - Path resolution and validation

### File Management
- [ ] **Extended File Operations**
  - Implement file metadata management (size, timestamps, permissions)
  - Add support for larger files (beyond single disk sectors)
  - Implement file truncation and extension
  - Add file seek operations

- [ ] **File Allocation Methods**
  - Implement indexed allocation (indirect blocks)
  - Add support for large files with multiple indirection levels
  - Optimize file block allocation strategies
  - Implement efficient free space management

### File System Reliability
- [ ] **Journaling and Recovery**
  - Implement write-ahead logging for metadata changes
  - Add crash recovery mechanisms
  - Implement atomic operations for file system updates
  - Add file system consistency checking

- [ ] **Caching and Buffering**
  - Implement buffer cache for disk blocks
  - Add write-back and write-through caching policies
  - Implement LRU replacement for cache blocks
  - Optimize I/O performance through buffering

### Advanced Features
- [ ] **File System Performance**
  - Implement read-ahead mechanisms
  - Add support for asynchronous I/O operations
  - Optimize directory lookup performance
  - Implement file system defragmentation

- [ ] **File System Security**
  - Add file permission system (read/write/execute)
  - Implement user and group ownership
  - Add access control mechanisms
  - Implement secure file deletion

### Testing
- [ ] Test directory creation and navigation
- [ ] Verify file operations under various conditions
- [ ] Test file system recovery after simulated crashes
- [ ] Benchmark file system performance
- [ ] Test concurrent file system access

### Files to Modify
- `code/filesys/filesys.h` and `filesys.cc`
- `code/filesys/directory.h` and `directory.cc`
- `code/filesys/filehdr.h` and `filehdr.cc`
- `code/machine/disk.h` and `disk.cc`
- `code/filesys/synchdisk.h` and `synchdisk.cc`

## Build and Run
```bash
cd NachOS-4.0_MP4
make run
# Inside container:
bash build_nachos.sh -j
make clean; make <test>
../build.linux/nachos -f -cp <test> <test>
../build.linux/nachos -x <test>
```

## Test Programs
- Directory operations: Create nested directories
- Large file operations: Test with files larger than single blocks
- Concurrent access: Multiple processes accessing files
- Crash simulation: Test recovery mechanisms
- Performance benchmarks: File I/O intensive operations

## Key Concepts
- File system organization and metadata
- Directory structures and path resolution
- File allocation methods (contiguous, linked, indexed)
- File system journaling and crash recovery
- Buffer management and caching strategies
- File system performance optimization