#ifndef BOOST_INTERTHREADS_THREAD_DECORATOR__HPP
#define BOOST_INTERTHREADS_THREAD_DECORATOR__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Roland Schwarz 2006. 
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Extension of the init class of the threadalert library of Roland Schwarz
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
#include <boost/interthreads/detail/decorator_function.hpp>
#include <boost/interthreads/thread_decoration.hpp>

//#define BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {
    inline void decorate();

    template <typename T>
    class basic_thread_decorator {
    public:
        typedef T result_type;

        // copy/constructor operators (functor)
        basic_thread_decorator() {}
        ~basic_thread_decorator() {}
        basic_thread_decorator(const basic_thread_decorator& other) {
            func_ =other.func_;
        }
        
        basic_thread_decorator& operator=(const basic_thread_decorator& other) {
            basic_thread_decorator tmp(other);
            tmp.swap(*this);
            return *this;
        }

#if defined(BOOST_HAS_VARIADIC_TMPL) 
#if defined(BOOST_HAS_RVALUE_REFS)
        template <typename Callable, typename Arg...>
        explicit basic_thread_decorator(Callable&& f, ...Arg&& args)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),static_cast<F&&>(f),...args))) {};
#else
        template <typename Callable, typename Arg...>
        explicit basic_thread_decorator(Callable f, ...Arg args)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),f,...args))) {};
#endif
#else // !defined(BOOST_HAS_VARIADIC_TMPL)
#if defined(BOOST_HAS_RVALUE_REFS)
        template <typename Callable>
        explicit basic_thread_decorator(Callable&& f)
            : func_(detail::make_decorator_function(static_cast<F&&>(f))) {};
#else // !defined(BOOST_HAS_RVALUE_REFS)            
#ifdef BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
#ifndef BOOST_NO_SFINAE
        template <class Callable>
        explicit basic_thread_decorator(Callable f): func_(detail::make_decorator_function(f)) {}
#else // BOOST_NO_SFINAE
        template <class Callable>
        explicit basic_thread_decorator(Callable f
            ,typename disable_if<boost::is_convertible<F&,boost::detail::thread_move_t<F> >, detail::dummy* >::type=0)
            : func_(detail::make_decorator_function(f)) {}
#endif // BOOST_NO_SFINAE
        template <class Callable>
        explicit basic_thread_decorator(boost::detail::thread_move_t<Callable> f):
            func_(detail::make_decorator_function(f)) {}
#else // ! BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
        template <class Callable>
        explicit basic_thread_decorator(Callable f): func_(detail::make_decorator_function(f)) {}
#endif // ! BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
            
                
        template <typename Callable, typename A1>
        basic_thread_decorator(Callable f, A1 a1)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),f,a1))) {};
                
        template <typename Callable, typename A1, typename A2>
        basic_thread_decorator(Callable f, A1 a1, A2 a2)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),f,a1,a2))) {};
                
        template <typename Callable, typename A1, typename A2, typename A3>
        basic_thread_decorator(Callable f, A1 a1, A2 a2, A3 a3)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),f,a1,a2,a3))) {};
                
        template <typename Callable, typename A1, typename A2, typename A3, typename A4>
        basic_thread_decorator(Callable f, A1 a1, A2 a2, A3 a3, A4 a4)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),f,a1,a2,a3,a4))) {};
#endif
#endif                
        // move semantics
#if defined(BOOST_HAS_RVALUE_REFS)
        basic_thread_decorator(basic_thread_decorator&& other) {
            func_.swap(other.func_);
        }
        
        basic_thread_decorator& operator=(basic_thread_decorator&& other) {
            func_=other.func_;
            other.func_.reset();
            return *this;
        }

        basic_thread_decorator&& move() {
            return static_cast<basic_thread_decorator&&>(*this);
        }
        
#else // ! defined(BOOST_HAS_RVALUE_REFS)
#ifdef BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
        basic_thread_decorator(boost::detail::thread_move_t<basic_thread_decorator> x) {
            func_=x->func_;
            x->func_.reset();
        }
        
        basic_thread_decorator& operator=(boost::detail::thread_move_t<basic_thread_decorator> x) {
            basic_thread_decorator new_decorator(x);
            swap(new_decorator);
            return *this;
        }
        
        operator boost::detail::thread_move_t<basic_thread_decorator>() {
            return move();
        }
        
        boost::detail::thread_move_t<basic_thread_decorator> move() {
            boost::detail::thread_move_t<basic_thread_decorator> x(*this);
            return x;
        }
#endif // ! BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE      
#endif // ! defined(BOOST_HAS_RVALUE_REFS)

        void swap(basic_thread_decorator& x) {
            func_.swap(x.func_);
        }     
        T operator()() const {
            decorate();
            return (*func_)();
        }
    private:
        boost::shared_ptr<detail::decorator_function_base<T> > func_;
    };
 
    typedef basic_thread_decorator<void> thread_decorator;
    
    template <class F>
    basic_thread_decorator<typename boost::result_of<F()>::type> make_decorator(F f) {
#ifdef BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
        return move(basic_thread_decorator<typename boost::result_of<F()>::type>(f));
#else
        return basic_thread_decorator<typename boost::result_of<F()>::type>(f);
#endif        
    }
   
    template <class F>
#ifdef BOOST_HAS_RVALUE_REFS
    thread&& 
#else
    boost::detail::thread_move_t<thread> 
#endif        
    create_decorated_thread(F f) {
        return thread(make_decorator(f));
    }   

    inline void decorate() {thread_decoration::decorate();}
     
}
}

#include <boost/config/abi_suffix.hpp>


#endif

