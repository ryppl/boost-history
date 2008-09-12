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
#include <boost/mpl/is_sequence.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace dataflow {

/// Convenience class for Traits types.
template<typename Framework>
struct traits
{
    /// The Framework of the port.
    typedef Framework framework;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_framework<Framework> ));
};

namespace detail {
    struct enable_guard;
}

/// Boolean Metafunction determining whether a type is a Traits.
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

/// Metafunction returning the Traits of a type.
template<typename T, typename Framework=default_framework, typename Enable=void>
struct traits_of
{
#ifdef DOXYGEN_DOCS_BUILD
    typedef detail::unspecified type; ///< Traits type.
#endif
};

/// INTERNAL ONLY
/** Specialization allowing intrusive specification of the Traits.
*/
template<typename T>
struct traits_of<
    T,
    typename T::dataflow_traits::framework,
    typename enable_if<
        mpl::not_<mpl::is_sequence<typename T::dataflow_traits> >
    >::type
>
{
    /// INTERNAL ONLY
    typedef typename T::dataflow_traits type;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_traits<type> ));
};

} }

// Specialization allowing intrusive specification of a sequence of Traits.
#include <boost/dataflow/generic/framework_entity/traits_sequence_intrusive_registration.hpp>

namespace boost { namespace dataflow {

/// Allows registration of Traits for all cv-qualified versions of a type.
template<typename T, typename Framework=default_framework, typename Enable=void>
struct register_traits
{};


// Specialization allowing non-intrusive specification of the Traits.

/// INTERNAL ONLY
template<typename T, typename Framework>
struct traits_of<
    T,
    Framework,
    typename utility::enable_if_type<
        typename register_traits<
            typename remove_cv<T>::type,
            Framework
        >::type
    >::type
>
{
    /// INTERNAL ONLY
    typedef
        typename register_traits<
            typename remove_cv<T>::type,
            Framework
        >::type type;
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_traits<type> ));
};

} } // namespace boost::dataflow

/// Macro simplifying non-intrusive specification of a type's Traits.
#define BOOST_DATAFLOW_TRAITS(Type,Traits)      \
namespace boost { namespace dataflow {          \
template<>                                      \
struct register_traits<                         \
    Type,                                       \
    Traits::framework>                          \
{                                               \
    typedef Traits type;                        \
    BOOST_MPL_ASSERT(( is_traits<type> ));      \
};                                              \
}}

/// Macro simplifying non-intrusive specification of multiple types'
/// Traits, using a boost::enable_if condition.
#define BOOST_DATAFLOW_TRAITS_ENABLE_IF(Type,Cond,Traits)       \
namespace boost { namespace dataflow {                          \
template<typename Type>                                         \
struct register_traits<                                         \
    Type,                                                       \
    typename Traits::framework,                                 \
    typename boost::enable_if< Cond >::type>                    \
{                                                               \
    typedef Traits type;                                        \
    BOOST_MPL_ASSERT(( is_traits<type> ));                      \
};                                                              \
}}

#endif // BOOST__DATAFLOW__GENERIC__TRAITS_HPP
