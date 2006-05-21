//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_SIMPLE_SEQ_FIT_HPP
#define BOOST_SHMEM_SIMPLE_SEQ_FIT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/shmem_fwd.hpp>
#include <boost/shmem/mem_algo/basic_simple_seq_fit.hpp>
#include <boost/shmem/offset_ptr.hpp>

/*!\file
   Describes sequential fit algorithm used to allocate objects in shared memory.
*/

namespace boost {

namespace shmem {

/*!This class implements the simple sequential fit algorithm with a simply
   linked list of free buffers.*/
template<class MutexFamily, class VoidPointer>
class simple_seq_fit 
   : public basic_simple_seq_fit<MutexFamily, VoidPointer>
{
   typedef basic_simple_seq_fit<MutexFamily, VoidPointer> base_t;
 public:
   /*!Constructor. "size" is the total size of the fixed size memory segment, 
      "extra_hdr_bytes" indicates the extra bytes beginning in the sizeof(simple_seq_fit)
      offset that the allocator should not use at all.*/
   simple_seq_fit           (size_t size, size_t extra_hdr_bytes)
      : base_t(size, extra_hdr_bytes){}
};

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //#ifndef BOOST_SHMEM_SIMPLE_SEQ_FIT_HPP

