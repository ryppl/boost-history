//
//  generic_ptr/asserting.hpp
//
//  Copyright (c) 2009 Frank Mori Hess
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

#ifndef BOOST_GENERIC_PTR_ASSERTING_HPP_INCLUDED
#define BOOST_GENERIC_PTR_ASSERTING_HPP_INCLUDED

#include <boost/assert.hpp>
#include <boost/generic_ptr/pointer_cast.hpp>
#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/utility/swap.hpp>

namespace boost
{
  namespace generic_ptr
  {
    template<typename T>
    class asserting
    {
      typedef asserting this_type; // for detail/operator_bool.hpp
    public:
      typedef typename pointer_traits<T>::value_type value_type;
      typedef T pointer;
      typedef typename pointer_traits<T>::reference reference;

      template<typename ValueType>
      struct rebind
      {
        typedef asserting<typename generic_ptr::rebind<pointer, ValueType>::other> other;
      };

      asserting(): px()
      {}
      template<typename U>
      asserting( U p ): px( p )
      {}
      template<typename U>
      asserting(const asserting<U> & other): px(other.px)
      {}
#ifndef BOOST_NO_RVALUE_REFERENCES
      asserting(asserting && other): px(std::move(other.px))
      {}
      template<typename U>
      asserting(asserting<U> && other): px(std::move(other.px))
      {}
#endif

      // default copy constructor and assignment operator are fine

      void swap(asserting & other)
      {
        boost::swap(px, other.px);
      }

      template<typename U>
      asserting & operator=(const asserting<U> & other)
      {
        asserting(other).swap(*this);
        return *this;
      }
#ifndef BOOST_NO_RVALUE_REFERENCES
      asserting & operator=(asserting && other)
      {
        asserting(std::move(other)).swap(*this);
        return *this;
      }
      template<typename U>
      asserting & operator=(asserting<U> && other)
      {
        asserting(std::move(other)).swap(*this);
        return *this;
      }
#endif
      void reset()
      {
        asserting().swap(*this);
      }
      template<typename U> void reset(U p)
      {
        asserting(p).swap(*this);
      }

      pointer get() const {return px;}

// implicit conversion to "bool"
#include <boost/generic_ptr/detail/operator_bool.hpp>

      pointer operator->() const
      {
        BOOST_ASSERT(get_plain_old_pointer(px));
        return px;
      }

      reference operator*() const
      {
        BOOST_ASSERT(get_plain_old_pointer(px));
        return *px;
      }

      // conversion to wrapped pointer type
      operator pointer() const
      {
        return px;
      }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    private:
      template<typename Y> friend class asserting;
#endif

      pointer px;
    };

    template<typename T>
    T get_pointer(const asserting<T> &p)
    {
      return p.get();
    }

    // casts
    template<typename ToValueType, typename U>
    typename rebind<asserting<U>, ToValueType>::other static_pointer_cast
    (
      asserting<U> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return static_pointer_cast(p.get(), to_type_iden);
    }
    template<typename ToValueType, typename U>
    typename rebind<asserting<U>, ToValueType>::other const_pointer_cast
    (
      asserting<U> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return const_pointer_cast(p.get(), to_type_iden);
    }
    template<typename ToValueType, typename U>
    typename rebind<asserting<U>, ToValueType>::other dynamic_pointer_cast
    (
      asserting<U> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return dynamic_pointer_cast(p.get(), to_type_iden);
    }

    // comparisons
    template<class T, class U> inline bool operator==(asserting<T> const & a, asserting<U> const & b)
    {
      return a.get() == b.get();
    }
    template<class T, class U> inline bool operator!=(asserting<T> const & a, asserting<U> const & b)
    {
      return a.get() != b.get();
    }
    template<class T, class U> inline bool operator==(asserting<T> const & a, U const & b)
    {
      return a.get() == b;
    }
    template<class T, class U> inline bool operator!=(asserting<T> const & a, U const & b)
    {
      return a.get() != b;
    }
    template<class T, class U> inline bool operator==(T const & a, asserting<U> const & b)
    {
      return a == b.get();
    }
    template<class T, class U> inline bool operator!=(T const & a, asserting<U> const & b)
    {
      return a != b.get();
    }
    #if __GNUC__ == 2 && __GNUC_MINOR__ <= 96
    // Resolve the ambiguity between our op!= and the one in rel_ops
    template<class T> inline bool operator!=(asserting<T> const & a, asserting<T> const & b)
    {
      return a.get() != b.get();
    }
    #endif
    template<class T> inline bool operator<(asserting<T> const & a, asserting<T> const & b)
    {
      return std::less<typename asserting<T>::pointer>()(a.get(), b.get());
    }
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_ASSERTING_HPP_INCLUDED
