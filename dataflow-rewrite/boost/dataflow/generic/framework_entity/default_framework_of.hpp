/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__FRAMEWORK_ENTITY__DEFAULT_FRAMEWORK_HPP
#define BOOST__DATAFLOW__GENERIC__FRAMEWORK_ENTITY__DEFAULT_FRAMEWORK_HPP


#include <boost/dataflow/generic/framework_entity/traits.hpp>


namespace boost { namespace dataflow {

/// Metafunction returing the default framework for a type.
template<typename Entity, typename Enable=void>
struct default_framework_of
{
    /// The Framework of the port.
    typedef default_framework type;
};


/// INTERNAL ONLY
template<typename Entity>
struct default_framework_of<Entity,
    typename enable_if<
        mpl::not_<mpl::is_sequence<typename Entity::dataflow_traits> >
    >::type>
{
    /// INTERNAL ONLY
    BOOST_MPL_ASSERT(( is_traits<typename Entity::dataflow_traits> ));
    /// INTERNAL ONLY
    typedef typename Entity::dataflow_traits::framework type;
};

/// Allows registration of the default framework for all cv-qualified versions of a type.
template<typename Entity, typename Enable=void>
struct register_default_framework
{};

/// INTERNAL ONLY
template<typename Entity>
struct default_framework_of<
    Entity,
    typename utility::enable_if_type<
        typename register_default_framework<
            typename remove_cv<Entity>::type
        >::type
    >::type
>
{
    /// INTERNAL ONLY
    typedef
        typename register_default_framework<
            typename remove_cv<Entity>::type
        >::type type;
};

} } // namespace boost::dataflow

/// Macro simplifying non-intrusive specification of a type's Traits.
#define BOOST_DATAFLOW_DEFAULT_FRAMEWORK(Type,Framework)        \
namespace boost { namespace dataflow {                          \
template<>                                                      \
struct register_default_framework<                              \
    Type                                                        \
    >                                                           \
{                                                               \
    typedef Framework type;                                     \
    BOOST_MPL_ASSERT(( is_framework<type> ));                   \
};                                                              \
}}

/// Macro simplifying non-intrusive specification of multiple types'
/// Traits, using a boost::enable_if condition.
#define BOOST_DATAFLOW_DEFAULT_FRAMEWORK_ENABLE_IF(Type,Cond,Framework) \
namespace boost { namespace dataflow {                                  \
template<typename Type>                                                 \
struct register_default_framework<                                      \
    Type,                                                               \
    typename boost::enable_if< Cond >::type>                            \
{                                                                       \
    typedef Framework type;                                             \
    BOOST_MPL_ASSERT(( is_framework<Framework> ));                      \
};                                                                      \
}}

#endif // BOOST__DATAFLOW__GENERIC__FRAMEWORK_ENTITY__DEFAULT_FRAMEWORK_HPP
