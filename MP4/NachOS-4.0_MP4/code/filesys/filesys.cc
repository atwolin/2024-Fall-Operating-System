// filesys.cc
//	Routines to manage the overall operation of the file system.
//	Implements routines to map from textual file names to files.
//
//	Each file in the file system has:
//	   A file header, stored in a sector on disk
//		(the size of the file header data structure is arranged
//		to be precisely the size of 1 disk sector)
//	   A number of data blocks
//	   An entry in the file system directory
//
// 	The file system consists of several data structures:
//	   A bitmap of free disk sectors (cf. bitmap.h)
//	   A directory of file names and file headers
//
//      Both the bitmap and the directory are represented as normal
//	files.  Their file headers are located in specific sectors
//	(sector 0 and sector 1), so that the file system can find them
//	on bootup.
//
//	The file system assumes that the bitmap and directory files are
//	kept "open" continuously while Nachos is running.
//
//	For those operations (such as Create, Remove) that modify the
//	directory and/or bitmap, if the operation succeeds, the changes
//	are written immediately back to disk (the two files are kept
//	open during all this time).  If the operation fails, and we have
//	modified part of the directory and/or bitmap, we simply discard
//	the changed version, without writing it back to disk.
//
// 	Our implementation at this point has the following restrictions:
//
//	   there is no synchronization for concurrent accesses
//	   files have a fixed size, set when the file is created
//	   files cannot be bigger than about 3KB in size
//	   there is no hierarchical directory structure, and only a limited
//	     number of files can be added to the system
//	   there is no attempt to make the system robust to failures
//	    (if Nachos exits in the middle of an operation that modifies
//	    the file system, it may corrupt the disk)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.
#ifndef FILESYS_STUB
#include <string.h>

#include "filesys.h"

#include "copyright.h"
#include "debug.h"
#include "directory.h"
#include "disk.h"
#include "filehdr.h"
#include "pbitmap.h"



// Sectors containing the file headers for the bitmap of free sectors,
// and the directory of files.  These file headers are placed in well-known
// sectors, so that they can be located on boot-up.
#define FreeMapSector 0
#define DirectorySector 1

// Initial file sizes for the bitmap and directory; until the file system
// supports extensible files, the directory size sets the maximum number
// of files that can be loaded onto the disk.
#define FreeMapFileSize (NumSectors / BitsInByte)
// #define NumDirEntries 10
#define NumDirEntries 64
#define DirectoryFileSize (sizeof(DirectoryEntry) * NumDirEntries)

//----------------------------------------------------------------------
// FileSystem::FileSystem
// 	Initialize the file system.  If format = TRUE, the disk has
//	nothing on it, and we need to initialize the disk to contain
//	an empty directory, and a bitmap of free sectors (with almost but
//	not all of the sectors marked as free).
//
//	If format = FALSE, we just have to open the files
//	representing the bitmap and the directory.
//
//	"format" -- should we initialize the disk?
//----------------------------------------------------------------------

FileSystem::FileSystem(bool format) {
    DEBUG(dbgFile, "Initializing the file system.");
    if (format) {  // -f
        PersistentBitmap *freeMap = new PersistentBitmap(NumSectors);
        Directory *directory = new Directory(NumDirEntries);
        FileHeader *mapHdr = new FileHeader;
        FileHeader *dirHdr = new FileHeader;

        DEBUG(dbgFile, "Formatting the file system.");

        // First, allocate space for FileHeaders for the directory and bitmap
        // (make sure no one else grabs these!)
        freeMap->Mark(FreeMapSector);
        freeMap->Mark(DirectorySector);

        // Second, allocate space for the data blocks containing the contents
        // of the directory and bitmap files.  There better be enough space!

        ASSERT(mapHdr->Allocate(freeMap, FreeMapFileSize));
        ASSERT(dirHdr->Allocate(freeMap, DirectoryFileSize));

        // Flush the bitmap and directory FileHeaders back to disk
        // We need to do this before we can "Open" the file, since open
        // reads the file header off of disk (and currently the disk has garbage
        // on it!).

        DEBUG(dbgFile, "Writing headers back to disk.");
        mapHdr->WriteBack(FreeMapSector);
        dirHdr->WriteBack(DirectorySector);

        // OK to open the bitmap and directory files now
        // The file system operations assume these two files are left open
        // while Nachos is running.

        freeMapFile = new OpenFile(FreeMapSector);
        directoryFile = new OpenFile(DirectorySector);
        openedFile = NULL;

        // Once we have the files "open", we can write the initial version
        // of each file back to disk.  The directory at this point is completely
        // empty; but the bitmap has been changed to reflect the fact that
        // sectors on the disk have been allocated for the file headers and
        // to hold the file data for the directory and bitmap.

        DEBUG(dbgFile, "Writing bitmap and directory back to disk.");
        freeMap->WriteBack(freeMapFile);  // flush changes to disk
        DEBUG(dbgFile, "^V^");
        directory->WriteBack(directoryFile);


        if (debug->IsEnabled('f')) {
            freeMap->Print();
            directory->Print();
        }
        delete freeMap;
        delete directory;
        delete mapHdr;
        delete dirHdr;
    } else {
        // if we are not formatting the disk, just open the files representing
        // the bitmap and directory; these are left open while Nachos is running
        freeMapFile = new OpenFile(FreeMapSector);
        directoryFile = new OpenFile(DirectorySector);
        openedFile = NULL;
    }
}

