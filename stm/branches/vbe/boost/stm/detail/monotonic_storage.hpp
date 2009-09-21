//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_MONOTONIC_STORAGE__HPP
#define BOOST_STM_MONOTONIC_STORAGE__HPP

#include <boost/stm/detail/config.hpp>

namespace boost { namespace stm {

#if defined(BOOST_STM_USE_MEMCOPY) && defined(BOOST_STM_CACHE_USE_TSS_MONOTONIC_MEMORY_MANAGER)
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
   template <std::size_t size>
   struct monotonic_storage {
       char storage_[size];
       char* ptr_;
    public:
        monotonic_storage() : ptr_(&storage_[0]) {}
        template <typename T>
        char* allocate() {
            union aligned_storage {
                char val[sizeof(T)] ;
                int alignement;
            };
            char* ptr= ptr_;
            ptr_+= sizeof(aligned_storage);
            return ptr;
        }
        void reset() {ptr_=&storage_[0];}
   };
#endif

}}

///////////////////////////////////////////////////////////////////////////////
#endif // BOOST_STM_MONOTONIC_STORAGE__HPP


