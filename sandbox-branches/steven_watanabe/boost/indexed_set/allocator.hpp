/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_ALLOCATOR_HPP
#define BOOST_INDEXED_SET_ALLOCATOR_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/detail/workaround.hpp>

namespace boost{

namespace indexed_sets{

namespace detail{

/* allocator adaption layer */

namespace allocator{

#if defined(BOOST_DINKUMWARE_STDLIB)&&defined(BOOST_NO_STD_ALLOCATOR)
/* This version of Dinkumware's stdlib does not define rebind
 * and provides a member function _Charalloc as a sort of workaround.
 */

template<typename Allocator,typename Type>
struct rebind
{
  typedef Allocator type;
};

template<typename Type,typename Allocator>
Type* allocate(Allocator& al,typename Allocator::size_type n,Type* =0)
{
  return static_cast<Type*>(static_cast<void*>(al._Charalloc(n*sizeof(Type))));
}

template<typename Allocator,typename Type>
void deallocate(Allocator& al,Type* p,typename Allocator::size_type n)
{
  al.deallocate(p,n);
}

template<typename Allocator,typename Type>
void construct(Allocator& al,Type* p,const Type& t)
{
  new(static_cast<void*>(p))Type(t);
}

template<typename Allocator,typename Type>
void destroy(Allocator& al,Type* p)
{
  (static_cast<Type*>(p))->~Type();
}

#else

/* From here on we assume a standard conformant allocator,
 * though this could be not the case.
 */

#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
/* Workaround for a problem in MSVC with dependent template typedefs
 * when doing rebinding of allocators.
 * Modelled after <boost/mpl/aux_/msvc_dtw.hpp> (thanks, Aleksey!)
 */

template<typename Allocator>
struct msvc_rebind
{
  template<bool> struct fake_allocator:Allocator{};
  template<> struct fake_allocator<true>
  {
    template<typename Type>
    struct rebind{};
  };

  template<typename Type>
  struct result:
    fake_allocator<mpl::aux::msvc_never_true<Allocator>::value>::
      template rebind<Type>
  {
  };
};

template<typename Allocator,typename Type>
struct rebind
{
  typedef msvc_rebind<Allocator>::template result<Type> type;
};

#else

template<typename Allocator,typename Type>
struct rebind
{
  typedef typename Allocator::BOOST_NESTED_TEMPLATE rebind<Type>::other type;
};

#endif

template<typename Type,typename Allocator>
typename Allocator::pointer allocate(Allocator& al,typename Allocator::size_type n)
{
  return al.allocate(n);
}

template<typename Allocator>
void deallocate(Allocator& al,typename Allocator::pointer p,typename Allocator::size_type n)
{
  al.deallocate(p,n);
}

template<typename Allocator,typename Type>
void construct(Allocator& al,typename Allocator::pointer p,const Type& t)
{
  al.construct(p,t);
}

template<typename Allocator>
void destroy(Allocator& al,typename Allocator::pointer p)
{
  al.destroy(p);
}

#endif

/* allocator-independent versions of construct and destroy */

template<typename Type>
void construct(void* p,const Type& t)
{
  new (p) Type(t);
}

template<typename Type>
void destroy(const Type* p)
{
  p->~Type();
}

} /* namespace indexed_sets::detail::allocator */

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
