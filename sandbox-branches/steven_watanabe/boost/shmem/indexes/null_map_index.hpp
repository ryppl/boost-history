//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SHMEM_NULL_MAP_INDEX_HPP
#define BOOST_SHMEM_NULL_MAP_INDEX_HPP

#include <boost/shmem/offset_ptr.hpp>

/*!\file
   Describes a null index adaptor, so that if we don't want to construct 
   named objects, we can use this null index type to save resources.
*/

namespace boost { namespace shmem {

/*!Null index type*/
template <class MapConfig>
class null_index
{
   typedef typename MapConfig::segment_manager segment_manager;

 public:
   
   /*!Dummy function*/
   null_index(segment_manager *segment_mngr_hdr){}

};

}}   //namespace boost { namespace shmem {

#endif   //#ifndef BOOST_SHMEM_NULL_MAP_INDEX_HPP
