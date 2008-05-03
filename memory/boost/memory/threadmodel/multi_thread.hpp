//
//  boost/memory/threadmodel/multi_thread.hpp (*)
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_THREADMODEL_MULTI_THREAD_H
#define BOOST_MEMORY_THREADMODEL_MULTI_THREAD_H

#ifndef BOOST_MEMORY_BASIC_HPP
#include "../basic.hpp"
#endif

#ifndef BOOST_MEMORY_WINAPI_WINBASE_H
#include "../winapi/winbase.h"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class refcount_mt

class refcount_mt
{
public:
	typedef LONG value_type;

private:
	value_type m_nRef;

public:
	refcount_mt(value_type nRef)
		: m_nRef(nRef)
	{
	}

/* @@todo:
	value_type BOOST_MEMORY_CALL acquire()
	{
		return InterlockedIncrement(&m_nRef);
	}

	value_type BOOST_MEMORY_CALL release()
	{
		return InterlockedDecrement(&m_nRef);
	}
*/
	operator value_type()
	{
		return m_nRef;
	}
};

// -------------------------------------------------------------------------
// class critical_section_mt

class critical_section_mt
{
private:
	CRITICAL_SECTION m_cs;
	
private:
	critical_section_mt(const critical_section_mt&);
	void operator=(const critical_section_mt&);

public:
	critical_section_mt()
	{
		InitializeCriticalSection(&m_cs);
	}
	~critical_section_mt()
	{
		DeleteCriticalSection(&m_cs);
	}

	void BOOST_MEMORY_CALL acquire()
	{
		EnterCriticalSection(&m_cs);
	}

	void BOOST_MEMORY_CALL release()
	{
		LeaveCriticalSection(&m_cs);
	}

	bool BOOST_MEMORY_CALL good() const
	{
		return isInitialized(m_cs);
	}
};

// -------------------------------------------------------------------------
//	$Log: $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_THREADMODEL_MULTI_THREAD_H */
