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
// intrinsic function log2

namespace detail {

#if defined(_MSC_VER) // vc++

#if _MSC_VER > 1200
#pragma warning(push)
#endif

#pragma warning(disable:4035)
#pragma warning(disable:4793)

__forceinline unsigned int log2(unsigned int val) {
	BOOST_MEMORY_ASSERT(val != 0);
	__asm {
		bsr eax, val
	};
}

#if _MSC_VER > 1200
#pragma warning(pop)
#endif

#elif defined(__GNUG__) || defined(__GNUC__) // g++/gcc

__forceinline unsigned int log2(unsigned int val)
{
	BOOST_MEMORY_ASSERT(val != 0);
	unsigned int result;
	__asm__("bsr %0, %%eax"::"m"(val));
	__asm__("movl %%eax, %0":"=m"(result));
	return result;
}

#else

__forceinline unsigned int log2(unsigned int val)
{
	BOOST_MEMORY_ASSERT(val != 0);

	if (!(val & 0xFFFFFFFFU))
		return 0xCDCDCDCDU;

	unsigned int result = 31, mask = (1 << 31);
	for (;;)
	{
		if (val & mask)
			return result;
		--result;
		mask >>= 1;
	}
}

#pragma message("Warning: unknown compiler and use a slow log2 algorithm!"

#endif

} // namespace detail

// -------------------------------------------------------------------------
// class pools, scoped_pools

template <class PolicyT, int m_fPreAlloc = 1>
class pools_alloc
{
private:
	pools_alloc(const pools_alloc&);
	const pools_alloc& operator=(const pools_alloc&);

public:
	typedef size_t size_type;
	typedef pool_alloc<PolicyT> pool_type;
	typedef region_alloc<PolicyT, m_fPreAlloc> region_alloc_type;
	typedef typename pool_type::alloc_type block_pool_type;

private:
	// 8, 16, 24, 32, 40, ..., 128
	enum { NPOOL1 = 16 };
	enum { ALIGN_BITS1 = 3 };
	enum { ALIGN1 = 1 << ALIGN_BITS1 };
	enum { MIN_BYTES1 = ALIGN1 };
	enum { MAX_BYTES1 = ALIGN1 * NPOOL1 };

	// 2^8-16, 2^9-16, 2^10-16, 2^11-16, 2^12-16, 2^13-16, 2^14-16, 2^15-16
	enum { PADDING2 = 16 };
	enum { MIN_BITS2 = 8 };
	enum { MAX_BITS2 = 15 };
	enum { NPOOL2 = MAX_BITS2 - MIN_BITS2 + 1 };
	enum { MIN_BYTES2 = (1 << MIN_BITS2) - PADDING2 };
	enum { MAX_BYTES2 = (1 << MAX_BITS2) - PADDING2 };

	enum { NPOOL = NPOOL1 + NPOOL2 };

public:
	enum { MAX_BYTES = MAX_BYTES2 + 1 };

private:
	pool_type* m_pools[NPOOL];
	block_pool_type m_recycle;
	region_alloc_type m_alloc;

public:
	pools_alloc()
	{
		memset(m_pools, 0, sizeof(pool_type*)*NPOOL);
	}

	pools_alloc(block_pool_type recycle)
		: m_recycle(recycle), m_alloc(recycle)
	{
		memset(m_pools, 0, sizeof(pool_type*)*NPOOL);
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
		
		if (cb - 1 < (size_type)MAX_BYTES1)
			return get_pool1(cb);
		else
			return get_pool2(cb);
	}
	
	bool BOOST_MEMORY_CALL has_pool(size_type cb) const
	{
		return cb - 1 < (size_type)MAX_BYTES2;
	}

private:
	pool_type& BOOST_MEMORY_CALL get_pool2(size_type cb)
	{
		BOOST_MEMORY_ASSERT(!has_pool1(cb));
		BOOST_MEMORY_ASSERT(has_pool(cb));

		const size_type index = detail::log2(cb + (PADDING2 - 1)) + (NPOOL1 - (MIN_BITS2 - 1));
		pool_type* p = m_pools[index];
		if (p == NULL)
		{
			const size_type cbElem = (1 << (index - (NPOOL1 - MIN_BITS2))) - PADDING2;
			m_pools[index] = p = BOOST_MEMORY_NEW(m_alloc, pool_type)(m_recycle, cbElem);
		}
		return *p;
	}

	pool_type& BOOST_MEMORY_CALL get_pool1(size_type cb)
	{
		BOOST_MEMORY_ASSERT(has_pool1(cb));

		const size_type index = (cb - 1) >> ALIGN_BITS1;
		pool_type* p = m_pools[index];
		if (p == NULL)
		{
			const size_type cbElem = (index + 1) << ALIGN_BITS1;
			m_pools[index] = p = BOOST_MEMORY_NEW(m_alloc, pool_type)(m_recycle, cbElem);
		}
		return *p;
	}

	bool BOOST_MEMORY_CALL has_pool1(size_type cb) const
	{
		return cb - 1 < (size_type)MAX_BYTES1;
	}

public:
	//
	// function allocate/deallocate
	//
	void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		typedef typename PolicyT::system_alloc_type sysalloc;
		if (cb - 1 < (size_type)MAX_BYTES1)
			return get_pool1(cb).allocate();
		else if (cb - 1 < (size_type)MAX_BYTES2)
			return get_pool2(cb).allocate();
		else
			return sysalloc::allocate(cb);
	}

	void BOOST_MEMORY_CALL deallocate(void* p, size_t cb)
	{
		typedef typename PolicyT::system_alloc_type sysalloc;
		if (cb - 1 < (size_type)MAX_BYTES1)
			get_pool1(cb).deallocate(p);
		else if (cb - 1 < (size_type)MAX_BYTES2)
			get_pool2(cb).deallocate(p);
		else
			sysalloc::deallocate(p, cb);
	}
};

typedef pools_alloc<NS_BOOST_MEMORY_POLICY::stdlib> pools;
typedef pools_alloc<NS_BOOST_MEMORY_POLICY::scoped> scoped_pools;

// -------------------------------------------------------------------------

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_POOLS_HPP */
