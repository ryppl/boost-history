/**
	@file
	Boost local_pool.hpp header file.

	@author
	Copyright (c) 2008 Phil Bouchard <phil@fornux.com>
	              2005-2007 Niall Douglas <s_sourceforge@nedprod.com>

	@note
	Distributed under the Boost Software License, Version 1.0.

	See accompanying file LICENSE_1_0.txt or copy at
	http://www.boost.org/LICENSE_1_0.txt

	See http://www.boost.org/libs/smart_ptr/doc/index.html for documentation.
*/


#ifndef BOOST_LOCAL_POOL_HPP
#define BOOST_LOCAL_POOL_HPP

#include <cstddef>

#include <boost/config.hpp>
#include <boost/pool/poolfwd.hpp>

#include <boost/detail/nedmalloc.c.h>


#ifdef BOOST_NO_STDC_NAMESPACE
 namespace std { using ::malloc; using ::free; }
#endif

namespace boost {


class local_pool
{
  public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;


    explicit local_pool(const size_type nrequested_size = 0, const size_type nnext_size = 32)
	{
		p = nedcreatepool(nrequested_size, 0);
	}
    ~local_pool()
	{
		neddestroypool(p);
	}

    bool release_memory();
    bool purge_memory();
    void * malloc(const size_type n)
	{
		return nedpmalloc(p, n);
	}
    void * ordered_malloc();
    void * ordered_malloc(size_type n);
    void free(void * const chunk)
	{
		nedpfree(p, chunk);
	}
    void ordered_free(void * const chunk);
    void free(void * const chunks, const size_type n);
    void ordered_free(void * const chunks, const size_type n);
    bool is_from(void * const chunk) const
	{
		return nedpisfrom(p, chunk);
	}
	
  private:
    nedpool * p;
};

} // namespace boost

#endif