//----------------------------------------------------------------------
// MP4 mod tag
// FileSystem::~FileSystem
//----------------------------------------------------------------------
FileSystem::~FileSystem() {
    delete freeMapFile;
    delete directoryFile;
}

//----------------------------------------------------------------------
// FileSystem::Create
// 	Create a file in the Nachos file system (similar to UNIX create).
//	Since we can't increase the size of files dynamically, we have
//	to give Create the initial size of the file.
//
//	The steps to create a file are:
//	  Make sure the file doesn't already exist
//        Allocate a sector for the file header
// 	  Allocate space on disk for the data blocks for the file
//	  Add the name to the directory
//	  Store the new file header on disk
//	  Flush the changes to the bitmap and the directory back to disk
//
//	Return TRUE if everything goes ok, otherwise, return FALSE.
//
// 	Create fails if:
//   		file is already in directory
//	 	no free space for file header
//	 	no free entry for file in directory
//	 	no free space for data blocks for the file
//
// 	Note that this implementation assumes there is no concurrent access
//	to the file system!
//
//	"name" -- name of file to be created
//	"initialSize" -- size of file to be created
//----------------------------------------------------------------------

bool FileSystem::Create(char *name, int initialSize) {
    DEBUG(dbgFile, "In FileSystem::Create()");
    Directory *directory;
    PersistentBitmap *freeMap;
    FileHeader *hdr;
    int sector;
    bool success;
    char *fileName;
    char *copyName;
    char *token;
    OpenFile *dirFile = directoryFile;

    // DEBUG(dbgFile, "Before capturing fileName: " << name);
    directory = new Directory(NumDirEntries);
    directory->FetchFrom(directoryFile);

    // Get file
    copyName = new char[strlen(name)];
    strcpy(copyName, name);
    token = strtok(copyName, "/");
    while (token != NULL) {
        sector = directory->Find(token);
        DEBUG(dbgFile,"    In FileSystem::Create(), token: " << token << ", found secotr: " << sector);

        // Check non exited file or directory
        if (sector == -1) {
            DEBUG(dbgFile, "    0L0 token: " << token);
            fileName = token;
            break;
        }
        dirFile = new OpenFile(sector);
        directory->FetchFrom(dirFile);
        token = strtok(NULL, "/");
    }

    // Start creating
    if (directory->Find(fileName) != -1) {
        DEBUG(dbgFile, "file is already in directory");
        success = FALSE;  // file is already in directory
    } else {
        freeMap = new PersistentBitmap(freeMapFile, NumSectors);
        sector =
            freeMap->FindAndSet();  // find a sector to hold the file header
        if (sector == -1) {
            DEBUG(dbgFile, "no free block for file header");
            success = FALSE;  // no free block for file header
        }
        else if (!directory->Add(fileName, sector, TRUE)) {
            DEBUG(dbgFile, "no space in directory");
            success = FALSE;  // no space in directory
        }
        else {
            hdr = new FileHeader;
            if (!hdr->Allocate(freeMap, initialSize)) {
                DEBUG(dbgFile, "no space on disk for data");
                success = FALSE;  // no space on disk for data
            }
            else {
                success = TRUE;
                // everthing worked, flush all changes back to disk
                DEBUG(dbgFile, "In FileSystem::Create(), call WriteBack()");
                hdr->WriteBack(sector);
                // directory->WriteBack(directoryFile);
                // ===== MP4 =====
                directory->WriteBack(dirFile);
                // ===== MP4 =====
                freeMap->WriteBack(freeMapFile);
                DEBUG(dbgFile, "In FileSystem::Create(), end WriteBack()");
            }
            // ===== Bonus II =====
            // hdr->Print();
            // ===== Bonus II =====
            delete hdr;
        }
        delete freeMap;
        DEBUG(dbgFile, "    >>> To create file: " << fileName << ", in directory: " << directory << ", sector=" << sector);
    }
    delete directory;
    return success;
}

