/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__FRAMEWORK_HPP
#define BOOST__DATAFLOW__GENERIC__FRAMEWORK_HPP


#include <boost/dataflow/generic/operation.hpp>
#include <boost/dataflow/utility/enable_if_type.hpp>
#include <boost/dataflow/utility/forced_sequence.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost { namespace dataflow {


template<typename Operations, typename FrameworkObject=void>
struct framework
{
    typedef typename utility::forced_sequence<Operations>::type operations;
    typedef FrameworkObject framework_object_type;
};

template<typename Operations>
struct framework<Operations>
{
    typedef typename utility::forced_sequence<Operations>::type operations;
};

namespace operations {

struct connect : public operation<semantics::connect> {};

}

/// The default Framework argument for Dataflow templates.
struct default_framework
    : public framework<operations::connect>
{};

/// Boolean metafunction determining whether a type is a Framework.
template<typename T, typename Enable=void>
struct is_framework
    : public mpl::false_ {};

template<typename T>
struct is_framework<T,
    typename enable_if<mpl::is_sequence<typename T::operations> >::type>
    : public mpl::true_ {};

/// Boolean metafunction determining whether a type is a Framework.
template<typename T, typename Enable=void>
struct has_framework_object
    : public mpl::false_ {};

template<typename T>
struct has_framework_object<T,
    typename utility::enable_if_type<typename T::framework_object_type>::type>
    : public mpl::true_ {};

} } // namespace boost::dataflow


#endif // BOOST__DATAFLOW__GENERIC__FRAMEWORK__HPP
