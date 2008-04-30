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

#pragma warning(disable:4786)

#ifndef _BOOST_MEMORY_SCOPED_ALLOC_HPP_
#include "scoped_alloc.hpp"
#endif

#ifndef _BOOST_MEMORY_STL_QUEUE_HPP_
#include "stl/queue.hpp" // boost::priority_queue
#endif

#if !defined(_DEQUE_) && !defined(_DEQUE)
#include <deque> // std::deque
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
			_MemHeader* BOOST_MEMORY_CALL current() const {
				return (_MemHeader*)m_start;
			}
			_MemHeader* BOOST_MEMORY_CALL next() {
				BOOST_MEMORY_ASSERT(!done());
				m_start += sizeof(_MemHeader) + ((_MemHeader*)m_start)->cbSize;
				return (_MemHeader*)m_start;
			}
			bool BOOST_MEMORY_CALL done() const {
				return m_start >= m_last;
			}
		};
	};
#pragma pack()

	struct _Pred : std::binary_function<_FreeMemHeader*, _FreeMemHeader*, bool>
	{
		bool operator()(_FreeMemHeader* a, _FreeMemHeader* b) const {
			return a->cbSize < b->cbSize;
		}
	};
	typedef std::deque<_FreeMemHeader*> _Cont;
	typedef boost::priority_queue<_FreeMemHeader*, _Cont, _Pred> _PriorityQ;
	
	char* m_begin;
	char* m_end;
	_Alloc m_alloc;
	_MemHeaderEx* m_destroyChain;
	_MemBlock* m_blockList;
	_PriorityQ m_freeList;
	_HugeGCAlloc m_hugeAlloc;
	size_t m_freeSize, m_GCLimitSize;
	static _FreeMemHeader _null;

private:
	enum { HeaderSize = sizeof(void*) };
	enum { BlockSize = MemBlockSize - HeaderSize };
	enum { _AllocSizeBigDef = MAX(_Policy::AllocSizeBig, BlockSize/4) };
	enum { _AllocSizeHugeDef = MAX(_Policy::AllocSizeHuge, 64*1024) };
	enum { _GCLimitSizeDef = MAX(_Policy::GCLimitSizeDef, 64*1024) };
	enum { AllocSizeBig = MIN(_AllocSizeBigDef, BlockSize/2) };
	enum { AllocSizeHuge = MIN(_AllocSizeHugeDef, (1 << 29)) };
	enum { GCLimitSizeDef = MIN(_GCLimitSizeDef, (1 << 29)) };
	enum { RecycleSizeMin = MAX(_Policy::RecycleSizeMin, 128) };

private:
	const gen_alloc& operator=(const gen_alloc&);
	
	static bool BOOST_MEMORY_CALL _IsEqual(size_t cbAlloc, size_t cb)
	{
		return cbAlloc >= cb && cb + 64 > cbAlloc;
	}

	static bool BOOST_MEMORY_CALL _IsValid(void* obj, size_t cb, destructor_t fn)
	{
		_MemHeaderEx* node = (_MemHeaderEx*)obj - 1;
		BOOST_MEMORY_ASSERT(node->fnDestroy == fn);
		BOOST_MEMORY_ASSERT(_IsEqual(node->cbSize, cb + sizeof(_DestroyInfo)));
		BOOST_MEMORY_ASSERT(node->blkType == nodeAllocedWithDestructor);
		return node->fnDestroy == fn &&
			_IsEqual(node->cbSize, cb + sizeof(_DestroyInfo)) &&
			node->blkType == nodeAllocedWithDestructor;
	}

	static bool BOOST_MEMORY_CALL _IsValid(void* obj, size_t cb, int fnZero)
	{
		_MemHeader* node = (_MemHeader*)obj - 1;
		BOOST_MEMORY_ASSERT(_IsEqual(node->cbSize, cb));
		BOOST_MEMORY_ASSERT(node->blkType == nodeAlloced);
		return _IsEqual(node->cbSize, cb) && node->blkType == nodeAlloced;
	}

	template <class Type>
	static bool BOOST_MEMORY_CALL _IsValid(Type* obj)
	{
		return _IsValid(obj, sizeof(Type), destructor_traits<Type>::destruct);
	}

	template <class Type>
	static bool BOOST_MEMORY_CALL _IsValidArray(Type* array, size_t count)
	{
		void* buf = destructor_traits<Type>::getArrayBuffer(array);
		size_t cb = destructor_traits<Type>::getArrayAllocSize(count);
		if (buf == array)
		{
			return _IsValid(buf, cb, 0);
		}
		else
		{
			size_t count1 = destructor_traits<Type>::getArraySize(array);
			BOOST_MEMORY_ASSERT(count1 == count);
			bool fValid = _IsValid(buf, cb, destructor_traits<Type>::destructArray);
			return count1 == count && fValid;
		}
	}

