//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_FWD_HPP
#define BOOST_SHMEM_FWD_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>

#if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
// old Dinkumware
#  include <boost/compatibility/cpp_c_headers/cstddef>
#else
#  include <cstddef>
#endif

namespace boost { namespace shmem {

//////////////////////////////////////////////////////////////////////////////
//                            shared_memory
//////////////////////////////////////////////////////////////////////////////

class shared_memory;

//////////////////////////////////////////////////////////////////////////////
//                            mapped files
//////////////////////////////////////////////////////////////////////////////

class mmapped_file;

//////////////////////////////////////////////////////////////////////////////
//                               Mutexes
//////////////////////////////////////////////////////////////////////////////

class shared_mutex;
class shared_try_mutex;
class shared_timed_mutex;

class shared_recursive_mutex;
class shared_recursive_try_mutex;
class shared_recursive_timed_mutex;

class named_semaphore;
class named_mutex;

//////////////////////////////////////////////////////////////////////////////
//                         Mutex families
//////////////////////////////////////////////////////////////////////////////

struct shared_mutex_family;
struct null_mutex_family;

//////////////////////////////////////////////////////////////////////////////
//                      STL compatible allocators
//////////////////////////////////////////////////////////////////////////////

template<class T, class AllocAlgo>
class allocator;

template<class T, std::size_t N, class AllocAlgo>
class node_allocator;

template<class T, std::size_t N, class AllocAlgo>
class private_node_pool;

template<class T, class AllocAlgo>
class cached_node_allocator;

//////////////////////////////////////////////////////////////////////////////
//                            offset_ptr
//////////////////////////////////////////////////////////////////////////////

class full_offset_ptr;
class offset_1_null_ptr;

template <class PointedType, class RootPtr = offset_1_null_ptr>
class offset_ptr;


//////////////////////////////////////////////////////////////////////////////
//                         intersegment_ptr
//////////////////////////////////////////////////////////////////////////////

template <class Mutex>
struct flat_map_intersegment;

template <class T, class PT = flat_map_intersegment<shared_mutex> >
class intersegment_ptr;

//////////////////////////////////////////////////////////////////////////////
//                    Memory allocation algorithms
//////////////////////////////////////////////////////////////////////////////

//Single segment memory allocation algorithms
template<class MutexFamily, class VoidMutex = offset_ptr<void> >
class seq_fit;

template<class MutexFamily, class VoidMutex = offset_ptr<void> >
class simple_seq_fit;

//Single segment memory allocation algorithms
template<class MutexFamily, class VoidMutex = intersegment_ptr<void> >
class multi_seq_fit;

template<class MutexFamily, class VoidMutex = intersegment_ptr<void> >
class multi_simple_seq_fit;

//////////////////////////////////////////////////////////////////////////////
//                         Index Types
//////////////////////////////////////////////////////////////////////////////

template<class IndexConfig> class flat_map_index;
template<class IndexConfig> class map_index;

//////////////////////////////////////////////////////////////////////////////
//                  Named object creation front-ends
//////////////////////////////////////////////////////////////////////////////

template <
            class CharType, 
            class MemoryAlgorithm, 
            template<class IndexConfig> class IndexType
         >
class basic_named_user_object;

typedef basic_named_user_object < 
   char, 
   simple_seq_fit<null_mutex_family>,
   flat_map_index
   >  named_user_object;

typedef basic_named_user_object< 
   wchar_t, 
   simple_seq_fit<null_mutex_family>,
   flat_map_index
   >  wnamed_user_object;

template <
            class CharType, 
            class MemoryAlgorithm, 
            template<class IndexConfig> class IndexType
         >
class basic_named_shared_object;

typedef basic_named_shared_object < 
   char, 
   simple_seq_fit<shared_mutex_family>,
   flat_map_index
   >  named_shared_object;

typedef basic_named_shared_object< 
   wchar_t, 
   simple_seq_fit<shared_mutex_family>,
   flat_map_index
   >  wnamed_shared_object;

typedef basic_named_shared_object < 
   char, 
   simple_seq_fit<shared_mutex_family, void *>,
   flat_map_index
   >  fixed_named_shared_object;

typedef basic_named_shared_object< 
   wchar_t, 
   simple_seq_fit<shared_mutex_family, void *>,
   flat_map_index
   >  wfixed_named_shared_object;

template <
            class CharType, 
            class MemoryAlgorithm, 
            template<class IndexConfig> class IndexType
         >
class basic_named_heap_object;


typedef basic_named_heap_object < 
   char, 
   simple_seq_fit<null_mutex_family>,
   flat_map_index
   >  named_heap_object;

typedef basic_named_heap_object< 
   wchar_t, 
   simple_seq_fit<null_mutex_family>,
   flat_map_index
   >  wnamed_heap_object;


template <
            class CharType, 
            class MemoryAlgorithm, 
            template<class IndexConfig> class IndexType
         >
class basic_named_mfile_object;


typedef basic_named_mfile_object < 
   char, 
   simple_seq_fit<shared_mutex_family>,
   flat_map_index
   >  named_mfile_object;

typedef basic_named_mfile_object< 
   wchar_t, 
   simple_seq_fit<shared_mutex_family>,
   flat_map_index
   >  wnamed_mfile_object;
//////////////////////////////////////////////////////////////////////////////
//                            Exceptions
//////////////////////////////////////////////////////////////////////////////

class lock_exception;
class process_resource_exception;
class bad_alloc;

}  }  //namespace boost { namespace shmem {

#include <boost/shmem/detail/config_end.hpp>

#endif //#ifndef BOOST_SHMEM_FWD_HPP

