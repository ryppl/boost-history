//
//  boost/memory/gc_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef _BOOST_MEMORY_GC_ALLOC_HPP_
#define _BOOST_MEMORY_GC_ALLOC_HPP_

#ifndef _BOOST_MEMORY_SCOPED_ALLOC_HPP_
#include "scoped_alloc.hpp"
#endif

_NS_BOOST_BEGIN

// -------------------------------------------------------------------------
// class gen_alloc

#ifndef MAX
#define MAX(a, b)		((a) < (b) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b)		((a) < (b) ? (a) : (b))
#endif

template <class _Policy>
class gen_alloc
{
private:
	typedef typename _Policy::allocator_type _Alloc;
	typedef typename _Policy::huge_gc_allocator _HugeGCAlloc;
	typedef size_t _HeaderSizeT;

public:
	enum { MemBlockSize = _Policy::MemBlockSize };
	enum { IsGCAllocator = TRUE };

	typedef _Alloc allocator_type;
	typedef _HugeGCAlloc huge_gc_allocator;

#pragma pack(1)
private:
	enum _MemNodeType {
		nodeFree = 0,
		nodeAlloced = 1,
		nodeAllocedWithDestructor = 3,
	};
	
	struct _MemHeader
	{
		_HeaderSizeT cbSize	 : 30;
		_HeaderSizeT blkType : 2;
	};

	struct _MemHeaderEx;
	struct _DestroyInfo
	{
		_MemHeaderEx* pPrev;
		destructor_t fnDestroy;
	};

	struct _MemHeaderEx // = _MemHeader + _DestroyInfo
	{
		_HeaderSizeT cbSize	 : 30;
		_HeaderSizeT blkType : 2;
		
		_MemHeaderEx* pPrev;
		destructor_t fnDestroy;

		void BOOST_MEMORY_CALL destruct() {
			if (blkType == nodeAllocedWithDestructor) {
				blkType = nodeFree;
				fnDestroy(this + 1);
			}
		}
	};

	friend struct _MemHeaderEx;

	struct _FreeMemHeader
	{
		_HeaderSizeT cbSize;
		_HeaderSizeT BOOST_MEMORY_CALL getBlockType() const {
			return ((_MemHeader*)this)->blkType;
		}
		char* BOOST_MEMORY_CALL getData() const {
			return (char*)(this + 1);
		}
	};

	struct _MemBlock
	{
		_MemBlock* pPrev;
		char buffer[1];
		
		class Enumerator
		{
		private:
			char* m_start;
			char* m_last;

		public:
			Enumerator(_Alloc& alloc, _MemBlock* block) {
				m_start = block->buffer;
				m_last = (char*)block + alloc.alloc_size(block);
			}
			_MemHeader* BOOST_MEMORY_CALL first() const {
				return (_MemHeader*)m_start;
			}
			_MemHeader* BOOST_MEMORY_CALL next() {
				m_start += sizeof(_MemHeader) + ((_MemHeader*)m_start)->cbSize;
				return (_MemHeader*)m_start;
			}
			bool BOOST_MEMORY_CALL done() const {
				return m_start >= m_last;
			}
		};
	};

	struct _FreeListNode
	{
		_HeaderSizeT cbSize;
		_FreeListNode* pPrev;
	};
#pragma pack()
	
	char* m_begin;
	char* m_end;
	_Alloc m_alloc;
	_MemHeaderEx* m_destroyChain;
	_MemBlock* m_blockList;
	_FreeListNode* m_freeList;
	_HugeGCAlloc m_hugeAlloc;

private:
	const gen_alloc& operator=(const gen_alloc&);

	static bool BOOST_MEMORY_CALL _IsValid(void* obj, destructor_t fn)
	{
		_MemHeaderEx* node = (_MemHeaderEx*)obj - 1;
		BOOST_MEMORY_ASSERT(node->fnDestroy == fn);
		BOOST_MEMORY_ASSERT(node->cbSize == sizeof(Type) + sizeof(_DestroyInfo));
		BOOST_MEMORY_ASSERT(node->blkType == nodeAllocedWithDestructor);
		return node->fnDestroy == fn &&
			node->cbSize == sizeof(Type) + sizeof(_DestroyInfo) &&
			node->blkType == nodeAllocedWithDestructor;
	}

