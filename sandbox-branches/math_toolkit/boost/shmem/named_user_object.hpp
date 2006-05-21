//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_NAMED_USER_OBJECT_HPP
#define BOOST_SHMEM_NAMED_USER_OBJECT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/detail/named_object_impl.hpp>
#include <string.h>  //for memcpy

/*!\file
   Describes a named user memory allocation user class. 
*/

namespace boost {

namespace shmem {

/*!A basic user memory named object creation class. Inherits all 
   basic functionality from 
   basic_named_object_impl<CharType, AllocationAlgorithm, IndexType>*/
template
      <
         class CharType, 
         class AllocationAlgorithm, 
         template<class IndexConfig> class IndexType
      >
class basic_named_user_object 
   : public detail::basic_named_object_impl <CharType, AllocationAlgorithm, IndexType>
{
   typedef detail::basic_named_object_impl 
      <CharType, AllocationAlgorithm, IndexType>    base_t;
 public:
   /*!Creates and places the segment manager. This can throw*/
   bool  create   (void *addr, std::size_t size)
      {  return base_t::create_impl(addr, size);  }
 
   /*!Connects to a created the segment manager. Never throws.*/
   bool  open     (void *addr, std::size_t size)
      {  return base_t::open_impl(addr, size);  }

   /*!Frees resources. Never throws.*/
   void close()
      {  base_t::close_impl();   }

   void grow(std::size_t extra_bytes)
      {  base_t::grow(extra_bytes);   }
};

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_SHMEM_NAMED_USER_OBJECT_HPP

