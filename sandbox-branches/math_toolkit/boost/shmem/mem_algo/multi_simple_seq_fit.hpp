//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_MULTI_SIMPLE_SEQ_FIT_HPP
#define BOOST_SHMEM_MULTI_SIMPLE_SEQ_FIT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/shmem_fwd.hpp>
#include <boost/shmem/mem_algo/basic_simple_seq_fit.hpp>
#include <boost/shmem/intersegment_ptr.hpp>

/*!\file
   Describes sequential fit algorithm used to allocate objects in shared memory.
*/

namespace boost {

namespace shmem {

/*!This class implements the simple sequential fit algorithm with a simply
   linked list of free buffers.*/
template<class MutexFamily, class VoidPtr>
class multi_simple_seq_fit 
   : public basic_simple_seq_fit<MutexFamily, VoidPtr >
{
   typedef basic_simple_seq_fit<MutexFamily, VoidPtr > base_t;
 public:
   /*!Constructor. "size" is the total size of the fixed size memory segment, 
      "extra_hdr_bytes" indicates the extra bytes beginning in the sizeof(multi_simple_seq_fit)
      offset that the allocator should not use at all.*/
   multi_simple_seq_fit           (size_t size, size_t extra_hdr_bytes)
      : base_t(size, extra_hdr_bytes){}

   /*!Allocates bytes from existing segments. If there is no memory, it uses
      the growing functor associated with the group to allocate a new segment.
      If this fails, returns 0.*/
   void* allocate             (size_t nbytes)
      {  return base_t::multi_allocate(nbytes);   }
};

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //#ifndef BOOST_SHMEM_MULTI_SIMPLE_SEQ_FIT_HPP

