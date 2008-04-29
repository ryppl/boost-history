//
//  boost/memory/basic.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef __BOOST_MEMORY_BASIC_HPP__
#define __BOOST_MEMORY_BASIC_HPP__

// -------------------------------------------------------------------------

#if !defined(_NEW_) && !defined(_NEW)
#include <new>	// new
#endif

#if !defined(_CSTDIO_) && !defined(_CSTDIO)
#include <cstdio>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H)
#include <malloc.h>	// _alloca
#endif

#if defined(_MSC_VER) && !defined(_INC_CRTDBG)
#include <crtdbg.h> // _CrtSetDbgFlag
#endif

// =========================================================================

#ifndef BOOST_MEMORY_CALL
#define BOOST_MEMORY_CALL
#endif

// -------------------------------------------------------------------------

#ifndef __NS_BOOST_BEGIN
#define __NS_BOOST_BEGIN	namespace boost {
#define __NS_BOOST_END		}
#endif

// =========================================================================
// BOOST_MEMORY_ASSERT - diagnost

#if defined(ASSERT)
#define BOOST_MEMORY_ASSERT(e)		ASSERT(e)
#elif defined(_ASSERTE)
#define BOOST_MEMORY_ASSERT(e)		_ASSERTE(e)
#else
#ifdef _DEBUG
#ifndef assert
#include <cassert>
#endif
#define BOOST_MEMORY_ASSERT(e)		assert(e)
#else
#define BOOST_MEMORY_ASSERT(e)
#endif
#endif

// =========================================================================
// Configurations

#ifndef BOOST_MEMORY_ALLOC_PADDING
#define BOOST_MEMORY_ALLOC_PADDING	32
#endif

#ifndef BOOST_MEMORY_BLOCK_TOTAL
#define BOOST_MEMORY_BLOCK_TOTAL	16384	// 16k
#endif

#ifndef BOOST_MEMORY_BLOCK_SIZE
#define BOOST_MEMORY_BLOCK_SIZE		(BOOST_MEMORY_BLOCK_TOTAL - BOOST_MEMORY_ALLOC_PADDING)
#endif

// =========================================================================
// constructor_traits, destructor_traits

__NS_BOOST_BEGIN

typedef void BOOST_MEMORY_CALL __FnDestructor(void* data);
typedef __FnDestructor* destructor_t;

template <class Type>
struct constructor_traits
{
	static Type* BOOST_MEMORY_CALL construct(void* data)
	{
		return new(data) Type;
	}

	static Type* BOOST_MEMORY_CALL constructArray(Type* array, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			new(array + i) Type;
		return array;
	}
};

template <class Type>
struct destructor_traits
{
	typedef destructor_t destructor_type;
	struct array_destructor_header
	{
		size_t count;
	};
	
	static void BOOST_MEMORY_CALL destruct(void* data)
	{
		((Type*)data)->~Type();
	}

	static void BOOST_MEMORY_CALL destructArrayN(Type* array, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			array[i].~Type();
	}

	static void BOOST_MEMORY_CALL destructArray(void* data)
	{
		array_destructor_header* hdr = (array_destructor_header*)data;
		destructArrayN((Type*)(hdr + 1), hdr->count);
	}

	template <class AllocT>
	static void* BOOST_MEMORY_CALL allocArray(AllocT& alloc, size_t count)
	{
		array_destructor_header* hdr =
			(array_destructor_header*)alloc.allocate(
				sizeof(array_destructor_header)+sizeof(Type)*count, destructArray);
		hdr->count = count;
		return hdr + 1;
	}

    static void* BOOST_MEMORY_CALL getArrayBuffer(void* array)
	{
		return (array_destructor_header*)array - 1;
	}

    static size_t BOOST_MEMORY_CALL getArraySize(void* array)
	{
		return ((array_destructor_header*)array - 1)->count;
	}
};

template <class Type>
inline void BOOST_MEMORY_CALL destroyArray(Type* array, size_t count)
{
	destructor_traits<Type>::destructArrayN(array, count);
}

__NS_BOOST_END

// =========================================================================
// BOOST_NO_DESTRUCTOR

