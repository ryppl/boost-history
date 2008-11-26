/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#ifndef BOOST__DATAFLOW__GENERIC__DYNAMIC_VECTOR__TRAITS_HPP
#define BOOST__DATAFLOW__GENERIC__DYNAMIC_VECTOR__TRAITS_HPP

#include <boost/dataflow/generic/framework_entity/traits.hpp>
#include <boost/mpl/is_sequence.hpp>

namespace boost { namespace dataflow {

template<typename Entity, typename Framework=default_framework>
struct dynamic_vector_traits
    : public traits<Framework>
{
    typedef Entity entity;
};

/// Boolean Metafunction determining whether a type is a PortTraits.
template<typename StaticVectorTraits, typename Enable=detail::enable_guard>
struct is_dynamic_vector_traits : public mpl::false_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_same<Enable, detail::enable_guard> ));
};

/// INTERNAL ONLY
template<typename DynamicVectorTraits>
struct is_dynamic_vector_traits<DynamicVectorTraits,
    typename utility::enable_if_type<
        utility::all_of<
            typename DynamicVectorTraits::framework,
            typename DynamicVectorTraits::entity
        >,
        detail::enable_guard
    >::type>
 : public mpl::true_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_framework<typename DynamicVectorTraits::framework>));
};

} } // namespace boost::dataflow

#endif // BOOST__DATAFLOW__GENERIC__DYNAMIC_VECTOR__TRAITS_HPP