	static bool BOOST_MEMORY_CALL _IsValid(void* obj, int fnZero)
	{
		_MemHeader* node = (_MemHeader*)obj - 1;
		BOOST_MEMORY_ASSERT(node->cbSize == sizeof(Type));
		BOOST_MEMORY_ASSERT(node->blkType == nodeAlloced);
		return node->cbSize == sizeof(Type) && node->blkType == nodeAlloced;
	}

	template <class Type>
	static bool BOOST_MEMORY_CALL _IsValid(Type* obj)
	{
		return _IsValid(obj, destructor_traits<Type>::destruct);
	}

	template <class Type>
	static bool BOOST_MEMORY_CALL _IsValidArray(Type* array, size_t count)
	{
		void* buf = destructor_traits<Type>::getArrayBuffer(array);
		if (buf == array)
		{
			return _IsValid(buf, sizeof(Type)*count);
		}
		else
		{
			size_t count1 = destructor_traits<Type>::getArraySize(array);
			BOOST_MEMORY_ASSERT(count1 == count);
			bool fValid = _IsValid(buf, destructor_traits<Type>::destructArray);
			return count1 == count && fValid;
		}
	}

	void BOOST_MEMORY_CALL _Travel() const
	{
		_MemBlock* pHeader = m_blockList;
		while (pHeader)
		{
			_MemBlock::Enumerator coll(pHeader->pPrev);
			pHeader = pHeader->pPrev;
			for (_MemHeader* it = coll.first(); !coll.done(); it = coll.next())
				it;
		}
	}

	_MemBlock* BOOST_MEMORY_CALL _NewMemBlock(size_t cbBlock)
	{
		_MemBlock* pNew = (_MemBlock*)m_alloc.allocate(cbBlock);
		pNew->pPrev = m_blockList;
		m_blockList = pNew;
		return pNew;
	}

	void BOOST_MEMORY_CALL _Init()
	{
		_MemBlock* pNew = _NewMemBlock(MemBlockSize);
		_MemHeader* p = (_MemHeader*)pNew->buffer;
		p->blkType = nodeFree;
		m_begin = (char*)(p + 1);
		m_end = (char*)pNew + m_alloc.alloc_size(pNew);
	}

private:
	enum { HeaderSize = sizeof(void*) };
	enum { BlockSize = MemBlockSize - HeaderSize };
	enum { AllocSizeBig = MIN(_Policy::AllocSizeBig, BlockSize/2) };
	enum { AllocSizeHuge = (1 << 30) };
	enum { RecycleSizeMin = MAX(_Policy::RecycleSizeMin, 128) };

public:
	gen_alloc() : m_blockList(NULL), m_freeList(NULL), m_destroyChain(NULL)
	{
		_Init();
	}
	explicit gen_alloc(_Alloc alloc)
		: m_alloc(alloc), m_blockList(NULL), m_freeList(NULL), m_destroyChain(NULL)
	{
		_Init();
	}
	explicit gen_alloc(gen_alloc& owner)
		: m_alloc(owner.m_alloc), m_blockList(NULL), m_freeList(NULL), m_destroyChain(NULL)
	{
		_Init();
	}

	~gen_alloc()
	{
		clear();
	}

	void BOOST_MEMORY_CALL swap(gen_alloc& o)
	{
		std::swap(m_begin, o.m_begin);
		std::swap(m_end, o.m_end);
		std::swap(m_blockList, o.m_blockList);
		std::swap(m_freeList, o.m_freeList);
		std::swap(m_destroyChain, o.m_destroyChain);
		m_alloc.swap(o.m_alloc);
		m_hugeAlloc.swap(o.m_hugeAlloc);
	}

