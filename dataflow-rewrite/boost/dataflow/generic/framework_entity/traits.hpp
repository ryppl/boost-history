/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__FRAMEWORK_ENTITY__TRAITS_HPP
#define BOOST__DATAFLOW__GENERIC__FRAMEWORK_ENTITY__TRAITS_HPP


#include <boost/dataflow/generic/framework.hpp>
#include <boost/dataflow/utility/enable_if_type.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace dataflow {

/// Convenience base class for FrameworkEntityTraits types.
template<typename Framework>
struct traits
{
    /// The Framework of the FrameworkEntityTraits.
    typedef Framework framework;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_framework<Framework> ));
};

namespace detail {
    struct enable_guard;
}

/// Boolean Metafunction determining whether a type is a FrameworkEntityTraits.
template<typename Traits, typename Enable=detail::enable_guard>
struct is_traits : public mpl::false_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_same<Enable, detail::enable_guard> ));
};

/// INTERNAL ONLY
template<typename Traits>
struct is_traits<Traits,
    typename utility::enable_if_type<
        utility::all_of<
            typename Traits::framework
        >,
        detail::enable_guard
    >::type>
 : public mpl::true_
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_framework<typename Traits::framework> ));
};

}}

#endif // BOOST__DATAFLOW__GENERIC__FRAMEWORK_ENTITY__TRAITS_HPP
