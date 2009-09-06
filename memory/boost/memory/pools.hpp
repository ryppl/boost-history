//
//  boost/memory/pools.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_POOLS_HPP
#define BOOST_MEMORY_POOLS_HPP

#ifndef BOOST_MEMORY_POOL_HPP
#include "pool.hpp"
#endif

#ifndef BOOST_MEMORY_REGION_ALLOC_HPP
#include "region_alloc.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class pools, scoped_pools

template <class PolicyT, size_t nPool = 16, size_t nAlignBits = 3, int m_fPreAlloc = 1>
class pools_alloc
{
private:
	pools_alloc(const pools_alloc&);
	const pools_alloc& operator=(const pools_alloc&);

public:
	typedef size_t size_type;
	typedef fixed_alloc<PolicyT> pool_type;
	typedef region_alloc<PolicyT, m_fPreAlloc> region_alloc_type;
	typedef typename pool_type::alloc_type block_pool_type;

private:
	enum { ALIGN_BITS = nAlignBits };
	enum { NPOOL = nPool };
	enum { ALIGN = 1 << ALIGN_BITS };
	enum { MIN_BYTES = ALIGN };
	enum { MAX_BYTES = ALIGN * NPOOL };

private:
	pool_type* m_pools[nPool];
	block_pool_type m_recycle;
	region_alloc_type m_alloc;

public:
	pools_alloc()
	{
		std::fill_n(m_pools, nPool, (pool_type*)NULL);
	}

	pools_alloc(block_pool_type recycle)
		: m_recycle(recycle), m_alloc(recycle)
	{
		std::fill_n(m_pools, nPool, (pool_type*)NULL);
	}

	block_pool_type BOOST_MEMORY_CALL get_block_pool() const
	{
		return m_recycle;
	}

	region_alloc_type& BOOST_MEMORY_CALL get_region_alloc()
	{
		return m_alloc;
	}

	pool_type& BOOST_MEMORY_CALL get_pool(size_type cb)
	{
		BOOST_MEMORY_ASSERT(has_pool(cb));
		
		const size_type index = (cb - 1) >> ALIGN_BITS;
		pool_type* p = m_pools[index];
		if (p == NULL)
		{
			const size_type cbElem = (index + 1) << ALIGN_BITS;
			m_pools[index] = p = BOOST_MEMORY_NEW(m_alloc, pool_type)(m_recycle, cbElem);
		}
		return *p;
	}
	
	bool BOOST_MEMORY_CALL has_pool(size_type cb) const
	{
		const size_type index = (cb - 1) >> ALIGN_BITS;
		return index < nPool;
	}
};

typedef pools_alloc<NS_BOOST_MEMORY_POLICY::stdlib> pools;
typedef pools_alloc<NS_BOOST_MEMORY_POLICY::scoped> scoped_pools;

// -------------------------------------------------------------------------
// $Log: $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_POOLS_HPP */
