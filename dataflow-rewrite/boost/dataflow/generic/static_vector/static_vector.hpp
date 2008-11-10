/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#ifndef BOOST__DATAFLOW__GENERIC__STATIC_VECTOR__STATIC_VECTOR_HPP
#define BOOST__DATAFLOW__GENERIC__STATIC_VECTOR__STATIC_VECTOR_HPP

#include <boost/dataflow/generic/static_vector/traits.hpp>
#include <boost/dataflow/generic/framework_entity/default_framework_of.hpp>
#include <boost/dataflow/generic/framework_entity/traits_of.hpp>

namespace boost { namespace dataflow {

/// Boolean Metafunction determining whether a type is a Port.
/** is_port<T, Framework> is an Integral Constant of type bool.
    It evaluates to true if T models Port, false otherwise.
*/
template<typename T, typename Framework=typename default_framework_of<T>::type, typename Enable=void>
struct is_static_vector
    : public mpl::false_
{
#ifdef DOXYGEN_DOCS_BUILD
    /// Integral Constant value type
    typedef bool value_type;
    /// true if T models Port, false otherwise.
    typedef detail::unspecified value;
    /// Integral Constant
    typedef detail::unspecified type;
#endif
};

/// INTERNAL ONLY
template<typename T, typename Framework>
struct is_static_vector<
    T,
    Framework,
    typename enable_if<
        is_static_vector_traits<typename traits_of<T, Framework>::type>
    >::type >
    : public mpl::true_ {};

/// Convenience base class for Port types. 
template<typename StaticVectorTraits>
struct static_vector
{
    /// PortTraits for the Port.
    typedef StaticVectorTraits dataflow_traits;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_static_vector_traits<StaticVectorTraits> ));
};

} } // namespace boost::dataflow

#endif // BOOST__DATAFLOW__GENERIC__STATIC_VECTOR__STATIC_VECTOR_HPP
