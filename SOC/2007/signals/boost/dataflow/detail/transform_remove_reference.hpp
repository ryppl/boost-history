// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_DETAIL_TRANSFORM_REMOVE_REFERENCE_HPP
#define BOOST_DATAFLOW_DETAIL_TRANSFORM_REMOVE_REFERENCE_HPP

#include <boost/mpl/transform.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace boost { namespace dataflow {

namespace detail
{
    template<typename T>
    struct transform_remove_reference
    {
        typedef typename boost::mpl::transform<T,
            boost::remove_reference<boost::mpl::_> >::type type;
    };
} // namespace detail

}} // namespace boost::dataflow

#endif // BOOST_DATAFLOW_DETAIL_TRANSFORM_REMOVE_REFERENCE_HPP

