//
//  boost/memory/threadmodel/single_thread.hpp (*)
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_THREADMODEL_SINGLE_THREAD_H
#define BOOST_MEMORY_THREADMODEL_SINGLE_THREAD_H

#ifndef BOOST_MEMORY_BASIC_HPP
#include "../basic.hpp"
#endif

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// struct refcount_st

class refcount_st
{
public:
	typedef long value_type;

private:
	value_type m_nRef;

public:
	refcount_st(value_type nRef) : m_nRef(nRef)
	{
	}

	value_type BOOST_MEMORY_CALL acquire() { return ++m_nRef; }
	value_type BOOST_MEMORY_CALL release() { return --m_nRef; }

	operator value_type()
	{
		return m_nRef;
	}
};

// -------------------------------------------------------------------------
// class critical_section_st

class critical_section_st
{
public:
	void BOOST_MEMORY_CALL acquire() {}
	void BOOST_MEMORY_CALL release() {}
	bool BOOST_MEMORY_CALL good() const {
		return true;
	}
};

// -------------------------------------------------------------------------
//	$Log: $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_THREADMODEL_SINGLE_THREAD_H */
