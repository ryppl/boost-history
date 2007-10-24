// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP
#define BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP

#include <boost/dataflow/detail/enable_if_defined.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>

//temp
#include <boost/mpl/vector.hpp>

namespace boost { namespace dataflow {

template<typename Mechanism>
struct component_traits
{
    typedef Mechanism mechanism;
};

/// Boolean metafunction determining whether a type is a ComponentTraits.
template<typename ComponentTraits, typename Enable=void>
struct is_component_traits : public mpl::false_
{};

template<typename ComponentTraits>
struct is_component_traits<ComponentTraits,
    typename detail::enable_if_defined<
        typename ComponentTraits::mechanism
    >::type>
 : public mpl::true_
{};

/// Metafunction returning the ComponentTraits of a type.
template<typename Mechanism, typename T, typename Enable=void>
struct component_traits_of
{};

/// Specialization allowing intrusive specification of the ComponentTraits.
template<typename Mechanism, typename T>
struct component_traits_of<Mechanism, T,
    typename enable_if<
        is_same<Mechanism, typename T::component_traits::mechanism>
    >::type
>
{
    typedef typename T::component_traits type;
    BOOST_MPL_ASSERT(( is_component_traits<type> ));
};

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SUPPORT_COMPONENT_HPP