//----------------------------------------------------------------------
// FileSystem::CreateDir()
//----------------------------------------------------------------------
bool FileSystem::CreateDir(char *name) {
    DEBUG(dbgFile, "In FileSystem::CreateDir()");
    Directory *directory;
    PersistentBitmap *freeMap;
    FileHeader *hdr;
    int sector;
    bool success;
    char *dirName;
    char *copyName;
    char *token;
    OpenFile *dirFile = directoryFile;

    directory = new Directory(NumDirEntries);
    directory->FetchFrom(directoryFile);

    // Get directory
    copyName = new char[strlen(name)];
    strcpy(copyName, name);
    token = strtok(copyName, "/");
    while (token != NULL) {
        sector = directory->Find(token);
        DEBUG(dbgFile,"    In FileSystem::CreateDir(), token: " << token << ", found secotr: " << sector);

        // Check non existed file or directory
        if (sector == -1) {
            DEBUG(dbgFile, "    0L0 token: " << token);
            dirName = token;
            break;
        }
        dirFile = new OpenFile(sector);
        directory->FetchFrom(dirFile);
        token = strtok(NULL, "/");
    }

    // Start creating
    if (directory->Find(dirName) != -1) {
        DEBUG(dbgFile, "directory is already in directory");
        success = FALSE;  // file is already in directory
    } else {
        freeMap = new PersistentBitmap(freeMapFile, NumSectors);
        sector =
            freeMap->FindAndSet();  // find a sector to hold the file header
        if (sector == -1) {
            DEBUG(dbgFile, "no free block for file header");
            success = FALSE;  // no free block for file header
        }
        else if (!directory->Add(dirName, sector, FALSE)) {
            DEBUG(dbgFile, "no space in directory");
            success = FALSE;  // no space in directory
        }
        else {
            hdr = new FileHeader;
            // if (!hdr->Allocate(freeMap, initialSize)) {
            if (!hdr->Allocate(freeMap, DirectoryFileSize)) {
                DEBUG(dbgFile, "no space on disk for data");
                success = FALSE;  // no space on disk for data
            }
            else {
                success = TRUE;
                // everthing worked, flush all changes back to disk
                DEBUG(dbgFile, "In FileSystem::CreateDir(), call WriteBack()");
                hdr->WriteBack(sector);
                // directory->WriteBack(directoryFile);
                // ===== MP4 =====
                directory->WriteBack(dirFile);
                // ===== MP4 =====
                freeMap->WriteBack(freeMapFile);
                DEBUG(dbgFile, "In FileSystem::CreateDir(), end WriteBack()");
            }
            delete hdr;
        }
        delete freeMap;
        DEBUG(dbgFile, "    >>> To create directory: " << dirName << ", in directory: " << directory << ", sector=" << sector);
    }
    delete directory;
    return success;
}

//----------------------------------------------------------------------
// FileSystem::Open
// 	Open a file for reading and writing.
//	To open a file:
//	  Find the location of the file's header, using the directory
//	  Bring the header into memory
//
//	"name" -- the text name of the file to be opened
//----------------------------------------------------------------------