	void BOOST_MEMORY_CALL clear()
	{
		m_hugeAlloc.clear();
		while (m_destroyChain)
		{
			_MemHeaderEx* curr = m_destroyChain;
			m_destroyChain = m_destroyChain->pPrev;
			curr->destruct();
		}
		_MemBlock* pHeader = m_blockList;
		while (pHeader)
		{
			_MemBlock* pTemp = pHeader->pPrev;
			m_alloc.deallocate(pHeader);
			pHeader = pTemp;
		}
		m_begin = m_end = NULL;
		m_blockList = NULL;
		m_freeList = NULL;
	}

	template <class Type>
	void BOOST_MEMORY_CALL destroy(Type* obj)
	{
		BOOST_MEMORY_ASSERT( _IsValid(obj) );

		_MemHeader* p = (_MemHeader*)obj - 1;
		p->blkType = nodeFree;
		obj->~Type();
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
		BOOST_MEMORY_ASSERT( _IsValidArray(obj, count) );

		void* buf = destructor_traits<Type>::getArrayBuffer(array);
		_MemHeader* p = (_MemHeader*)buf - 1;
		p->blkType = nodeFree;
		destructor_traits<Type>::destructArrayN(array, count);
	}

	void* BOOST_MEMORY_CALL allocate(size_t cbData)
	{
		const size_t cb = cbData + sizeof(_MemHeader);
		if ((size_t)(m_end - m_begin) < cb)
		{
			_MemBlock* pNew;
			if (cb >= AllocSizeBig)
			{
				if (cbData >= AllocSizeHuge)
					return m_hugeAlloc.allocate(cbData);

				if (cb >= BlockSize)
				{
					pNew = _NewMemBlock(cb + HeaderSize);
					m_blockList->pPrev = pNew;
					m_blockList = pNew;
					_MemHeader* p = (_MemHeader*)pNew->buffer;
					p->blkType = nodeAlloced;
					p->cbSize = cbData;
					return p + 1;
				}
				pNew = _NewMemBlock(MemBlockSize);
			}
			else
			{
				pNew = _NewMemBlock(MemBlockSize);
			}
			_FreeMemHeader* old = (_FreeMemHeader*)m_begin - 1;
			BOOST_MEMORY_ASSERT(old->getBlockType() == nodeFree);
			old->cbSize = m_end - m_begin;
			_MemHeader* p = (_MemHeader*)pNew->buffer;
			p->blkType = nodeFree;
			m_begin = (char*)(p + 1);
			m_end = (char*)pNew + m_alloc.alloc_size(pNew);
		}
		_MemHeader* pAlloc = (_MemHeader*)(m_end -= cb);
		pAlloc->blkType = nodeAlloced;
		pAlloc->cbSize = cbData;
		return pAlloc + 1;
	}

	void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn)
	{
		_DestroyInfo* pNode = (_DestroyInfo*)allocate(sizeof(_DestroyInfo) + cb);
		pNode->fnDestroy = fn;
		pNode->pPrev = m_destroyChain;
		m_destroyChain = (_MemHeaderEx*)((char*)pNode - sizeof(_MemHeader));
		m_destroyChain->blkType = nodeAllocedWithDestructor;
		return pNode + 1;
	}

	void* BOOST_MEMORY_CALL allocate(size_t cb, int fnZero)
	{
		return allocate(cb);
	}

	void BOOST_MEMORY_CALL deallocate(void* pData, size_t cbData)
	{
		if (cbData >= AllocSizeHuge)
			return m_hugeAlloc.deallocate(p, cbData);

		_MemHeader* p = (_MemHeader*)pData - 1;
		BOOST_MEMORY_ASSERT(p->cbSize == cbData);
		BOOST_MEMORY_ASSERT(p->blkType == nodeAlloced);

		p->blkType = nodeFree;
	}

	_BOOST_FAKE_DBG_ALLOCATE();
};

// -------------------------------------------------------------------------
// class gc_alloc

typedef gen_alloc<policy::pool> gc_alloc;

// -------------------------------------------------------------------------
// $Log: gc_alloc.hpp,v $

_NS_BOOST_END

#endif /* _BOOST_MEMORY_GC_ALLOC_HPP_ */
