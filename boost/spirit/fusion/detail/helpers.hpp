/*=============================================================================
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_HELPERS_HPP)
#define BOOST_FUSION_HELPERS_HPP

#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace fusion { namespace tuple_detail {

    template <typename T>
    struct maybe_string
    {
        typedef typename
            mpl::if_<
                is_array<T>,
                typename mpl::if_<
                    is_convertible<T, char const*>,
                    char const*,
                    typename mpl::if_<
                        is_convertible<T, wchar_t const*>,
                        wchar_t const*,
                        T>::type
                    >::type,
                T
            >::type
        type;
    };

    template <typename T>
    struct call_param
    {
        typedef typename
            mpl::if_<
                is_class<T>
              , typename add_reference<typename add_const<T>::type>::type
              , typename add_const<T>::type
            >::type
        type;
    };

}}} //  namespace boost::fusion::tuple_detail

#endif
