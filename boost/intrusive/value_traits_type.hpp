/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga  2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_VALUE_TRAITS_TYPE_HPP
#define BOOST_INTRUSIVE_VALUE_TRAITS_TYPE_HPP

namespace boost {
namespace intrusive {

//!This enumeration defines the type of value_traits that can be defined
//!for Boost.Intrusive containers
enum {
   //!This value_traits does not need any treatment in containers
   normal_value_traits,

   //!This value_traits needs the container to check if the nodes are
   //!default initialized by the user before inserting them,
   //!and tells the container to default initialize the nodes after
   //!erasing them from the container.
   safe_mode_value_traits,

   //!Same as "safe_mode_value_traits" but the user type is an auto-unlink
   //!type, so it's not compatible with non constant-time size containers.
   //!and the container might be modified without using its public interface.
   auto_unlink_value_traits
};
} //namespace intrusive 
} //namespace boost 

#endif //BOOST_INTRUSIVE_VALUE_TRAITS_TYPE_HPP
