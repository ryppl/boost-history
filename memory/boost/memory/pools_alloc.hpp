//
//  boost/memory/pools_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_POOLS_ALLOC_HPP
#define BOOST_MEMORY_POOLS_ALLOC_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

#ifndef BOOST_MEMORY_BASIC_HPP
#include "fixed_alloc.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class simple_alloc

#pragma pack(1)

template <class PolicyT>
class simple_alloc
{
private:
	simple_alloc(const simple_alloc&);
	const simple_alloc& operator=(const simple_alloc&);

	void swap(simple_alloc& o); // NO SWAP: dcl_list

public:
	typedef typename PolicyT::system_alloc_type alloc_type;
	typedef size_t size_type;
	
private:
	struct MemHeader : dcl_list_node<MemHeader> {};

	dcl_list<MemHeader> m_chunks;

public:
	simple_alloc() {}
	~simple_alloc() {
		clear();
	}
	
	void BOOST_MEMORY_CALL clear()
	{
		MemHeader* nextChunk;
		for (MemHeader* chunk = m_chunks.first(); !m_chunks.done(chunk); chunk = nextChunk)
		{
			nextChunk = chunk->next();
			alloc_type::deallocate(chunk);
		}
		m_chunks.clear();
	}

	void* BOOST_MEMORY_CALL allocate(size_type cb) {
		MemHeader* p = (MemHeader*)alloc_type::allocate(cb + sizeof(MemHeader));
		m_chunks.push_front(p);
		return p + 1;
	}

	void BOOST_MEMORY_CALL deallocate(void* ptr) {
		MemHeader* p = (MemHeader*)ptr - 1;
		p->erase();
		alloc_type::deallocate(p);
	}
};

#pragma pack()

// -------------------------------------------------------------------------
// class pools_alloc

#pragma pack(1)

template <class PolicyT, int nPool = 16>
class pools_alloc
{
private:
	pools_alloc(const pools_alloc&);
	const pools_alloc& operator=(const pools_alloc&);

	void swap(pools_alloc& o); // NO SWAP: dcl_list

private:
	typedef simple_alloc<PolicyT> LargeAllocT;

public:
	typedef fixed_alloc<PolicyT> pool_type;
	typedef typename pool_type::alloc_type alloc_type;
	typedef typename pool_type::size_type size_type;

	enum { MemBlockSize = PolicyT::MemBlockBytes - alloc_type::Padding };

private:
	enum { ALIGN_BITS = 3 };
	enum { NPOOL = nPool };
	enum { ALIGN = 1 << ALIGN_BITS };
	enum { MIN_BYTES = ALIGN };
	enum { MAX_BYTES = ALIGN * NPOOL };
	
	enum { POOL_BYTES = sizeof(pool_type) };
	enum { POOLS_TOTAL_BYTES = POOL_BYTES * NPOOL };

	//
	// pool 0: 1 ~ ALIGN
	// pool 1: ALIGN + 1 ~ 2*ALIGN
	// pool 2: ...
	//
	char m_pools[POOLS_TOTAL_BYTES];	
	LargeAllocT m_large_alloc;

public:
	pools_alloc()
	{
		BOOST_MEMORY_ASSERT(ALIGN >= sizeof(void*));
		
		char* p = m_pools;
		for (size_t cb = MIN_BYTES; cb <= MAX_BYTES; cb += ALIGN)
		{
			new(p) pool_type(cb);
			p += POOL_BYTES;
		}
	}
	
	explicit pools_alloc(alloc_type alloc)
	{
		BOOST_MEMORY_ASSERT(ALIGN >= sizeof(void*));
		
		char* p = m_pools;
		for (size_t cb = MIN_BYTES; cb <= MAX_BYTES; cb += ALIGN)
		{
			new(p) pool_type(alloc, cb);
			p += POOL_BYTES;
		}
	}

	~pools_alloc()
	{
		do_clear_();
	}

	alloc_type BOOST_MEMORY_CALL get_alloc() const
	{
		return ((pool_type*)m_pools)->get_alloc();
	}

	pool_type& BOOST_MEMORY_CALL get_pool(size_type cb) const
	{
		BOOST_MEMORY_ASSERT(has_pool(cb));

		const size_type index = (cb - 1) >> ALIGN_BITS;
		return *((pool_type*)m_pools + index);
	}

	bool BOOST_MEMORY_CALL has_pool(size_type cb) const
	{
		const size_type index = (cb - 1) >> ALIGN_BITS;
		return index < (size_type)NPOOL;
	}

private:
	void BOOST_MEMORY_CALL do_clear_()
	{
		char* pEnd = m_pools + POOLS_TOTAL_BYTES;
		for (char* p = m_pools; p != pEnd; p += POOL_BYTES)
		{
			((pool_type*)p)->clear();
		}
	}

public:
	void BOOST_MEMORY_CALL clear()
	{
		do_clear_();
		m_large_alloc.clear();
	}

private:
	void* BOOST_MEMORY_CALL do_allocate_(size_type cb)
	{
		BOOST_MEMORY_ASSERT(cb == 0 || cb > MAX_BYTES);
		
		if (cb != 0)
			return m_large_alloc.allocate(cb);
		else
			return this + 1;
	}

public:
	void* BOOST_MEMORY_CALL allocate(size_type cb)
	{
		const size_type index = (cb - 1) >> ALIGN_BITS;
		if (index < (size_type)NPOOL)
			return ((pool_type*)m_pools + index)->allocate();
		else
			return do_allocate_(cb);
	}

public:
	void BOOST_MEMORY_CALL deallocate(void* p, size_t cb)
	{
		const size_type index = (cb - 1) >> ALIGN_BITS;
		if (index < (size_type)NPOOL)
			((pool_type*)m_pools + index)->deallocate(p);
		else if (cb != 0)
			m_large_alloc.deallocate(p);
	}

public:
	void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize)
	{
		const size_type oldIndex = (oldSize - 1) >> ALIGN_BITS;
		const size_type newIndex = (newSize - 1) >> ALIGN_BITS;
		
		if (oldIndex == newIndex && oldIndex < (size_type)NPOOL)
			return p;
		void* p2 = allocate(newSize);
		memcpy(p2, p, MIN(oldSize, newSize));
		deallocate(p, oldSize);
		return p2;
	}
};

#pragma pack()

// -------------------------------------------------------------------------
// $Log: pools_alloc.hpp,v $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_POOLS_ALLOC_HPP */
