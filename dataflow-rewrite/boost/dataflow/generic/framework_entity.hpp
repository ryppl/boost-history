/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__FRAMEWORK_ENTITY_HPP
#define BOOST__DATAFLOW__GENERIC__FRAMEWORK_ENTITY_HPP


#include <boost/dataflow/generic/framework_entity/traits.hpp>
#include <boost/dataflow/utility/enable_if_type.hpp>

#include <boost/mpl/bool.hpp>

namespace boost { namespace dataflow {

namespace detail {
    struct enable_guard;
}

/// Boolean Metafunction determining whether a type has traits.
template<typename T, typename Framework=default_framework, typename Enable=detail::enable_guard>
struct is_framework_entity : public mpl::false_
{
#ifdef DOXYGEN_DOCS_BUILD
    /// Integral Constant value type
    typedef bool value_type;
    /// true if T models Entity, false otherwise.
    typedef detail::unspecified value;
    /// Integral Constant
    typedef detail::unspecified type;
#endif
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_same<Enable, detail::enable_guard> ));
};

/// INTERNAL ONLY
template<typename T, typename Framework>
struct is_framework_entity<
    T,
    Framework,
    typename utility::enable_if_type<
        typename traits_of<T, Framework>::type,
        detail::enable_guard
    >::type>
    : public mpl::true_
{};

} } // namespace boost::dataflow

#endif // BOOST__DATAFLOW__GENERIC__FRAMEWORK_ENTITY_HPP
