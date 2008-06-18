// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_STORABLE_HPP
#define SIGNAL_NETWORK_STORABLE_HPP

#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/mpl/transform.hpp>

namespace boost { namespace signals {

namespace detail
{
    ///	Converts a type into a storable type by removing const qualifiers and references.
    template<typename T>
    struct storable : public boost::remove_const<typename boost::remove_reference<T>::type > {};

    template<typename T>
    struct mpl_storable
    {
        typedef typename boost::mpl::transform<
            typename boost::mpl::transform<T,
            boost::remove_reference<boost::mpl::_> >::type,
            boost::remove_const<boost::mpl::_> >::type type;
    };
} // namespace detail

} } // namespace boost::signals

#endif // SIGNAL_NETWORK_STORABLE_HPP
