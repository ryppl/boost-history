// Boost.Function library

//  Copyright Douglas Gregor 2001-2006
//  Copyright Emil Dotchevski 2007
//  Use, modification and distribution is subject to the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <boost/function/detail/prologue.hpp>

namespace boost
{
    namespace detail
    {
        namespace function
        {
            struct not_a_std_function {};

            template <typename R, typename ... Args>
            struct which_std_function {
                typedef not_a_std_function type;
            };
            template <typename R, typename Arg1>
            struct which_std_function<R, Arg1> {
                typedef std::unary_function<Arg1, R> type;
            };
            template <typename R, typename Arg1, typename Arg2>
            struct which_std_function<R, Arg1, Arg2> {
                typedef std::unary_function<Arg1, Arg2, R> type;
            };
        }
    }

    template <typename R, typename ... Args>
    class function<R (Args...)> : public function_base,
        public detail::function::which_std_function<R, Args...>::type
    {
    };
}
