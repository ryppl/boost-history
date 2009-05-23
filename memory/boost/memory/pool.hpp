//
//  boost/memory/pool_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_POOL_HPP
#define BOOST_MEMORY_POOL_HPP

#ifndef BOOST_MEMORY_FIXED_ALLOC_HPP
#include "fixed_alloc.hpp"
#endif

#ifndef BOOST_MEMORY_BLOCKPOOL_HPP
#include "block_pool.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class pool, scoped_pool

#pragma pack(1)

template <class PolicyT>
class pool_alloc : private fixed_alloc<PolicyT>
{
private:
	typedef fixed_alloc<PolicyT> PoolT;

	pool_alloc(const pool_alloc& o);
	void operator=(const pool_alloc& o);

	void swap(pool_alloc& o); // NO SWAP: dcl_list

public:
	typedef typename PoolT::alloc_type alloc_type;
	typedef typename PoolT::size_type size_type;
	
	using PoolT::element_size;

private:
	alloc_type m_alloc;
	
public:
	__forceinline explicit pool_alloc(size_type cbElem)
		: PoolT(cbElem) {
	}
	__forceinline explicit pool_alloc(alloc_type alloc, size_type cbElem)
		: PoolT(cbElem), m_alloc(alloc) {
	}
	__forceinline ~pool_alloc() {
		PoolT::clear(m_alloc);
	}
	
	__forceinline alloc_type BOOST_MEMORY_CALL get_alloc() const {
		return m_alloc;
	}

	__forceinline void BOOST_MEMORY_CALL clear() {
		PoolT::clear(m_alloc);
	}
	
	__forceinline void* BOOST_MEMORY_CALL allocate() {
		return PoolT::allocate(m_alloc);
	}

	__forceinline void BOOST_MEMORY_CALL deallocate(void* p) {
		return PoolT::deallocate(m_alloc, p);
	}
};

typedef pool_alloc<NS_BOOST_MEMORY_POLICY::stdlib> pool;
typedef pool_alloc<NS_BOOST_MEMORY_POLICY::scoped> scoped_pool;

#pragma pack()

// -------------------------------------------------------------------------
// $Log: $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_POOL_HPP */
