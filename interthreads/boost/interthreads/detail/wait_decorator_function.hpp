#ifndef BOOST_INTERTHREADS_WAIT_DECORATION_FUNCTION__HPP
#define BOOST_INTERTHREADS_WAIT_DECORATION_FUNCTION__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
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

#include <boost/interthreads/detail/decorator_function.hpp>
//#define BOOST_INTERTHREADS_THREAD_DECORATOR_MOVE

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {
    namespace detail {

        template<typename F>
        struct BOOST_INTERTHREADS_DECL wait_decorator_function : decorator_function_base<typename boost::result_of<F()>::type> {
            F f_;
            boost::condition_variable& cond_;
            bool started_&;

            typedef  typename boost::result_of<F()>::type result_type;

            wait_decorator_function(boost::condition_variable& cond, bool& started, F f)
                : f_(f)
                , cond_(cond)
                , started_(started)
                {}

            typename boost::result_of<F()>::type operator()() const {
                started_=true;
                cond_.notify_one();
                return f_();
            }
        private:
            wait_decorator_function();

        };

        template<typename F>
        static inline boost::shared_ptr<wait_decorator_function_base<typename boost::result_of<F()>::type> > make_wait_decorator_function(F f)
        {
            return boost::shared_ptr<wait_decorator_function_base<typename boost::result_of<F()>::type> >(new wait_decorator_function<F>(f));
        }

    }

}
}

#include <boost/config/abi_suffix.hpp>


#endif

