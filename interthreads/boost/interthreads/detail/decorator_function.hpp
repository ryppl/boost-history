#ifndef BOOST_INTERTHREADS_DECORATION_FUNCTION__HPP
#define BOOST_INTERTHREADS_DECORATION_FUNCTION__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interthreads/detail/config.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/utility/result_of.hpp>

//#define BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {
    namespace detail {
        template <typename T>
        struct decorator_function_base {
            virtual ~decorator_function_base() {}
            virtual T operator()() const=0;
        };
        
        template<typename F>
        struct BOOST_INTERTHREADS_DECL decorator_function : decorator_function_base<typename boost::result_of<F()>::type> {
            F f;
            
#ifdef BOOST_INTERTHREADS_THREAD_DECORATION_MOVE
#ifdef BOOST_HAS_RVALUE_REFS
            decorator_function(F&& f_):
                f(static_cast<F&&>(f_))
            {}
#else
            decorator_function(F f_): f(f_) {}
            decorator_function(boost::detail::thread_move_t<F> f_): f(f_) {}
#endif                
#else                
            decorator_function(F f_): f(f_) {}
#endif            
            
            typename boost::result_of<F()>::type operator()() const {
                return f();
            }
        private:
            decorator_function();
                
        };
        
#ifdef BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
#ifdef BOOST_HAS_RVALUE_REFS
        template<typename F>
        static inline boost::shared_ptr<decorator_function_base<typename boost::result_of<F()>::type> > make_decorator_function(F&& f)
        {
            return boost::shared_ptr<decorator_function_base>(
                new decorator_function<typename boost::remove_reference<F>::type> >(static_cast<F&&>(f)));
        }
        static inline boost::shared_ptr<decorator_function_base<typename boost::result_of<F()>::type> > make_decorator_function(void (*f)())
        {
            return boost::shared_ptr<decorator_function_base<typename boost::result_of<F()>::type> >(new decorator_function<void(*)()> >(f));
        }
#else
        struct dummy;

        template<typename F>
        static inline boost::shared_ptr<decorator_function_base<typename boost::result_of<F()>::type> > make_decorator_function(F f)
        {
            return boost::shared_ptr<decorator_function_base<typename boost::result_of<F()>::type> >(new decorator_function<F>(f));
        }
        template<typename F>
        static inline boost::shared_ptr<decorator_function_base<typename boost::result_of<F()>::type> > make_decorator_function(boost::detail::thread_move_t<F> f)
        {
            return boost::shared_ptr<decorator_function_base<typename boost::result_of<F()>::type> >(new decorator_function<F>(f));
        }
#endif        
#else
        template<typename F>
        static inline boost::shared_ptr<decorator_function_base<typename boost::result_of<F()>::type> > make_decorator_function(F f)
        {
            return boost::shared_ptr<decorator_function_base<typename boost::result_of<F()>::type> >(new decorator_function<F>(f));
        }
        
#endif      
    }

     
}
}

#include <boost/config/abi_suffix.hpp>


#endif

