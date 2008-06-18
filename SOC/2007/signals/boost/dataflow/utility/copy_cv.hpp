// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_UTILITY_TRANSFER_CV_HPP
#define BOOST_DATAFLOW_UTILITY_TRANSFER_CV_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/add_volatile.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_volatile.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_volatile.hpp>

namespace boost { namespace dataflow { namespace utility {

template<typename T_dst, typename T_src>
struct copy_const
{
    typedef
        typename mpl::if_<
            is_const<T_src>,
            add_const<T_dst>,
            remove_const<T_dst>
        >::type::type type;
};

template<typename T_dst, typename T_src>
struct copy_volatile
{
    typedef
        typename mpl::if_<
            is_volatile<T_src>,
            add_volatile<T_dst>,
            remove_volatile<T_dst>
        >::type::type type;
};

template<typename T_dst, typename T_src>
struct copy_cv
{
    typedef
        typename copy_volatile<
            typename copy_const<
                T_dst,
                T_src
            >::type,
            T_src
        >::type type;
};

} } }

#endif // BOOST_DATAFLOW_UTILITY_TRANSFER_CV_HPP