public:
	void BOOST_MEMORY_CALL force_gc()
	{
		// 0. Prepare

		// 0.1. Commit current block:
		_CommitCurrentBlock();
		m_begin = m_end = _null.getData();

		// 0.2. Clear destroy chain
		m_destroyChain = NULL;

		// 0.3. Clear free list
		m_freeList.clear();
		m_freeSize = 0;

		// 1. Collect free nodes
		_MemBlock* pHeader = m_blockList;
		while (pHeader)
		{
			typename _MemBlock::Enumerator coll(m_alloc, pHeader);
			pHeader = pHeader->pPrev;
			for (;;)
			{
				if (coll.done())
					break;
				_MemHeader* it = coll.current();
				if (it->blkType == nodeFree)
				{
					// merge nodes marked with nodeFree
					UINT cbFree = it->cbSize;
					for (;;) {
						_MemHeader* it2 = coll.next();
						if (coll.done() || it2->blkType != nodeFree)
							break;
						cbFree += it2->cbSize;
					}
					it->cbSize = cbFree;
					if (cbFree >= RecycleSizeMin)
						m_freeList.push((_FreeMemHeader*)it);
					if (coll.done())
						break;
					it = coll.current();
				}
				if (it->blkType == nodeAllocedWithDestructor)
				{
					_MemHeaderEx* itEx = (_MemHeaderEx*)it;
					itEx->pPrev = m_destroyChain;
					m_destroyChain = itEx;
				}
				// skip nodes marked with nodeAlloced
				coll.next();
			}
		}
	}
	
	void BOOST_MEMORY_CALL try_gc()
	{
		if (m_freeSize >= m_GCLimitSize)
			force_gc();
	}

	size_t BOOST_MEMORY_CALL get_free_size() const
	{
		return m_freeSize;
	}

	void BOOST_MEMORY_CALL set_gclim(size_t gcLimit)
	{
		m_GCLimitSize = gcLimit;
	}

private:
	void BOOST_MEMORY_CALL _CommitCurrentBlock()
	{
		_FreeMemHeader* old = (_FreeMemHeader*)m_begin - 1;
		BOOST_MEMORY_ASSERT(old->getBlockType() == nodeFree);
		old->cbSize = (m_end - m_begin);
	}

	_MemHeader* BOOST_MEMORY_CALL _NewBlock(size_t cbBlock)
	{
		_MemBlock* pBlock = (_MemBlock*)m_alloc.allocate(cbBlock);
		pBlock->pPrev = m_blockList;
		m_blockList = pBlock;

		_MemHeader* pNew = (_MemHeader*)pBlock->buffer;
		pNew->blkType = nodeFree;
		pNew->cbSize = m_alloc.alloc_size(pBlock) - (sizeof(_MemHeader) + HeaderSize);
		return pNew;
	}

	void BOOST_MEMORY_CALL _Init()
	{
		m_blockList = NULL;
		m_destroyChain = NULL;
		m_freeSize = 0;
		m_GCLimitSize = GCLimitSizeDef;

		_MemHeader* pNew = _NewBlock(MemBlockSize);
		m_begin = (char*)(pNew + 1);
		m_end = m_begin + pNew->cbSize;
	}

