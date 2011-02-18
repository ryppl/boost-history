#ifndef BOOST_ASYNC_LAZY_FORK__HPP
#define BOOST_ASYNC_LAZY_FORK__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/bind.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/async/act_traits.hpp>
#include <boost/thread/future.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace async {

namespace detail {
    template <typename T>
    struct invoke_lazy_task {
        typedef void result_type;
        void operator()(packaged_task<T>& task)
        {
            try
            {
                task();
            }
            catch(task_already_started&)
            {}
        }
    };
}

namespace result_of {
    template <typename AE,typename F>
    struct lazy_fork {
        typedef typename boost::result_of<F()>::type result_type;
        typedef packaged_task<result_type> type;
        typedef boost::detail::thread_move_t<packaged_task<result_type> > move_type;
    };
}

namespace partial_specialization_workaround {
template< typename AE, typename F >
struct lazy_fork {
    static typename result_of::lazy_fork<AE,F>::move_type apply(AE& , F fn ) {
        packaged_task<typename boost::result_of<F()>::type> tsk(fn);
        tsk.set_wait_callback(detail::invoke_lazy_task<typename boost::result_of<F()>::type>());
        return boost::move(tsk);
    }
};
}
template< typename AE, typename F >
typename result_of::lazy_fork<AE,F>::move_type
lazy_fork( AE& ae, F fn ) {
    return partial_specialization_workaround::lazy_fork<AE,F>::apply(ae, fn);
}

template< typename AE, typename F, typename A1 >
typename asynchronous_completion_token<AE, typename boost::result_of<F(A1)>::type >::type
lazy_fork( AE& ae, F fn, A1 a1 ) {
    return lazy_fork( ae, bind( fn, a1 ) );
}

template< typename AE, typename F, typename A1, typename A2 >
typename  asynchronous_completion_token<AE, typename boost::result_of<F(A1,A2)>::type >::type
lazy_fork( AE& ae, F fn, A1 a1, A2 a2 ) {
    return lazy_fork( ae, bind( fn, a1, a2 ) );
}

template< typename AE, typename F, typename A1, typename A2, typename A3 >
typename  asynchronous_completion_token<AE, typename boost::result_of<F(A1,A2,A3)>::type >::type
lazy_fork( AE& ae, F fn, A1 a1, A2 a2, A3 a3 ) {
    return lazy_fork( ae, bind( fn, a1, a2, a3 ) );
}

template< typename AE, typename F, typename A1, typename A2, typename A3, typename A4 >
typename  asynchronous_completion_token<AE, typename boost::result_of<F(A1,A2,A3,A4)>::type >::type
lazy_fork( AE& ae, F fn, A1 a1, A2 a2, A3 a3, A4 a4 ) {
    return lazy_fork( ae, bind( fn, a1, a2, a3, a4 ) );
}

}
}


#include <boost/config/abi_suffix.hpp>

#endif

