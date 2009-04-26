//
//  boost/memory/typed_alloc.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_TYPED_ALLOC_HPP
#define BOOST_MEMORY_TYPED_ALLOC_HPP

NS_BOOST_MEMORY_BEGIN

// -------------------------------------------------------------------------
// class norm_typed_alloc

template <class Type, class PolicyT>
class norm_typed_alloc : private fixed_alloc<PolicyT>
{
private:
	typedef fixed_alloc<PolicyT> PoolT;

#pragma pack(1)
protected:
	struct ChunkHeader
	{
		union
		{
			MemBlock* pBlock;
			size_t tag;
		};
	};
#pragma pack()

protected:
	size_t BOOST_MEMORY_CALL is_allocated_(void*p )
	{
		return 1 & ((ChunkHeader*)p - 1)->tag;
	}
	
	void BOOST_MEMORY_CALL mark_allocated_(void* p)
	{
		BOOST_MEMORY_ASSERT(!is_allocated_(p));
		++((ChunkHeader*)p - 1)->tag;
	}

	void BOOST_MEMORY_CALL mark_deallocated_(void* p)
	{
		BOOST_MEMORY_ASSERT(is_allocated_(p));
		--((ChunkHeader*)p - 1)->tag;
	}
	
public:
	typedef PoolT pool_type;
	
public:
	using PoolT::get_alloc;

public:
	norm_typed_alloc() : PoolT(sizeof(Type))
	{
		BOOST_MEMORY_STATIC_ASSERT(sizeof(ChunkHeader) == sizeof(typename PoolT::ChunkHeader));
	}
	
	explicit norm_typed_alloc(AllocT alloc) : PoolT(alloc, sizeof(Type))
	{
		BOOST_MEMORY_STATIC_ASSERT(sizeof(ChunkHeader) == sizeof(typename PoolT::ChunkHeader));
	}
	
	~norm_typed_alloc()
	{
		clear();
	}

	pool_type& BOOST_MEMORY_CALL get_pool()
	{
		return *this;
	}
	
public:
	__forceinline void BOOST_MEMORY_CALL clear()
	{
		// ...
	}

#if defined(BOOST_MEMORY_NO_STRICT_EXCEPTION_SEMANTICS)
	__forceinline void* BOOST_MEMORY_CALL allocate(size_t cb, destructor_t fn)
	{
		BOOST_MEMORY_ASSERT(fn == BOOST_MEMORY_DESTRUCTOR(Type));
		void* p = PoolT::allocate(cb);
		manage(p, fn);
		return p;
	}
#endif
	
	__forceinline void* BOOST_MEMORY_CALL unmanaged_alloc(size_t cb, destructor_t fn) {
		BOOST_MEMORY_ASSERT(fn == BOOST_MEMORY_DESTRUCTOR(Type));
		return PoolT::allocate(cb);
	}

	__forceinline void BOOST_MEMORY_CALL manage(void* p, destructor_t fn) {
		BOOST_MEMORY_ASSERT(fn == BOOST_MEMORY_DESTRUCTOR(Type));
		mark_allocated_(p);
	}

	void BOOST_MEMORY_CALL destroy(Type* obj) {
		mark_deallocated_(obj);
		obj->~Type();
		return PoolT::deallocate(obj);
	}
};

// -------------------------------------------------------------------------
// class typed_alloc

#if defined(_MSC_VER) && (_MSC_VER <= 1200) // VC++ 6.0

template <class Type, class PolicyT, int hasDestructor>
struct typed_alloc_base_traits_
{
	template <int hasDestructor>
	struct traits_ {
		typedef norm_typed_alloc<Type, PolicyT> type;
	};
	
	template <>
	struct traits_<0> {
		typedef pod_typed_alloc<Type, PolicyT> type;
	};
	
	typedef typename traits_<hasDestructor>::type type;
};

#else

template <class Type, class PolicyT, int hasDestructor>
struct typed_alloc_base_traits_ {
	typedef norm_typed_alloc<Type, PolicyT> type;
};

template <class Type, class PolicyT>
struct typed_alloc_base_traits_<Type, PolicyT, 0> {
	typedef pod_typed_alloc<Type, PolicyT> type;
};

#endif

//
// class typed_alloc
//

template <class Type, class PolicyT>
class typed_alloc : 
	public typed_alloc_base_traits_<Type, PolicyT, destructor_traits<Type>::hasDestructor>::type
{
private:
	typedef typed_alloc_base_traits_<Type, PolicyT, destructor_traits<Type>::hasDestructor>::type Base;
	typedef typename Base::alloc_type AllocT;
	
public:
	typed_alloc() :
		Base(sizeof(Type)) {
	}
	explicit typed_alloc(AllocT alloc) :
		Base(alloc, sizeof(Type)) {
	}
};

// -------------------------------------------------------------------------
// $Log: $

NS_BOOST_MEMORY_END

#endif /* BOOST_MEMORY_TYPED_ALLOC_HPP */
