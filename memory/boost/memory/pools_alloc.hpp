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

template <class PolicyT>
class pools_alloc
{
private:
	pools_alloc(const pools_alloc&);
	const pools_alloc& operator=(const pools_alloc&);

	void swap(pools_alloc& o); // NO SWAP: dcl_list

private:
	typedef fixed_alloc<PolicyT> FixedAllocT;
	typedef simple_alloc<PolicyT> LargeAllocT;

public:
	typedef typename FixedAllocT::alloc_type alloc_type;
	typedef typename FixedAllocT::size_type size_type;

	enum { MemBlockSize = PolicyT::MemBlockBytes - alloc_type::Padding };

private:
	enum { ALIGN_BITS = 3 };
	enum { NPOOL = 16 };
	enum { ALIGN = 1 << ALIGN_BITS };
	enum { MIN_BYTES = ALIGN };
	enum { MAX_BYTES = ALIGN * NPOOL };
	
	enum { POOL_BYTES = sizeof(FixedAllocT) };
	enum { POOLS_TOTAL_BYTES = POOL_BYTES * NPOOL };

	//
	// pool 0: 1 ~ ALIGN
	// pool 1: ALIGN + 1 ~ 2*ALIGN
	// pool 2: ...
	//
	char m_pools[POOLS_TOTAL_BYTES];
	
	alloc_type m_alloc;
	LargeAllocT m_large_alloc;

private:
	void BOOST_MEMORY_CALL init_()
	{
		BOOST_MEMORY_ASSERT(ALIGN >= sizeof(void*));
		
		char* p = m_pools;
		for (size_t cb = MIN_BYTES; cb <= MAX_BYTES; cb += ALIGN)
		{
			new(p) FixedAllocT(cb);
			p += POOL_BYTES;
		}
	}
	
public:
	pools_alloc()
	{
		init_();
	}
	
	explicit pools_alloc(alloc_type alloc) : m_alloc(alloc)
	{
		init_();
	}

	~pools_alloc()
	{
		do_clear_();
	}

	alloc_type BOOST_MEMORY_CALL get_alloc() const
	{
		return m_alloc;
	}
	
private:
	void BOOST_MEMORY_CALL do_clear_()
	{
		char* pEnd = m_pools + POOLS_TOTAL_BYTES;
		for (char* p = m_pools; p != pEnd; p += POOL_BYTES)
		{
			((FixedAllocT*)p)->clear(m_alloc);
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
			return ((FixedAllocT*)m_pools + index)->allocate(m_alloc);
		else
			return do_allocate_(cb);
	}

public:
	void BOOST_MEMORY_CALL deallocate(void* p, size_t cb)
	{
		const size_type index = (cb - 1) >> ALIGN_BITS;
		if (index < (size_type)NPOOL)
			((FixedAllocT*)m_pools + index)->deallocate(m_alloc, p);
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
