// ----------------------------------------------------------------------------
// Copyright (C) 2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROFILER_DETAIL_MSVC_SEMAPHORE_HPP_INCLUDED
#define BOOST_PROFILER_DETAIL_MSVC_SEMAPHORE_HPP_INCLUDED

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOGDICAPMASKS     
#define NOVIRTUALKEYCODES 
#define NOWINMESSAGES     
#define NOWINSTYLES       
#define NOSYSMETRICS      
#define NOMENUS           
#define NOICONS           
#define NOKEYSTATES       
#define NOSYSCOMMANDS     
#define NORASTEROPS       
#define NOSHOWWINDOW      
#define OEMRESOURCE       
#define NOATOM            
#define NOCLIPBOARD       
#define NOCOLOR           
#define NOCTLMGR          
#define NODRAWTEXT        
#define NOGDI             
#define NOKERNEL          
#define NOUSER            
#define NONLS             
#define NOMB              
#define NOMEMMGR          
#define NOMETAFILE        
#define NOMINMAX          
#define NOMSG             
#define NOOPENFILE        
#define NOSCROLL          
#define NOSERVICE         
#define NOSOUND           
#define NOTEXTMETRIC      
#define NOWH              
#define NOWINOFFSETS      
#define NOCOMM            
#define NOKANJI           
#define NOHELP            
#define NOPROFILER        
#define NODEFERWINDOWPOS  
#define NOMCX             
#include <windows.h>

namespace boost { namespace profiler { namespace detail 
{

    typedef volatile LONG semaphore_t;
    
    inline void acquire_semaphore(semaphore_t &semaphore)
    {
        while (InterlockedCompareExchange(&semaphore, 0, 1) == 0)
            Sleep(0);
    }

    inline void release_semaphore(semaphore_t &semaphore)
    {
        InterlockedExchange(&semaphore, 1);
    }

} } }

#endif
