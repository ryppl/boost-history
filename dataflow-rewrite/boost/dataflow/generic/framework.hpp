/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__FRAMEWORK_HPP
#define BOOST__DATAFLOW__GENERIC__FRAMEWORK_HPP


#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace dataflow {

/// Base class for Framework types.
struct framework_base {};

/// The default Framework argument for Dataflow templates.
struct default_framework : framework_base {};

/// Boolean metafunction determining whether a type is a Framework.
template<typename T>
struct is_framework
    : public is_base_of<framework_base, T> {};


} } // namespace boost::dataflow


#endif // BOOST__DATAFLOW__GENERIC__FRAMEWORK__HPP
