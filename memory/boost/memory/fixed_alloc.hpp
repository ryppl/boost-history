//
//  boost/memory/fixed_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_FIXED_ALLOC_HPP
#define BOOST_MEMORY_FIXED_ALLOC_HPP

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

#ifndef BOOST_MEMORY_CONTAINER_DCL_LIST_HPP
#include "container/dcl_list.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class fixed_alloc

#pragma pack(1)

#ifndef ROUND
#define ROUND(x, y)		(((x)+((y)-1)) & ~((y)-1))
#endif

#ifndef MAX
#define MAX(x, y)		((x) > (y) ? (x) : (y))
#define MIN(x, y)		((x) < (y) ? (x) : (y))
#endif

#ifndef BOOST_MEMORY_DBG_FILL
#if defined(_DEBUG)
#define BOOST_MEMORY_DBG_FILL(p, cb)	memset(p, 0xcd, cb)
#else
#define BOOST_MEMORY_DBG_FILL(p, cb)
#endif
#endif

template <class PolicyT>
class fixed_alloc
{
private:
	fixed_alloc(const fixed_alloc&);
	void operator=(const fixed_alloc&);

public:
	typedef typename PolicyT::alloc_type alloc_type;
	typedef size_t size_type;

	enum { MemBlockSize = PolicyT::MemBlockBytes - alloc_type::Padding };

protected:
	struct MemBlock;
	friend struct MemBlock;
	
	struct Header : public dcl_list_node<MemBlock>
	{
		size_t nUsed;
	};

	enum { HeaderSize = sizeof(Header) };
	enum { BlockSize = MemBlockSize - HeaderSize };

	struct MemBlock : Header
	{
		char buffer[BlockSize];
	};

	struct ChunkHeader
	{
		MemBlock* pBlock;
	};

	struct FreeChunk : public dcl_list_node<FreeChunk>
	{
	};

	enum { ChunkHeaderSize = sizeof(ChunkHeader) };
	enum { MinElemBytes = sizeof(FreeChunk) };

	dcl_list<FreeChunk> m_freelist;
	dcl_list<MemBlock> m_blks;
	size_type m_cbChunk;
	MemBlock* m_lastBlock;

public:
	explicit fixed_alloc(size_type cbElem)
	{
		cbElem = ROUND(cbElem, sizeof(void*));
		m_cbChunk = MAX(cbElem, MinElemBytes) + ChunkHeaderSize;
		m_lastBlock = NULL;

		BOOST_MEMORY_ASSERT(BlockSize/m_cbChunk > 0);
	}

#if defined(_DEBUG)
	~fixed_alloc()
	{
		BOOST_MEMORY_ASSERT(m_blks.empty());
		BOOST_MEMORY_ASSERT(m_freelist.empty());
	}
#endif

	size_type BOOST_MEMORY_CALL element_size() const
	{
		return m_cbChunk - ChunkHeaderSize;
	}

	void BOOST_MEMORY_CALL clear(alloc_type alloc)
	{
		MemBlock* nextBlk;
		for (MemBlock* blk = this->m_blks.first(); !this->m_blks.done(blk); blk = nextBlk)
		{
			nextBlk = blk->next();
			alloc.deallocate(blk);
		}
		this->m_blks.clear();
		this->m_freelist.clear();
	}

private:
	__forceinline MemBlock*& BOOST_MEMORY_CALL chunkHeader_(void* const p)
	{
		return ((ChunkHeader*)p - 1)->pBlock;
	}

	void BOOST_MEMORY_CALL do_allocate_block_(alloc_type alloc)
	{
		MemBlock* const blk = (MemBlock*)alloc.allocate(sizeof(MemBlock));
		m_blks.push_front(blk);
		m_lastBlock = blk;

		blk->nUsed = 0;

		char* p = blk->buffer + ChunkHeaderSize;
		for (size_type i = BlockSize/m_cbChunk; i--; p += m_cbChunk)
		{
			chunkHeader_(p) = blk;
			m_freelist.push_front((FreeChunk*)p);
		}

		BOOST_MEMORY_ASSERT(!m_freelist.empty());
	}

	void BOOST_MEMORY_CALL do_deallocate_block_(alloc_type alloc, MemBlock* const blk)
	{
		char* p = blk->buffer + ChunkHeaderSize;
		for (size_type i = BlockSize/m_cbChunk; i--; p += m_cbChunk)
		{
			((FreeChunk*)p)->erase();
		}

		blk->erase();

		alloc.deallocate(blk);
	}

public:
	__forceinline void* BOOST_MEMORY_CALL allocate(alloc_type alloc)
	{
		if (m_freelist.empty())
			do_allocate_block_(alloc);

		void* p = &m_freelist.front();
		++chunkHeader_(p)->nUsed;
		m_freelist.pop_front();
		
		BOOST_MEMORY_DBG_FILL(p, element_size());
		return p;
	}

	__forceinline void BOOST_MEMORY_CALL deallocate(alloc_type alloc, void* const p)
	{
		MemBlock* const blk = chunkHeader_(p);

		BOOST_MEMORY_ASSERT(blk->nUsed > 0 && blk->nUsed <= BlockSize/m_cbChunk);
		BOOST_MEMORY_DBG_FILL(p, element_size());

		m_freelist.push_front((FreeChunk*)p);
		if (--blk->nUsed == 0 && blk != m_lastBlock)
			do_deallocate_block_(alloc, blk);
	}
};

#pragma pack()

// -------------------------------------------------------------------------
// $Log: $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_FIXED_ALLOC_HPP */
