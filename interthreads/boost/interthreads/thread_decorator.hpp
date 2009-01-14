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

//#define BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {
    
    namespace detail {
        struct decorator_function_base {
            virtual ~decorator_function_base() {}
            virtual void operator()() const=0;
        };
        
        template<typename F>
        struct BOOST_INTERTHREADS_DECL decorator_function : decorator_function_base {
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
            
            void operator()() const {
                f();
            }
        private:
//            void operator=(decorator_function&);
            decorator_function();
//            decorator_function(decorator_function&);
                
        };
        
#ifdef BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
#ifdef BOOST_HAS_RVALUE_REFS
        template<typename F>
        static inline boost::shared_ptr<decorator_function_base> make_decorator_function(F&& f)
        {
            return boost::shared_ptr<decorator_function_base>(
                new decorator_function<typename boost::remove_reference<F>::type> >(static_cast<F&&>(f)));
        }
        static inline boost::shared_ptr<decorator_function_base> make_decorator_function(void (*f)())
        {
            return boost::shared_ptr<decorator_function_base>(new decorator_function<void(*)()> >(f));
        }
#else
        struct dummy;

        template<typename F>
        static inline boost::shared_ptr<decorator_function_base> make_decorator_function(F f)
        {
            return boost::shared_ptr<decorator_function_base>(new decorator_function<F>(f));
        }
        template<typename F>
        static inline boost::shared_ptr<decorator_function_base> make_decorator_function(boost::detail::thread_move_t<F> f)
        {
            return boost::shared_ptr<decorator_function_base>(new decorator_function<F>(f));
        }
#endif        
#else
        template<typename F>
        static inline boost::shared_ptr<decorator_function_base> make_decorator_function(F f)
        {
            return boost::shared_ptr<decorator_function_base>(new decorator_function<F>(f));
        }
        
#endif      
    }
    class thread_decoration {
        thread_decoration();  
        thread_decoration(const thread_decoration&);  
    public:
 	    template<typename Callable1>
	    thread_decoration(Callable1 setup)
 		    : setup_(new detail::decorator_function<Callable1>(setup))
            , cleanup_(0)
            , prev_(last_) {
 		    // the constructor is not thread-safe so it must only be used on
 		    // global objects which are constructed before any threads
 		    last_ = this;
 	    }
        
 	    template<typename Callable1,typename Callable2>
	    thread_decoration(Callable1 setup, Callable2 cleanup)
 		    : setup_(new detail::decorator_function<Callable1>(setup))
//            , cleanup_(new detail::decorator_function<Callable2>(cleanup))
            , cleanup_(cleanup)
            , prev_(last_) {
 		    // the constructor is not thread-safe so it must only be used on
 		    // global objects which are constructed before any threads
 		    last_ = this;
 	    }
        
 	    ~thread_decoration() {
            delete setup_;
        }
        static void BOOST_INTERTHREADS_DECL decorate();

    private:
        friend class thread_decorator;
        detail::decorator_function_base* setup_;
        boost::function<void ()> cleanup_;
        static thread_decoration* last_;
        thread_decoration* prev_;
    };

    class thread_decorator {
    public:
        // copy/constructor operators (functor)
        thread_decorator() {}
        thread_decorator(const thread_decorator& other) {
            func_ =other.func_;
        }
        
        thread_decorator& operator=(const thread_decorator& other) {
            thread_decorator tmp(other);
            tmp.swap(*this);
            return *this;
        }

#if defined(BOOST_HAS_VARIADIC_TMPL) 
#if defined(BOOST_HAS_RVALUE_REFS)
        template <typename Callable, typename Arg...>
        explicit thread_decorator(Callable&& f, ...Arg&& args)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),static_cast<F&&>(f),...args))) {};
#else
        template <typename Callable, typename Arg...>
        explicit thread_decorator(Callable f, ...Arg args)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),f,...args))) {};