OpenFile *FileSystem::Open(char *name) {
    Directory *directory = new Directory(NumDirEntries);
    OpenFile *openFile = NULL;
    int sector;
    char *fileName;
    char *copyName;
    char *token;

    directory->FetchFrom(directoryFile);

    DEBUG(dbgFile, "    Path: " << name);
    // Get directory
    copyName = new char[strlen(name)];
    strcpy(copyName, name);
    token = strtok(copyName, "/");
    while (1) {
        sector = directory->Find(token);
        DEBUG(dbgFile, "    In FileSystem::Open(), token: " << token << ", found secotr: " << sector);

        // Check exited file or directory
        fileName = token;
        if((token = strtok(NULL, "/")) == NULL){
            break;
        }
        OpenFile *dirFile = new OpenFile(sector);
        directory->FetchFrom(dirFile);
    }

    // Get file secoter
    sector = directory->Find(fileName);
    DEBUG(dbgFile, "To open a file: " << fileName << ", in directory: " << directory << ", in sector=" << sector);

    if (sector >= 0)
        openFile = new OpenFile(sector);  // name was found in directory
    delete directory;

    return openFile;  // return NULL if not found
}

//----------------------------------------------------------------------
// FileSystem::IdOpen
//----------------------------------------------------------------------
OpenFileId FileSystem::IdOpen(char *name) {
    openedFile = Open(name);
    if (openedFile != NULL) {
        return 1;
    } else {
        return -1;
    }
}

//----------------------------------------------------------------------
// FileSystem::Read
//----------------------------------------------------------------------
int FileSystem::Read(char *buf, int size, OpenFileId id) {
    if (openedFile != NULL && id != -1) {
        DEBUG(dbgFile, "In FileSystem::Read, into OpenFile::Read()");
        int val = openedFile->Read(buf, size);
        DEBUG(dbgFile, "In FileSystem::Read, return from OpenFile::Read()");
        return val;
        // return openedFile->Read(buf, size);
    } else {
        return 0;
    }
}

//----------------------------------------------------------------------
// FileSystem::Write
//----------------------------------------------------------------------
int FileSystem::Write(char *buf, int size, OpenFileId id) {
    if (openedFile != NULL && id != -1) {
        return openedFile->Write(buf, size);
    } else {
        return 0;
    }
}

//----------------------------------------------------------------------
// FileSystem::Close
//----------------------------------------------------------------------
int FileSystem::Close(OpenFileId id) {
    delete openedFile;
    DEBUG(dbgFile,"In FileSystem::Close, after delete openedFile: " << openedFile);
    return 1;
}

//----------------------------------------------------------------------
// FileSystem::Remove
// 	Delete a file from the file system.  This requires:
//	    Remove it from the directory
//	    Delete the space for its header
//	    Delete the space for its data blocks
//	    Write changes to directory, bitmap back to disk
//
//	Return TRUE if the file was deleted, FALSE if the file wasn't
//	in the file system.
//
//	"name" -- the text name of the file to be removed
//----------------------------------------------------------------------

bool FileSystem::Remove(char *name) {
    Directory *directory;
    PersistentBitmap *freeMap;
    FileHeader *fileHdr;
    int sector;
    char *fileName;
    char *copyName;
    char *token;
    OpenFile *dirFile = directoryFile;

    directory = new Directory(NumDirEntries);
    directory->FetchFrom(directoryFile);

    // Get directory
    copyName = new char[strlen(name)];
    strcpy(copyName, name);
    token = strtok(copyName, "/");
    while (1) {
        sector = directory->Find(token);
        DEBUG(dbgFile,"    In FileSystem::Remove(), token: " << token << ", found secotr: " << sector);

        // Check existed file or directory
        fileName = token;
        if((token = strtok(NULL, "/")) == NULL){
            break;
        }
        dirFile = new OpenFile(sector);  // to read content
        directory->FetchFrom(dirFile);
    }
    // DEBUG(dbgFile, "    >>> To remove file: " << fileName << ", directory: " << directory << ", in sector=" << sector);

    // Get file sector
    sector = directory->Find(fileName);
    DEBUG(dbgFile, "    >>> To remove file: " << fileName << ", in directory: " << directory << ", in sector=" << sector);
    if (sector == -1) {
        delete directory;
        return FALSE;  // file not found
    }
    fileHdr = new FileHeader;
    fileHdr->FetchFrom(sector);

    freeMap = new PersistentBitmap(freeMapFile, NumSectors);

    fileHdr->Deallocate(freeMap);  // remove data blocks
    freeMap->Clear(sector);        // remove header block
    DEBUG(dbgFile, ":O");
    directory->Remove(fileName);

    freeMap->WriteBack(freeMapFile);      // flush to disk
    // directory->WriteBack(directoryFile);  // flush to disk
    // ===== MP4 =====
    directory->WriteBack(dirFile);
    // ===== MP4 =====
    delete fileHdr;
    delete directory;
    delete freeMap;
    return TRUE;
}

