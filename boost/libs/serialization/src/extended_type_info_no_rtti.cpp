/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// extended_type_info_no_rtti.cpp: specific implementation of type info
// that is NOT based on typeid

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/config.hpp>
#include <cstring>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{ using ::strcmp; }
#endif

#define BOOST_SERIALIZATION_SOURCE
#include <boost/serialization/extended_type_info_no_rtti.hpp>

namespace boost { 
namespace serialization { 

BOOST_SERIALIZATION_DECL  bool
extended_type_info_no_rtti_base::less_than(
    const boost::serialization::extended_type_info &rhs) const 
{
    return std::strcmp(key, rhs.key) < 0;
}
BOOST_SERIALIZATION_DECL  bool
extended_type_info_no_rtti_base::equal_to(
    const boost::serialization::extended_type_info &rhs) const
{
    return std::strcmp(key, rhs.key) == 0;
}
BOOST_SERIALIZATION_DECL  bool
extended_type_info_no_rtti_base::not_equal_to(
    const boost::serialization::extended_type_info &rhs) const 
{
    return std::strcmp(key, rhs.key) != 0;
}

BOOST_SERIALIZATION_DECL  
extended_type_info_no_rtti_base::extended_type_info_no_rtti_base() :
    boost::serialization::extended_type_info("extended_type_info_no_rtti")
{}

BOOST_SERIALIZATION_DECL  
extended_type_info_no_rtti_base::~extended_type_info_no_rtti_base()
{}

} // namespace serialization
} // namespace boost
