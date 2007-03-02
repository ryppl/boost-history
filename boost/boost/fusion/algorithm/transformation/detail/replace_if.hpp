/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_REPLACE_IF_08182005_0946)
#define FUSION_REPLACE_IF_08182005_0946

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <bool is_convertible>
    struct replacer_if_helper;

    template <>
    struct replacer_if_helper<false>
    {
        template <typename U, typename F, typename T>
        static U&
        call(U& x, F&, T const&)
        {
            return x;
        }
    };

    template <>
    struct replacer_if_helper<true>
    {
        template <typename U, typename F, typename T>
        static U
        call(U& x, F& f, T const& new_value)
        {
            return f(x) ? new_value : x;
        }
    };

    template <typename F, typename T>
    struct replacer_if
    {
        replacer_if(F f, T const& new_value)
            : f(f), new_value(new_value) {}

        template <typename U>
        struct result
        {
            typedef typename
                mpl::if_<is_convertible<T, U>, U, U const&>::type
            type;
        };
    
        template <typename U>
        typename result<U>::type
        operator()(U const& x) const
        {
            return replacer_if_helper<is_convertible<T, U>::value>::
                call(x, f, new_value);
        }

        F f;
        T new_value;
    };
}}}

#endif

