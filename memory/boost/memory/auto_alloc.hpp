//
//  boost/memory/auto_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef __BOOST_MEMORY_AUTO_ALLOC_HPP__
#define __BOOST_MEMORY_AUTO_ALLOC_HPP__

#ifndef __BOOST_MEMORY_SYSTEM_ALLOC_HPP__
#include "system_alloc.hpp"
#endif

#ifndef __BOOST_MEMORY_POLICY_HPP__
#include "policy.hpp"
#endif

__NS_BOOST_BEGIN

// -------------------------------------------------------------------------
// class region_alloc

template <class _Policy>
class region_alloc
{
private:
	typedef typename _Policy::allocator_type _Alloc;

public:
	enum { MemBlockSize = _Policy::MemBlockSize };
	enum { IsGCAllocator = TRUE };

	typedef _Alloc allocator_type;

private:
	enum { HeaderSize = sizeof(void*) };
	enum { BlockSize = MemBlockSize - HeaderSize };

#pragma pack(1)
private:
	struct _MemBlock;
	friend struct _MemBlock;

	struct _MemBlock
	{
		_MemBlock* pPrev;
		char buffer[BlockSize];
	};
	struct _DestroyNode
	{
		_DestroyNode* pPrev;
		destructor_t fnDestroy;
	};
#pragma pack()
	
	char* m_begin;
	char* m_end;
	_DestroyNode* m_destroyChain;
	_Alloc m_alloc;

private:
	const region_alloc& operator=(const region_alloc&);

	_MemBlock* BOOST_MEMORY_CALL _ChainHeader() const
	{
		return (_MemBlock*)(m_begin - HeaderSize);
	}

	void BOOST_MEMORY_CALL _Init()
	{
		_MemBlock* pNew = (_MemBlock*)m_alloc.allocate(sizeof(_MemBlock));
		pNew->pPrev = NULL;
		m_begin = pNew->buffer;
		m_end = (char*)pNew + m_alloc.alloc_size(pNew);
	}

public:
	region_alloc() : m_destroyChain(NULL)
	{
		_Init();
	}
	explicit region_alloc(_Alloc alloc) : m_alloc(alloc), m_destroyChain(NULL)
	{
		_Init();
	}
	explicit region_alloc(region_alloc& owner)
		: m_alloc(owner.m_alloc), m_destroyChain(NULL)
	{
		_Init();
	}

	~region_alloc()
	{
		clear();
	}

	void BOOST_MEMORY_CALL swap(region_alloc& o)
	{
		std::swap(m_begin, o.m_begin);
		std::swap(m_end, o.m_end);
		std::swap(m_destroyChain, o.m_destroyChain);
		m_alloc.swap(o.m_alloc);
	}

	void BOOST_MEMORY_CALL clear()
	{
		while (m_destroyChain)
		{
			_DestroyNode* curr = m_destroyChain;
			m_destroyChain = m_destroyChain->pPrev;
			curr->fnDestroy(curr + 1);
		}
		_MemBlock* pHeader = _ChainHeader();
		while (pHeader)
		{
			_MemBlock* pTemp = pHeader->pPrev;
			m_alloc.deallocate(pHeader);
			pHeader = pTemp;
		}
		m_begin = m_end = (char*)HeaderSize;
	}

	template <class Type>
	void BOOST_MEMORY_CALL destroy(Type* obj)
	{
		// no action
	}

	template <class Type>
	Type* BOOST_MEMORY_CALL newArray(size_t count, Type* zero)
	{
		Type* array = (Type*)destructor_traits<Type>::allocArray(*this, count);
		return constructor_traits<Type>::constructArray(array, count);
	}

	template <class Type>
	void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count)
	{
		// no action
	}

	void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		if ((size_t)(m_end - m_begin) < cb)
		{
			if (cb >= BlockSize)
			{
				_MemBlock* pHeader = _ChainHeader();
				_MemBlock* pNew = (_MemBlock*)m_alloc.allocate(HeaderSize + cb);
				if (pHeader)
				{
					pNew->pPrev = pHeader->pPrev;
					pHeader->pPrev = pNew;
				}
				else
				{
					m_end = m_begin = pNew->buffer;
					pNew->pPrev = NULL;
				}
				return pNew->buffer;
			}
			else
			{
				_MemBlock* pNew = (_MemBlock*)m_alloc.allocate(sizeof(_MemBlock));
				pNew->pPrev = _ChainHeader();
				m_begin = pNew->buffer;
				m_end = (char*)pNew + m_alloc.alloc_size(pNew);
			}
		}
		return m_end -= cb;
	}

	void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn)
	{
		_DestroyNode* pNode = (_DestroyNode*)allocate(sizeof(_DestroyNode) + cb);
		pNode->fnDestroy = fn;
		pNode->pPrev = m_destroyChain;
		m_destroyChain = pNode;
		return pNode + 1;
	}

	void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero)
	{
		return allocate(cb);
	}

	void* BOOST_MEMORY_CALL reallocate(void* p, size_t oldSize, size_t newSize)
	{
		if (oldSize >= newSize)
			return p;
		void* p2 = allocate(newSize);
		memcpy(p2, p, oldSize);
		return p2;
	}

	void BOOST_MEMORY_CALL deallocate(void* p, size_t cb)
	{
		// no action
	}

	__BOOST_FAKE_DBG_ALLOCATE();
};

// -------------------------------------------------------------------------
// class auto_alloc

typedef region_alloc<policy::sys> auto_alloc;

// -------------------------------------------------------------------------
// $Log: auto_alloc.hpp,v $

__NS_BOOST_END

#endif /* __BOOST_MEMORY_AUTO_ALLOC_HPP__ */
