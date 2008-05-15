/**
	@file
	Boost tlh_pool.hpp header file.

	@author
	Copyright (c) 2008 Phil Bouchard <phil@fornux.com>.

	@note
	Distributed under the Boost Software License, Version 1.0.

	See accompanying file LICENSE_1_0.txt or copy at
	http://www.boost.org/LICENSE_1_0.txt

	See http://www.boost.org/libs/smart_ptr/doc/index.html for documentation.
*/


#ifndef BOOST_TLH_POOL_HPP
#define BOOST_TLH_POOL_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/pool/poolfwd.hpp>

#include "nedmalloc.c"


#ifdef BOOST_NO_STDC_NAMESPACE
 namespace std { using ::malloc; using ::free; }
#endif

namespace boost {


class tlh_pool
{
  public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

  private:
    BOOST_STATIC_CONSTANT(unsigned, min_alloc_size =
        (::boost::details::pool::ct_lcm<sizeof(void *), sizeof(size_type)>::value) );

  public:
    explicit tlh_pool(const size_type nrequested_size, const size_type nnext_size = 32);
    ~pool();

    bool release_memory();
    bool purge_memory();
    void * malloc();
    void * ordered_malloc();
    void * ordered_malloc(size_type n);
    void free(void * const chunk);
    void ordered_free(void * const chunk);
    void free(void * const chunks, const size_type n);
    void ordered_free(void * const chunks, const size_type n);
    bool is_from(void * const chunk) const;
};

} // namespace boost

#endif