public:
	gen_alloc() {
		_Init();
	}
	explicit gen_alloc(_Alloc alloc) : m_alloc(alloc) {
		_Init();
	}
	explicit gen_alloc(gen_alloc& owner) : m_alloc(owner.m_alloc) {
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
		std::swap(m_destroyChain, o.m_destroyChain);
		std::swap(m_freeSize, o.m_freeSize);
		std::swap(m_GCLimitSize, o.m_GCLimitSize);
		m_alloc.swap(o.m_alloc);
		m_freeList.swap(o.m_freeList);
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
			_MemBlock* curr = pHeader;
			pHeader = pHeader->pPrev;
			m_alloc.deallocate(curr);
		}
		m_begin = m_end = _null.getData();
		m_blockList = NULL;
		m_freeList.clear();
		m_freeSize = 0;
	}

	void* BOOST_MEMORY_CALL allocate(size_t cbData)
	{
		const size_t cb = cbData + sizeof(_MemHeader);
		if ((size_t)(m_end - m_begin) < cb)
		{
			_MemHeader* pNew;
			if (cb >= AllocSizeBig)
			{
				if (cbData >= AllocSizeHuge)
					return m_hugeAlloc.allocate(cbData);

				if (cb >= BlockSize)
				{
					pNew = _NewBlock(cb + HeaderSize);
					pNew->blkType = nodeAlloced;
					return pNew + 1;
				}
				pNew = _NewBlock(MemBlockSize);
			}
			else
			{
				try_gc();
				_FreeMemHeader* p;
				if (m_freeList.empty() || (p = m_freeList.top())->cbSize < cb) {
					pNew = _NewBlock(MemBlockSize);
				}
				else {
					pNew = (_MemHeader*)p;
					m_freeList.pop();
				}
			}
			_CommitCurrentBlock();
			m_begin = (char*)(pNew + 1);
			m_end = m_begin + pNew->cbSize;
		}

		BOOST_MEMORY_ASSERT(m_end - m_begin >= cb);

		_MemHeader* pAlloc = (_MemHeader*)(m_end -= cb);
		pAlloc->blkType = nodeAlloced;
		pAlloc->cbSize = cbData;
		return pAlloc + 1;
	}

	void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn)
	{
		if (cb >= AllocSizeHuge)
			return m_hugeAlloc.allocate(cb, fn);
		
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
		{
			m_hugeAlloc.deallocate(pData, cbData);
		}
		else
		{
			_MemHeader* p = (_MemHeader*)pData - 1;
			BOOST_MEMORY_ASSERT(p->cbSize == cbData);
			BOOST_MEMORY_ASSERT(p->blkType == nodeAlloced);

			p->blkType = nodeFree;
			m_freeSize += cbData + sizeof(_MemHeader);
		}
	}

private:
	template <class Type>
	void BOOST_MEMORY_CALL _Destroy(Type* obj, destructor_t)
	{
		obj->~Type();
		_MemHeaderEx* p = (_MemHeaderEx*)obj - 1;
		p->blkType = nodeFree;
		m_freeSize += sizeof(Type) + sizeof(_MemHeaderEx);
	}

	template <class Type>
	void BOOST_MEMORY_CALL _Destroy(Type* obj, int)
	{
		_MemHeader* p = (_MemHeader*)obj - 1;
		p->blkType = nodeFree;
		m_freeSize += sizeof(Type) + sizeof(_MemHeader);
	}

	template <class Type>
	void BOOST_MEMORY_CALL _DestroyArray(Type* array, size_t count, destructor_t)
	{
		typedef destructor_traits<Type> _Traits;

		_Traits::destructArrayN(array, count);
		void* pData = _Traits::getArrayBuffer(array);
		_MemHeaderEx* p = (_MemHeaderEx*)pData - 1;
		p->blkType = nodeFree;
		m_freeSize += _Traits::getArrayAllocSize(count) + sizeof(_MemHeaderEx);
	}

	template <class Type>
	void BOOST_MEMORY_CALL _DestroyArray(Type* array, size_t count, int)
	{
		_MemHeader* p = (_MemHeader*)array - 1;
		p->blkType = nodeFree;
		m_freeSize += sizeof(Type) * count + sizeof(_MemHeader);
	}

public:
	template <class Type>
	void BOOST_MEMORY_CALL destroy(Type* obj)
	{
		BOOST_MEMORY_ASSERT( _IsValid(obj) );
		BOOST_MEMORY_ASSERT( sizeof(Type) < AllocSizeHuge );

		_Destroy(obj, destructor_traits<Type>::destruct);
	}

	template <class Type>
	Type* BOOST_MEMORY_CALL newArray(size_t count, Type* zero)
	{
		size_t cb = destructor_traits<Type>::getArrayAllocSize(count);
		if (cb >= AllocSizeHuge)
			return m_hugeAlloc.newArray(count, zero);

		Type* array = (Type*)destructor_traits<Type>::allocArray(*this, count);
		return constructor_traits<Type>::constructArray(array, count);
	}

	template <class Type>
	void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count)
	{
		BOOST_MEMORY_ASSERT( _IsValidArray(array, count) );

		size_t cb = destructor_traits<Type>::getArrayAllocSize(count);
		if (cb >= AllocSizeHuge)
		{
			m_hugeAlloc.destroyArray(array, count);
		}
		else
		{
			_DestroyArray(array, count, destructor_traits<Type>::destruct);
		}
	}

	_BOOST_FAKE_DBG_ALLOCATE();
};

template <class _Policy>
typename gen_alloc<_Policy>::_FreeMemHeader gen_alloc<_Policy>::_null;

// -------------------------------------------------------------------------
// class gc_alloc

typedef gen_alloc<policy::pool> gc_alloc;

// -------------------------------------------------------------------------
// $Log: gc_alloc.hpp,v $

_NS_BOOST_END

#endif /* _BOOST_MEMORY_GC_ALLOC_HPP_ */