#endif
#else // !defined(BOOST_HAS_VARIADIC_TMPL)
#if defined(BOOST_HAS_RVALUE_REFS)
        template <typename Callable>
        explicit thread_decorator(Callable&& f)
            : func_(detail::make_decorator_function(static_cast<F&&>(f))) {};
#else // !defined(BOOST_HAS_RVALUE_REFS)            
#ifdef BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
#ifndef BOOST_NO_SFINAE
        template <class Callable>
        explicit thread_decorator(Callable f): func_(detail::make_decorator_function(f)) {}
#else // BOOST_NO_SFINAE
        template <class Callable>
        explicit thread_decorator(Callable f
            ,typename disable_if<boost::is_convertible<F&,boost::detail::thread_move_t<F> >, detail::dummy* >::type=0)
            : func_(detail::make_decorator_function(f)) {}
#endif // BOOST_NO_SFINAE
        template <class Callable>
        explicit thread_decorator(boost::detail::thread_move_t<Callable> f):
            func_(detail::make_decorator_function(f)) {}
#else // ! BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
        template <class Callable>
        explicit thread_decorator(Callable f): func_(detail::make_decorator_function(f)) {}
#endif // ! BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
            
                
        template <typename Callable, typename A1>
        thread_decorator(Callable f, A1 a1)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),f,a1))) {};
                
        template <typename Callable, typename A1, typename A2>
        thread_decorator(Callable f, A1 a1, A2 a2)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),f,a1,a2))) {};
                
        template <typename Callable, typename A1, typename A2, typename A3>
        thread_decorator(Callable f, A1 a1, A2 a2, A3 a3)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),f,a1,a2,a3))) {};
                
        template <typename Callable, typename A1, typename A2, typename A3, typename A4>
        thread_decorator(Callable f, A1 a1, A2 a2, A3 a3, A4 a4)
            : func_(detail::make_decorator_function(boost::bind(boost::type<void>(),f,a1,a2,a3,a4))) {};
#endif
#endif                
        // move semantics
#if defined(BOOST_HAS_RVALUE_REFS)
        thread_decorator(thread_decorator&& other) {
            func_.swap(other.func_);
        }
        
        thread_decorator& operator=(thread_decorator&& other) {
            func_=other.func_;
            other.func_.reset();
            return *this;
        }

        thread_decorator&& move() {
            return static_cast<thread_decorator&&>(*this);
        }
        
#else // ! defined(BOOST_HAS_RVALUE_REFS)
#ifdef BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
        thread_decorator(boost::detail::thread_move_t<thread_decorator> x) {
            func_=x->func_;
            x->func_.reset();
        }
        
        thread_decorator& operator=(boost::detail::thread_move_t<thread_decorator> x) {
            thread_decorator new_decorator(x);
            swap(new_decorator);
            return *this;
        }
        
        operator boost::detail::thread_move_t<thread_decorator>() {
            return move();
        }
        
        boost::detail::thread_move_t<thread_decorator> move() {
            boost::detail::thread_move_t<thread_decorator> x(*this);
            return x;
        }
#endif // ! BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE      
#endif // ! defined(BOOST_HAS_RVALUE_REFS)

        void swap(thread_decorator& x) {
            func_.swap(x.func_);
        }     
        void operator()() const;
    private:
        boost::shared_ptr<detail::decorator_function_base> func_;
    };
 
    template <class F>
    thread_decorator make_decorator(F f) {
#ifdef BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE
        return move(thread_decorator(f));
#else
        return thread_decorator(f);
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

    inline static void decorate() {thread_decoration::decorate();}
  
    template <typename AE, typename Decorator>
    struct asynchronous_executor_decorator : AE {
        //typedef typename boost::result_of<F()>::type result_type;
        template <typename F>
        typename AE::template handle< typename boost::result_of<F()>::type >::type 
        fork( F fn ) {
            return this->AE::fork(Decorator(fn));
        }
    };

    //typedef asynchronous_executor_decorator<threader,thread_decorator> threader_decorator;
    
}
}

#include <boost/config/abi_suffix.hpp>


#endif

