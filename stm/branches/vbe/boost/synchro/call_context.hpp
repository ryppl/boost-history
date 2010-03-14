//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CALL_CTX_HPP
#define BOOST_CALL_CTX_HPP

#include <boost/synchro/detail/config.hpp>


#include <iostream>
//-----------------------------------------------------------------------------
#include <assert.h>
//-----------------------------------------------------------------------------
#include <boost/thread/mutex.hpp>
namespace boost {
extern boost::mutex log_mutex;
}

#ifdef BOOST_STM_USES_CALL_CONTEXT

namespace boost {
struct call_context {
    const void * that;
    const char* file;
    std::size_t line;
    const char* fct;
    call_context(const void *inst, const char* f, std::size_t l, const char* function) 
        : that(inst), file(f), line(l), fct(function) {}
    ~call_context() {}
};
}


#define BOOST_STM_CALL_CONTEXT_DCL_VAR_INST(NAME, INST) boost::call_context NAME(INST, __FILE__, __LINE__, BOOST_CURRENT_FUNCTION)
#define BOOST_STM_CALL_CONTEXT_DCL_VAR(NAME) BOOST_STM_CALL_CONTEXT_DCL_VAR_INST(NAME, this)
#define BOOST_STM_CALL_CONTEXT_DCL_INST(INST) BOOST_STM_CALL_CONTEXT_DCL_VAR_INST(__call_context, INST)
#define BOOST_STM_CALL_CONTEXT_DCL() BOOST_STM_CALL_CONTEXT_DCL_VAR(__call_context)

#define BOOST_STM_CALL_CONTEXT(NAME) ,&__call_context,NAME

#define BOOST_STM_CALL_CONTEXT_2_DCL() call_context* cctx_; const char* name_
#define BOOST_STM_CALL_CONTEXT_2_PARAMS() ,call_context* cctx=0, const char* name=0
#define BOOST_STM_CALL_CONTEXT_2_PARAMS1() call_context* cctx=0, const char* name=0
#define BOOST_STM_CALL_CONTEXT_2_TMPL_PARAMS() ,call_context* cctx, const char* name
#define BOOST_STM_CALL_CONTEXT_2_ACT_PARAMS() ,cctx, name
#define BOOST_STM_CALL_CONTEXT_2_ACT_CTX() ,cctx_, name_

#define BOOST_STM_CALL_CONTEXT_2_INIT() , cctx_(cctx), name_(name)

#else // !BOOST_STM_USES_CALL_CONTEXT

#define BOOST_STM_CALL_CONTEXT_DCL_VAR_INST(NAME, INST) ((void)0)
#define BOOST_STM_CALL_CONTEXT_DCL_VAR(NAME) ((void)0)
#define BOOST_STM_CALL_CONTEXT_DCL_INST(INST) ((void)0)
#define BOOST_STM_CALL_CONTEXT_DCL() ((void)0)


#define BOOST_STM_CALL_CONTEXT(NAME) 

#define BOOST_STM_CALL_CONTEXT_2_DCL() 
#define BOOST_STM_CALL_CONTEXT_2_PARAMS() 
#define BOOST_STM_CALL_CONTEXT_2_PARAMS1()
#define BOOST_STM_CALL_CONTEXT_2_TMPL_PARAMS() 
#define BOOST_STM_CALL_CONTEXT_2_ACT_PARAMS() 
#define BOOST_STM_CALL_CONTEXT_2_ACT_CTX() 
#define BOOST_STM_CALL_CONTEXT_2_INIT()
#endif
#endif // BOOST_CALL_CTX_HPP


