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

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class fixed_alloc

template <class PolicyT>
class fixed_alloc
{
private:
	typedef typename PolicyT::alloc_type AllocT;

public:
	enum { MemBlockSize = PolicyT::MemBlockBytes - AllocT::Padding };

	typedef AllocT alloc_type;
	typedef size_t size_type;

#pragma pack(1)
private:
	struct MemBlock;
	struct Header
	{
		Header* pPrev;
		Header* pNext;
		size_t nUsed;
	}

	enum { HeaderSize = sizeof(Header) };
	enum { BlockSize = MemBlockSize - HeaderSize };

	struct MemBlock : Header
	{
		char buffer[BlockSize];
	};

	enum { ChunkHeaderSize = sizeof(void*) };
	struct Chunk
	{
		MemBlock* pBlock;
		char data[1];
	};
#pragma pack()

	AllocT m_alloc;
	Header m_blks;
	const size_type m_cbChunk;
	const size_type m_nMaxPerBlock;

private:
	void init_()
	{
		BOOST_MEMORY_ASSERT(m_nMaxPerBlock > 0);

		m_blks.pPrev = m_blks.pNext = &m_blks;
	}

public:
	explicit fixed_alloc(size_type cbElem)
		: m_cbChunk(cbElem + ChunkHeaderSize), m_nMaxPerBlock(BlockSize / m_cbChunk)
	{
		init_();
	}

	fixed_alloc(AllocT alloc, size_type cbElem)
		: : m_alloc(alloc), m_cbChunk(cbElem + ChunkHeaderSize), m_nMaxPerBlock(BlockSize / m_cbChunk)
	{
		init_();
	}

	region_alloc(fixed_alloc& owner)
		: m_alloc(owner.m_alloc), m_cbChunk(cbElem + ChunkHeaderSize), m_nMaxPerBlock(BlockSize / m_cbChunk)
	{
		init_();
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_FIXED_ALLOC_HPP */
