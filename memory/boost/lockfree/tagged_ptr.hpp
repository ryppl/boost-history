//
//  boost/lockfree/tagged_ptr.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/lockfree/todo.htm for documentation.
//
#ifndef BOOST_LOCKFREE_TAGGED_PTR_HPP
#define BOOST_LOCKFREE_TAGGED_PTR_HPP

#ifndef BOOST_DETAIL_WINAPI_WINBASE_H
#include <boost/detail/winapi/winbase.h>
#endif

#ifndef BOOST_DETAIL_DEBUG_HPP
#include <boost/detail/debug.hpp>
#endif

// -------------------------------------------------------------------------

#ifndef NS_BOOST_LOCKFREE_BEGIN
#define NS_BOOST_LOCKFREE_BEGIN		namespace boost { namespace lockfree {
#define NS_BOOST_LOCKFREE_END		} }
#define NS_BOOST_LOCKFREE			boost::lockfree
#endif

#ifndef BOOST_LOCKFREE_CALL
#define BOOST_LOCKFREE_CALL
#endif

#if defined(_WIN32) || defined(__BIT32__)
#define NS_BOOST_LOCKFREE_BIT32
#elif defined(_WIN64) || defined(__BIT64__) || defined(__x86_64__)
#define NS_BOOST_LOCKFREE_BIT64
#else
#error "Unknown Configurations"
#endif

NS_BOOST_LOCKFREE_BEGIN

// -------------------------------------------------------------------------
// class tagged_ptr

#if defined(NS_BOOST_LOCKFREE_BIT32)

template <class Type>
class tagged_ptr
{
private:
	LONG64 m_p;

public:
	tagged_ptr() : m_p(0) {}
	
	template <class FuncT>
	bool BOOST_LOCKFREE_CALL set(FuncT& op)
	{
		BOOST_DETAIL_ASSERT(sizeof(Type*) == sizeof(LONG32));
		for (;;)
		{
			tagged_ptr vOld = *this;
			Type* pOld = vOld.get();
			if (!op.valid(pOld))
				return false;

			tagged_ptr vNew = vOld;
			vNew.m_p += (LONG64)1 << 32;
			*(LONG32*)&vNew.m_p = (LONG32)static_cast<Type*>(op(pOld));
			if (CompareAndSwap64(&m_p, vNew.m_p, vOld.m_p))
				return true;
		}
	}

	__forceinline Type* BOOST_LOCKFREE_CALL clear()
	{
		return (Type*)InterlockedExchangePointer((PVOID*)&m_p, NULL);
	}

	__forceinline Type* BOOST_LOCKFREE_CALL get() const
	{
		return (Type*)(LONG32)m_p;
	}
};

#elif defined(NS_BOOST_LOCKFREE_BIT64)

template <class Type>
class tagged_ptr
{
private:
	ATOMIC_LONG128 m_p;

public:
	tagged_ptr() : m_p(0) {}

	template <class FuncT>
	bool BOOST_LOCKFREE_CALL set(FuncT op)
	{
		BOOST_DETAIL_ASSERT(sizeof(Type*) == sizeof(LONG64));
		for (;;)
		{
			tagged_ptr vOld = *this;
			Type* pOld = vOld.get();
			if (!op.valid(pOld))
				return false;

			tagged_ptr vNew = vOld;
			vNew.m_p += (ATOMIC_LONG128)1 << 64;
			*(LONG64*)&vNew.m_p = (LONG64)static_cast<Type*>(op(pOld));
			if (CompareAndSwap128(&m_p, vNew.m_p, vOld.m_p))
				return true;
		}
	}

	__forceinline Type* BOOST_LOCKFREE_CALL clear()
	{
		return (Type*)InterlockedExchangePointer((PVOID*)&m_p, NULL);
	}

	__forceinline Type* BOOST_LOCKFREE_CALL get() const
	{
		return (Type*)(LONG64)m_p;
	}
};

#else
#error "Unknown Configurations"
#endif

// -------------------------------------------------------------------------
// $Log: $

NS_BOOST_LOCKFREE_END

#endif /* BOOST_LOCKFREE_TAGGED_PTR_HPP */