//----------------------------------------------------------------------
// FileSystem::List
// 	List all the files in the file system directory.
//----------------------------------------------------------------------
void FileSystem::List(char *name, bool isRecursive) {
    char *dirName;
    char *copyName;
    char *token;

    Directory *directory = new Directory(NumDirEntries);
    directory->FetchFrom(directoryFile);

    // Get directory
    copyName = new char[strlen(name)];
    strcpy(copyName, name);
    token = strtok(copyName, "/");
    while (token != NULL) {
        int sector = directory->Find(token);
        DEBUG(dbgFile,"    In FileSystem::List(), token: " << token << ", found secotr: " << sector);

        // Check exited directory
        dirName = token;

        OpenFile *dirFile = new OpenFile(sector);  // to read content
        directory->FetchFrom(dirFile);
        if((token = strtok(NULL, "/")) == NULL){
            break;
        }
        DEBUG(dbgFile,"PPP");
    }

    DEBUG(dbgFile,"    >>> In FileSystem::List(), dirName: " << dirName << ", in directory: " << directory);
    if (isRecursive) {
        directory->RecursiveList(0, NumDirEntries);
    } else {
        directory->List();
    }

    delete directory;
}

//----------------------------------------------------------------------
// FileSystem::Print
// 	Print everything about the file system:
//	  the contents of the bitmap
//	  the contents of the directory
//	  for each file in the directory,
//	      the contents of the file header
//	      the data in the file
//----------------------------------------------------------------------

void FileSystem::Print() {
    FileHeader *bitHdr = new FileHeader;
    FileHeader *dirHdr = new FileHeader;
    PersistentBitmap *freeMap = new PersistentBitmap(freeMapFile, NumSectors);
    Directory *directory = new Directory(NumDirEntries);

    printf("Bit map file header:\n");
    bitHdr->FetchFrom(FreeMapSector);
    bitHdr->Print();

    printf("Directory file header:\n");
    dirHdr->FetchFrom(DirectorySector);
    dirHdr->Print();

    freeMap->Print();

    directory->FetchFrom(directoryFile);
    directory->Print();

    delete bitHdr;
    delete dirHdr;
    delete freeMap;
    delete directory;
}

//----------------------------------------------------------------------
// FileSystem::FilenameCapture()
//----------------------------------------------------------------------
char* FileSystem::FilenameCapture(char* name) {
    char *copyName = new char[strlen(name)];
    strcpy(copyName, name);
    char *token = strtok(copyName, "/");
    // delete copyName;
    return token;
}

//----------------------------------------------------------------------
// FileSystem::Traverse()
//----------------------------------------------------------------------
char* FileSystem::CreateTraverse(Directory* dir, char* name) {
    if (strcmp(name, "/") == 0) {
        return name;
    }
    char *copyName = new char[strlen(name)];
    char *lastName = new char[256];
    int lastSector;
    int sector;
    strcpy(copyName, name);

    char *token = strtok(copyName, "/");

    while (1) {
        sector = dir->Find(token);
        DEBUG(dbgFile,"In FileSystem::CreateTraverse(), token: " << token << ", found secotr: " << sector);

        // Check non exited file or directory
        if (sector == -1) {
            DEBUG(dbgFile, "0L0 token: " << token);
            return token;
        }
        // strcpy(lastName, token);
        // if((token = strtok(NULL, "/")) == NULL){
        //     return lastName;
        // }
        OpenFile *dirFile = new OpenFile(sector);  // to read content
        dir->FetchFrom(dirFile);
    }
}

// root: t0
// t0: d0, d1, f0, f1, f2. [aa]
// d0: d00, f00, f01
// aa:
// d1: d10, d11

// mkdir: t0/aa
// t0's sector
// create a dir named "aa" in t0

// cp: t0/aa/f1
// t0's sector
// aa's sector

#endif  // FILESYS_STUB
