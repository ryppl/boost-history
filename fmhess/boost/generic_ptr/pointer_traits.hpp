//
//  generic_ptr/pointer_traits.hpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

#ifndef BOOST_GENERIC_PTR_POINTER_TRAITS_HPP_INCLUDED
#define BOOST_GENERIC_PTR_POINTER_TRAITS_HPP_INCLUDED

#include <boost/get_pointer.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_pointer.hpp>

namespace std
{
  template<typename T> class shared_ptr;
  template<typename T> class unique_ptr;
}

namespace boost
{
  template<typename T> class shared_ptr;
  template<typename T> class scoped_ptr;
  template<typename T> class intrusive_ptr;

  namespace generic_ptr
  {
    namespace detail
    {
      BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type) // defines has_value_type metafunction
      BOOST_MPL_HAS_XXX_TRAIT_DEF(pointer) // defines has_pointer metafunction
      BOOST_MPL_HAS_XXX_TRAIT_DEF(reference) // defines has_reference metafunction
      template<typename T>
      class has_member_pointer_traits: public
        mpl::and_
        <
          has_value_type<T>,
          has_pointer<T>,
          has_reference<T>
        >::type
      {};

      template<typename T> struct builtin_pointer_traits
      {
        typedef typename T::value_type value_type;
        typedef typename T::pointer pointer;
        typedef typename T::reference reference;
      };

      struct empty_pointer_traits
      {};
    }

    template <typename T>
    struct pointer_traits : public
      mpl::if_
      <
        detail::has_member_pointer_traits<T>,
        detail::builtin_pointer_traits<T>,
        detail::empty_pointer_traits
      >::type
    {};
    template<typename T> struct pointer_traits<T*>
    {
      typedef T value_type;
      typedef T * pointer;
      typedef T & reference;
    };
    template<> struct pointer_traits<void*>
    {
      typedef void value_type;
      typedef void * pointer;
      typedef void reference;
    };
#if !defined(BOOST_NO_CV_VOID_SPECIALIZATIONS)
   template<> struct pointer_traits<const void*>
    {
      typedef const void value_type;
      typedef const void * pointer;
      typedef void reference;
    };
    template<> struct pointer_traits<volatile void*>
    {
      typedef volatile void value_type;
      typedef volatile void * pointer;
      typedef void reference;
    };
    template<> struct pointer_traits<const volatile void*>
    {
      typedef const volatile void value_type;
      typedef const volatile void * pointer;
      typedef void reference;
    };
#endif

    template<typename T>
    T * get_plain_old_pointer(T * p)
    {
      return p;
    }

    template<typename GenericPtr>
    typename pointer_traits<GenericPtr>::value_type *
        get_plain_old_pointer(const GenericPtr &gp)
    {
      using boost::get_pointer;
      return get_plain_old_pointer(get_pointer(gp));
    }

    template<typename GenericPtr, typename ValueType>
    struct rebind
    {
      typedef typename GenericPtr::template rebind<ValueType>::other other;
    };
    template<typename T, typename ValueType>
    struct rebind<T*, ValueType>
    {
      typedef ValueType * other;
    };

    // pointer traits for some external smart pointers
    namespace detail
    {
      template<typename T> struct value_templated_ptr_pointer_traits
      {
        typedef T value_type;
        typedef T * pointer;
        typedef typename pointer_traits<T*>::reference reference;
      };
    } // namespace detail
    template<typename T> struct pointer_traits<boost::shared_ptr<T> > :
      public detail::value_templated_ptr_pointer_traits<T>
    {};
    template<typename T, typename ValueType>
    struct rebind<boost::shared_ptr<T>, ValueType>
    {
      typedef boost::shared_ptr<ValueType> other;
    };
    template<typename T> struct pointer_traits<boost::scoped_ptr<T> > :
      public detail::value_templated_ptr_pointer_traits<T>
    {};
    template<typename T, typename ValueType>
    struct rebind<boost::scoped_ptr<T>, ValueType>
    {
      typedef boost::scoped_ptr<ValueType> other;
    };
    template<typename T> struct pointer_traits<boost::intrusive_ptr<T> > :
      public detail::value_templated_ptr_pointer_traits<T>
    {};
    template<typename T, typename ValueType>
    struct rebind<boost::intrusive_ptr<T>, ValueType>
    {
      typedef boost::intrusive_ptr<ValueType> other;
    };
    template<typename T> struct pointer_traits<std::shared_ptr<T> > :
      public detail::value_templated_ptr_pointer_traits<T>
    {};
    template<typename T, typename ValueType>
    struct rebind<std::shared_ptr<T>, ValueType>
    {
      typedef std::shared_ptr<ValueType> other;
    };
    template<typename T> struct pointer_traits<std::unique_ptr<T> > :
      public detail::value_templated_ptr_pointer_traits<T>
    {};
    template<typename T, typename ValueType>
    struct rebind<std::unique_ptr<T>, ValueType>
    {
      typedef std::unique_ptr<ValueType> other;
    };
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_POINTER_TRAITS_HPP_INCLUDED
