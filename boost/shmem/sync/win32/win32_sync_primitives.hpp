//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_WIN32_SYNC_PRIMITIVES_HPP
#define BOOST_WIN32_SYNC_PRIMITIVES_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <stddef.h>
#  include <boost/detail/interlocked.hpp>
#else
# error "This file can only be included in Windows OS"
#endif

//Some used windows structure predeclaration
extern "C" 
struct _FILETIME;
extern "C" 
struct _SECURITY_ATTRIBUTES;
extern "C" 
struct _SYSTEM_INFO;

//The structures used in Shmem with the
//same binary interface as windows ones
namespace boost {
namespace shmem {

struct filetime_t
{  
   unsigned long  dwLowDateTime;  
   unsigned long  dwHighDateTime;
};

struct security_attributes
{
   unsigned long nLength;
   void *lpSecurityDescriptor;
   int bInheritHandle;
};

struct system_info_t {
    union {
        unsigned long dwOemId;          // Obsolete field...do not use
        struct {
            unsigned short wProcessorArchitecture;
            unsigned short wReserved;
        };
    };
    unsigned long dwPageSize;
    void * lpMinimumApplicationAddress;
    void * lpMaximumApplicationAddress;
    unsigned long * dwActiveProcessorMask;
    unsigned long dwNumberOfProcessors;
    unsigned long dwProcessorType;
    unsigned long dwAllocationGranularity;
    unsigned short wProcessorLevel;
    unsigned short wProcessorRevision;
};

//Some used constants
static const unsigned long infinite_time        = 0xFFFFFFFF;
static void *              invalid_handle       = (void*)(-1);
static const unsigned long error_already_exists = 183L;

static const unsigned long semaphore_all_access = (0x000F0000L)|(0x00100000L)|0x3;

static const unsigned long page_readonly        = 0x02;
static const unsigned long page_readwrite       = 0x04;

static const unsigned long file_map_read        = 0x0004;
static const unsigned long file_map_write       = 0x0002;
static const unsigned long file_map_all_access  = (0x000F0000L) | (0x0001) | 
                                                   file_map_write | file_map_read | 
                                                   (0x0008)| (0x0010);

static const unsigned long file_share_read      = 0x00000001;
static const unsigned long file_share_write     = 0x00000002;

static const unsigned long generic_read         = 0x80000000L;
static const unsigned long generic_write        = 0x40000000L;

static const unsigned long open_existing        = 3;
static const unsigned long wait_object_0        = 0;
static const unsigned long wait_abandoned       = 0x00000080L;
static const unsigned long wait_timeout         = 258L;

#if !defined( BOOST_USE_WINDOWS_H )

//Some windows API declarations
extern "C" __declspec(dllimport) long __stdcall InterlockedExchangeAdd(long volatile *, long);
extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentThreadId();
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);
extern "C" __declspec(dllimport) unsigned int __stdcall GetLastError();
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);
extern "C" __declspec(dllimport) void __stdcall GetSystemTimeAsFileTime(struct _FILETIME*);
extern "C" __declspec(dllimport) void * __stdcall CreateMutexA(struct _SECURITY_ATTRIBUTES*, int, const char *);
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void *, unsigned long);
extern "C" __declspec(dllimport) int __stdcall ReleaseMutex(void *);
extern "C" __declspec(dllimport) int __stdcall UnmapViewOfFile(void *);
extern "C" __declspec(dllimport) void * __stdcall CreateSemaphoreA(struct _SECURITY_ATTRIBUTES*, long, long, const char *);
extern "C" __declspec(dllimport) int __stdcall ReleaseSemaphore(void *, long, long *);
extern "C" __declspec(dllimport) void * __stdcall OpenSemaphoreA(unsigned long, int, const char *);
extern "C" __declspec(dllimport) void * __stdcall CreateFileMappingA (void *, struct _SECURITY_ATTRIBUTES*, unsigned long, unsigned long, unsigned long, const char *);
extern "C" __declspec(dllimport) void * __stdcall MapViewOfFileEx (void *, unsigned long, unsigned long, unsigned long, size_t, void*);
extern "C" __declspec(dllimport) void * __stdcall OpenFileMappingA (unsigned long, int, const char *);
extern "C" __declspec(dllimport) void * __stdcall CreateFileA (const char *, unsigned long, unsigned long, struct _SECURITY_ATTRIBUTES*, unsigned long, unsigned long, void *);
extern "C" __declspec(dllimport) void __stdcall GetSystemInfo (struct _SYSTEM_INFO *);
extern "C" __declspec(dllimport) int __stdcall FlushViewOfFile (void *, size_t);
extern "C" __declspec(dllimport) int __stdcall GetFileSizeEx (void *, __int64 *size);
/*
BOOL GetFileSizeEx(
  HANDLE hFile,
  PLARGE_INTEGER lpFileSize
);
*/

#endif   //#if !defined( BOOST_USE_WINDOWS_H )
static inline long exchange_and_add(long volatile* addend, long value)
{
   return InterlockedExchangeAdd((long*)addend, value);
}

static inline void sched_yield()
{
   Sleep(1);
}

static inline unsigned long current_thread_id()
{
   return GetCurrentThreadId();
}

static inline unsigned int close_handle(void* handle)
{
   return CloseHandle(handle);
}

static inline int get_last_error()
{
   return GetLastError();
}

static inline void get_system_time_as_file_time(filetime_t *filetime)
{
   GetSystemTimeAsFileTime((struct _FILETIME*)filetime);
}

static inline void *create_mutex(const char *name)
{
   return CreateMutexA(0, 0, name);
}

static inline unsigned long wait_for_single_object(void *handle, unsigned long time)
{
   return WaitForSingleObject(handle, time);
}

static inline int release_mutex(void *handle)
{
   return ReleaseMutex(handle);
}

static inline int unmap_view_of_file(void *address)
{
   return UnmapViewOfFile(address);
}

static inline void *create_semaphore(long initialCount, const char *name)
{
   return CreateSemaphoreA(0, initialCount, (long)(((unsigned long)(-1))>>1), name);
}

static inline int release_semaphore(void *handle, long release_count, long *prev_count)
{
   return ReleaseSemaphore(handle, release_count, prev_count);
}

static inline void *open_semaphore(const char *name)
{
   return OpenSemaphoreA(semaphore_all_access, 1, name);
}

static inline void * create_file_mapping (void * handle, unsigned long access, unsigned long high_size, unsigned long low_size, const char * name)
{
   return CreateFileMappingA (handle, 0, access, high_size, low_size, name);
}

static inline void * open_file_mapping (unsigned long access, const char *name)
{
   return OpenFileMappingA (access, 0, name);
}

static inline void *map_view_of_file_ex(void *handle, unsigned long file_access, unsigned long highoffset, unsigned long lowoffset, std::size_t numbytes, void *base_addr)
{
   return MapViewOfFileEx(handle, file_access, highoffset, lowoffset, numbytes, base_addr);
}

static inline void *create_file(const char *name, unsigned long access, unsigned long creation_flags)
{
   return CreateFileA(name, access, file_share_read | file_share_write, 0, creation_flags, 0, 0);
}

static inline void get_system_info(system_info_t *info)
{
   GetSystemInfo((struct _SYSTEM_INFO *)info);
}

static inline int flush_view_of_file(void *base_addr, std::size_t numbytes)
{
   return FlushViewOfFile(base_addr, numbytes);
}

}  //namespace boost 
}  //namespace shmem


#endif BOOST_WIN32_SYNC_PRIMITIVES_HPP