#define BOOST_NO_DESTRUCTOR(Type)											\
__NS_BOOST_BEGIN															\
template <>																	\
struct destructor_traits< Type >											\
{																			\
	typedef int destructor_type;											\
																			\
	enum { destruct = 0 };													\
	enum { destructArray = 0 };												\
																			\
	static void BOOST_MEMORY_CALL destructArrayN(Type* array, size_t count) {} \
																			\
	template <class AllocT>													\
	static void* BOOST_MEMORY_CALL allocArray(AllocT& alloc, size_t count) {\
		return alloc.allocate(sizeof(Type)*count);							\
	}																		\
																			\
	static void* BOOST_MEMORY_CALL getArrayBuffer(void* array) {			\
		return array;														\
	}																		\
																			\
	static size_t BOOST_MEMORY_CALL getArraySize(void* array) {				\
		BOOST_MEMORY_ASSERT( !"Don't call me!!!" );							\
		return 0;															\
	}																		\
};																			\
__NS_BOOST_END

#define BOOST_INT_NO_DESTRUCTOR(Type)										\
	BOOST_NO_DESTRUCTOR(unsigned Type);										\
	BOOST_NO_DESTRUCTOR(signed Type)

// -------------------------------------------------------------------------
// BOOST_NO_CONSTRUCTOR

#define BOOST_NO_CONSTRUCTOR(Type)											\
__NS_BOOST_BEGIN															\
template <>																	\
struct constructor_traits< Type >											\
{																			\
	static Type* BOOST_MEMORY_CALL construct(void* data) {					\
		return (Type*)data;													\
	}																		\
	static Type* BOOST_MEMORY_CALL constructArray(Type* array, size_t count) { \
		return array;														\
	}																		\
};																			\
__NS_BOOST_END

#define BOOST_INT_NO_CONSTRUCTOR(Type)										\
	BOOST_NO_CONSTRUCTOR(unsigned Type);									\
	BOOST_NO_CONSTRUCTOR(signed Type)

// -------------------------------------------------------------------------
// C Standard Types Support

#define BOOST_DECL_CTYPE(Type)												\
	BOOST_NO_CONSTRUCTOR(Type);												\
	BOOST_NO_DESTRUCTOR(Type)

#define BOOST_DECL_INT_CTYPE(Type)											\
	BOOST_NO_CONSTRUCTOR(Type);												\
	BOOST_INT_NO_DESTRUCTOR(Type)

// -------------------------------------------------------------------------

BOOST_DECL_CTYPE(bool);
BOOST_DECL_CTYPE(float);
BOOST_DECL_CTYPE(double);

BOOST_DECL_INT_CTYPE(int);
BOOST_DECL_INT_CTYPE(char);
BOOST_DECL_INT_CTYPE(short);
BOOST_DECL_INT_CTYPE(long);

// =========================================================================
// MEMORY_DBG_NEW_ARG

#if defined(_DEBUG)
#define MEMORY_FILE_LINE_ARG		,__FILE__, __LINE__
#else
#define MEMORY_FILE_LINE_ARG
#endif

#define MEMORY_NEW_ARG(Type)					sizeof(Type), boost::destructor_traits<Type>::destruct
#define MEMORY_DBG_NEW_ARG(Type)				MEMORY_NEW_ARG(Type) MEMORY_FILE_LINE_ARG

#define MEMORY_NEW_ARRAY_ARG(Type, count)		(count), (Type*)0
#define MEMORY_DBG_NEW_ARRAY_ARG(Type, count)	MEMORY_NEW_ARRAY_ARG(Type, count) MEMORY_FILE_LINE_ARG

#define MEMORY_DBG_ALLOC_ARG(Type)				sizeof(Type) MEMORY_FILE_LINE_ARG
#define MEMORY_DBG_ALLOC_ARRAY_ARG(Type, count)	sizeof(Type)*(count) MEMORY_FILE_LINE_ARG

// =========================================================================
// NEW, NEW_ARRAY, ALLOC, ALLOC_ARRAY

#define BOOST_NEW(alloc, Type)					::new((alloc).allocate(MEMORY_DBG_NEW_ARG(Type))) Type
#define BOOST_NEW_ARRAY(alloc, Type, count) 	(alloc).newArray(MEMORY_DBG_NEW_ARRAY_ARG(Type, count))

#define BOOST_ALLOC(alloc, Type)				((Type*)(alloc).allocate(MEMORY_DBG_ALLOC_ARG(Type)))
#define BOOST_ALLOC_ARRAY(alloc, Type, count)	((Type*)(alloc).allocate(MEMORY_DBG_ALLOC_ARRAY_ARG(Type, count)))

// =========================================================================

__NS_BOOST_BEGIN

inline void BOOST_MEMORY_CALL enableMemoryLeakCheck()
{
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif
}

__NS_BOOST_END

// =========================================================================
// $Log: basic.hpp,v $

#endif /* __BOOST_MEMORY_BASIC_HPP__ */
