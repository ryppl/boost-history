//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHO_PTHREAD_MUTEX_HPP
#define BOOST_SYNCHO_PTHREAD_MUTEX_HPP
#include <iostream>

//-----------------------------------------------------------------------------
#include <pthread.h>
#include <assert.h>
//-----------------------------------------------------------------------------
#include <boost/synchro/detail/config.hpp>
//-----------------------------------------------------------------------------
#include <boost/synchro/exceptions.hpp>
#include <boost/synchro/time.hpp>
#include <boost/synchro/lockable.hpp>
#include <boost/synchro/call_context.hpp>
#include <boost/thread.hpp>
//-----------------------------------------------------------------------------


#define BOOST_SYNCHRO_CHECK

#ifdef BOOST_SYNCHRO_CHECK
#define BOOST_STM_ASSERT(expr) assert(expr)

# define BOOST_STM_VERIFY(expr) BOOST_STM_ASSERT(expr)

#else

#define BOOST_STM_ASSERT(expr) ((void)0)
#define BOOST_STM_VERIFY(expr) ((void)(expr))
#endif
namespace boost { namespace synchro {
    
#ifdef BOOST_STM_USES_CALL_CONTEXT
    inline void trace_lock(int res, pthread_mutex_t& lockable,call_context* cctx=0, const char* name=0) {
        if (cctx==0) return;
        if (name==0) name="";
        boost::lock_guard<boost::mutex> lk(log_mutex);
        if (res!=0)
            std::cout << cctx->that << " : " << std::dec << boost::this_thread::get_id() 
                << " [[[[[ERROR[ "<< res <<"] on lock ["<<name<<"]("<< & lockable <<") "<< cctx->file << "[" << cctx->line << "] " << cctx->fct << std::endl;
        else
            std::cout << cctx->that << " : " << std::dec << boost::this_thread::get_id() 
                << " [[[[[INFO "<< " on lock ["<<name<<"]("<< & lockable <<") "<< cctx->file << "[" << cctx->line << "] " << cctx->fct << std::endl;
        
    }
    inline void trace_unlock(int res, pthread_mutex_t& lockable,call_context* cctx=0, const char* name=0)  {
        if (cctx==0) return;
        if (name==0) name="";
        boost::lock_guard<boost::mutex> lk(log_mutex);
        if (res!=0)
            std::cout << cctx->that << " : " << std::dec << boost::this_thread::get_id() 
                << " ]]]]]ERROR[ "<< res <<"] on unlock ["<<name<<"]("<< & lockable <<") "<< cctx->file << "[" << cctx->line << "] " << cctx->fct << std::endl;
        else
            std::cout << cctx->that << " : " << std::dec << boost::this_thread::get_id() 
                << " ]]]]]]INFO "<< " on unlock ["<<name<<"]("<< & lockable <<") "<< cctx->file << "[" << cctx->line << "] " << cctx->fct << std::endl;
    }
    inline void trace_try_lock(int res, pthread_mutex_t& lockable ,call_context* cctx=0, const char* name=0) {
        if (cctx==0) return;
        if (name==0) name="";
        boost::lock_guard<boost::mutex> lk(log_mutex);
        if (res!=0)
            std::cout << cctx->that << " : " << std::dec << boost::this_thread::get_id() 
                << " [[[[[ERROR[ "<< res <<"] on try_lock ["<<name<<"]("<< & lockable <<") "<< cctx->file << "[" << cctx->line << "] " << cctx->fct << std::endl;
        else
            std::cout << cctx->that << " : " << std::dec << boost::this_thread::get_id() 
                << " [[[[[INFO "<< " on try_lock ["<<name<<"]("<< & lockable <<") "<< cctx->file << "[" << cctx->line << "] " << cctx->fct << std::endl;
    }
    
#define BOOST_STM_TRACE_LOCK(RES,LOCK) trace_lock(RES,LOCK,cctx,name)
#define BOOST_STM_TRACE_UNLOCK(RES,LOCK) trace_unlock(RES,LOCK,cctx,name)
#define BOOST_STM_TRACE_TRY_LOCK(RES,LOCK) trace_try_lock(RES,LOCK,cctx,name)
#else    
#define BOOST_STM_TRACE_LOCK(RES,LOCK) ((void)0)
#define BOOST_STM_TRACE_UNLOCK(RES,LOCK) ((void)0)
#define BOOST_STM_TRACE_TRY_LOCK(RES,LOCK) ((void)0)
#endif    
    template<>
    inline void lock<pthread_mutex_t>(pthread_mutex_t& lockable BOOST_STM_CALL_CONTEXT_2_TMPL_PARAMS()) {
        int res=pthread_mutex_lock(&lockable);
        BOOST_STM_TRACE_LOCK(res,lockable);
        //std::cout << __FILE__<< __LINE__ << " mutex::lock " << & lockable << std::endl;
        BOOST_STM_ASSERT(res==0&&"synchro::lock<pthread_mutex_t>");
        if (res!=0) throw lock_error();
    }

    template<>
    inline void unlock<pthread_mutex_t>(pthread_mutex_t& lockable BOOST_STM_CALL_CONTEXT_2_TMPL_PARAMS()) {
        int res=pthread_mutex_unlock(&lockable);
        BOOST_STM_TRACE_UNLOCK(res,lockable);
        //std::cout << __FILE__<< __LINE__ << " mutex::unlock " << & lockable << std::endl;
        BOOST_STM_ASSERT(res==0&&"synchro::unlock<pthread_mutex_t>");
        if (res!=0) throw lock_error();
    }

    template<>
    inline bool try_lock<pthread_mutex_t>(pthread_mutex_t& lockable BOOST_STM_CALL_CONTEXT_2_TMPL_PARAMS()) {
        int const res=pthread_mutex_trylock(&lockable);
        BOOST_STM_TRACE_TRY_LOCK(res,lockable);
        //BOOST_STM_ASSERT(!res || res==EBUSY);
        return (!res);
    }

#ifdef BOOST_PTHREAD_HAS_TIMEDLOCK
    template<>
    inline bool lock_until<pthread_mutex_t>(
                pthread_mutex_t& lockable, system_time const& abs_time) {
        struct timespec const timeout=detail::get_timespec(abs_time);
        int const res=pthread_mutex_timedlock(&lockable,&timeout);
        BOOST_STM_VERIFY(!res || res==ETIMEDOUT);
        return !res;
    }   
#endif

    
}}
#endif // BOOST_SYNCHO_TRY_LOCK_HPP


