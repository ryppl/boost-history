//
//  boost/memory/system_pool.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef __BOOST_MEMORY_SYSTEM_POOL_HPP__
#define __BOOST_MEMORY_SYSTEM_POOL_HPP__

#ifndef BOOST_MEMORY_BASIC_HPP
#include "basic.hpp"
#endif

#ifndef BOOST_MEMORY_POLICY_HPP
#include "policy.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class system_pool_imp

template <class PolicyT>
class system_pool_imp
{
private:
	typedef typename PolicyT::threadmodel_type ThreadModel;
	typedef typename PolicyT::allocator_type AllocT;
	enum { cbBlock = PolicyT::MemBlockSize };

	struct Block {
		Block* next;
	};
	Block* m_freeList;

private:
	typedef typename ThreadModel::cs cs;
	typedef typename ThreadModel::cslock cslock;

	cs m_cs;

private:
	system_pool_imp(const system_pool_imp&);
	void operator=(const system_pool_imp&);

public:
	system_pool_imp()
		: m_freeList(NULL)
	{
	}
	~system_pool_imp()
	{
		clear();
	}

public:
	void* BOOST_MEMORY_CALL allocate(size_t cb)
	{
		BOOST_MEMORY_ASSERT(m_cs.good());
		BOOST_MEMORY_ASSERT(cb >= (size_t)cbBlock);

		if (cb > (size_t)cbBlock)
			return AllocT::allocate(cb);
		{
			cslock aLock(m_cs);
			if (m_freeList)
			{
				BOOST_MEMORY_ASSERT(AllocT::alloc_size(m_freeList) >= cb);
				Block* blk = m_freeList;
				m_freeList = blk->next;
				return blk;
			}
		}
		return AllocT::allocate(cbBlock);
	}

	void BOOST_MEMORY_CALL deallocate(void* p)
	{
		Block* blk = (Block*)p;
		cslock aLock(m_cs);
		blk->next = m_freeList;
		m_freeList = blk;
	}

	static size_t BOOST_MEMORY_CALL alloc_size(void* p)
	{
		return AllocT::alloc_size(p);
	}

	void BOOST_MEMORY_CALL clear()
	{
		cslock aLock(m_cs);
		while (m_freeList)
		{
			Block* blk = m_freeList;
			m_freeList = blk->next;
			AllocT::deallocate(blk);
		}
	}
};

// -------------------------------------------------------------------------
// class system_pool_s

template <class PolicyT>
class system_pool_s
{
private:
	typedef system_pool_imp<PolicyT> SystemPoolImpl;
	static SystemPoolImpl s_impl;

public:
	static void* BOOST_MEMORY_CALL allocate(size_t cb) { return s_impl.allocate(cb); }
	static void BOOST_MEMORY_CALL deallocate(void* p) { s_impl.deallocate(p); }
	static size_t BOOST_MEMORY_CALL alloc_size(void* p) {
		return s_impl.alloc_size(p);
	}
};

template <class PolicyT>
system_pool_imp<PolicyT> system_pool_s<PolicyT>::s_impl;

// -------------------------------------------------------------------------
// class system_pool

NS_BOOST_MEMORY_POLICY_BEGIN

class stdlib : public sys
{
public:
	typedef stdlib_alloc allocator_type;
};

NS_BOOST_MEMORY_POLICY_END

typedef system_pool_s<NS_BOOST_MEMORY_POLICY::stdlib> system_pool;

// -------------------------------------------------------------------------
// $Log: $

NS_BOOST_MEMORY_END

#endif /* __BOOST_MEMORY_SYSTEM_POOL_HPP__ */
