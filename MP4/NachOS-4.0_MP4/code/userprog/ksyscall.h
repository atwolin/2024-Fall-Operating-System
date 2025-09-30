/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"

#include "synchconsole.h"

void SysHalt()
{
	kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
	return op1 + op2;
}

#ifdef FILESYS_STUB
int SysCreate(char *filename)
{
	// return value
	// 1: success
	// 0: failed
	return kernel->interrupt->CreateFile(filename);
}
#endif
/* MP4 */
int SysCreate(char *filename, int size) {
    // return value
    // 1: success
    // 0: failed
    return kernel->fileSystem->Create(filename, size);
}

OpenFileId SysOpen(char *name) { return kernel->fileSystem->IdOpen(name); }

int SysRaed(char *name, int size, OpenFileId id) {
    return kernel->fileSystem->Read(name, size, id);
}

int SysWrite(char *name, int size, OpenFileId id) {
    return kernel->fileSystem->Write(name, size, id);
}

int SysClose(OpenFileId id) { return kernel->fileSystem->Close(id); }
#endif /* ! __USERPROG_KSYSCALL_H__ */
