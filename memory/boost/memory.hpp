//
//  boost/memory.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_HPP
#define BOOST_MEMORY_HPP

// -------------------------------------------------------------------------

#ifndef BOOST_MEMORY_AUTO_ALLOC_HPP
#include "memory/auto_alloc.hpp"
#endif

#ifndef BOOST_MEMORY_SCOPED_ALLOC_HPP
#include "memory/scoped_alloc.hpp"
#endif

#ifndef BOOST_MEMORY_GC_ALLOC_HPP
#include "memory/gc_alloc.hpp"
#endif

// -------------------------------------------------------------------------
// class stl_allocator

NS_BOOST_BEGIN

template <class Type, class AllocT = scoped_alloc>
class stl_allocator
{
private:
	AllocT* m_alloc;

public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef Type* pointer;
	typedef const Type* const_pointer;
	typedef Type& reference;
	typedef const Type& const_reference;
	typedef Type value_type;

    template <class U>
    struct rebind { typedef stl_allocator<U, AllocT> other; };

public:
	pointer address(reference val) const
		{ return &val; }
	const_pointer address(const_reference val) const
		{ return &val; }

	size_type max_size() const
		{ size_type count = (size_type)(-1) / sizeof (Type);
		  return (0 < count ? count : 1); }

public:
	stl_allocator(AllocT& alloc) : m_alloc(&alloc) {}

    template <class U>
	stl_allocator(const stl_allocator<U, AllocT>& rhs) : m_alloc(rhs._Getalloc()) {}

	pointer allocate(size_type count, const void* = NULL)
		{ return (pointer)m_alloc->allocate(count * sizeof(Type)); }
	void deallocate(void* p, size_type cb)
		{ m_alloc->deallocate(p, cb); }
	void construct(pointer p, const Type& val)
		{ new(p) Type(val); }
	void destroy(pointer p)
		{ p->~Type(); }

public:
	char* _Charalloc(size_type cb)
		{ return (char*)m_alloc->allocate(cb); }

	AllocT* _Getalloc() const { return m_alloc; }
};

#if defined(BOOST_MEMORY_SUPPORT_PARTIAL_TEMPLATE)

template <class AllocT>
class stl_allocator<void, AllocT>
{
public:
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
 
    template <class U>
    struct rebind { typedef stl_allocator<U, scoped_alloc> other; };
};

#else

template<> class stl_allocator<void, scoped_alloc>
{
public:
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
 
    template <class U>
    struct rebind { typedef stl_allocator<U, scoped_alloc> other; };
};

template<> class stl_allocator<void, auto_alloc>
{
public:
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
 
    template <class U>
    struct rebind { typedef stl_allocator<U, scoped_alloc> other; };
};

template<> class stl_allocator<void, gc_alloc>
{
public:
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
 
    template <class U>
    struct rebind { typedef stl_allocator<U, scoped_alloc> other; };
};

#endif

template <class Type, class AllocT>
inline bool operator==(const stl_allocator<Type, AllocT>&,
                       const stl_allocator<Type, AllocT>&) {
    return true;
}

template <class Type, class AllocT>
inline bool operator!=(const stl_allocator<Type, AllocT>&,
                       const stl_allocator<Type, AllocT>&) {
    return false;
}

NS_BOOST_END

// -------------------------------------------------------------------------
// $Log: memory.hpp,v $

#endif /* BOOST_MEMORY_HPP */
