//
//  generic_ptr/throwing.hpp
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

#ifndef BOOST_GENERIC_PTR_THROWING_HPP_INCLUDED
#define BOOST_GENERIC_PTR_THROWING_HPP_INCLUDED

#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/utility/swap.hpp>
#include <stdexcept>

namespace boost
{
  namespace generic_ptr
  {
    template<typename T>
    class throwing
    {
      typedef throwing this_type; // for detail/operator_bool.hpp
    public:
      typedef typename pointer_traits<T>::value_type value_type;
      typedef T pointer;
      typedef typename pointer_traits<T>::reference reference;

      template<typename ValueType>
      struct rebind
      {
        typedef throwing<typename generic_ptr::rebind<pointer, ValueType>::other> other;
      };

      throwing(): px()
      {}
      template<typename U>
      throwing( U p ): px( p )
      {}
      template<typename U>
      throwing(const throwing<U> & other): px(other.px)
      {}
#ifndef BOOST_NO_RVALUE_REFERENCES
      throwing(throwing && other): px(std::move(other.px))
      {}
      template<typename U>
      throwing(throwing<U> && other): px(std::move(other.px))
      {}
#endif

      void swap(throwing & other)
      {
        boost::swap(px, other.px);
      }

      throwing & operator=(const throwing & other)
      {
        throwing(other).swap(*this);
        return *this;
      }

      template<typename U>
      throwing & operator=(const throwing<U> & other)
      {
        throwing(other).swap(*this);
        return *this;
      }
#ifndef BOOST_NO_RVALUE_REFERENCES
      throwing & operator=(throwing && other)
      {
        throwing(std::move(other)).swap(*this);
        return *this;
      }
      template<typename U>
      throwing & operator=(throwing<U> && other)
      {
        throwing(std::move(other)).swap(*this);
        return *this;
      }
#endif
      void reset()
      {
        throwing().swap(*this);
      }
      template<typename U> void reset(U p)
      {
        throwing(p).swap(*this);
      }

      pointer get() const {return px;}

// implicit conversion to "bool"
#include <boost/generic_ptr/detail/operator_bool.hpp>

      pointer operator->() const
      {
        if(get_plain_old_pointer(px) == 0)
        {
          throw std::invalid_argument("Attempted to access object through null pointer.");
        }
        return px;
      }

      reference operator*() const
      {
        if(get_plain_old_pointer(px) == 0)
        {
          throw std::invalid_argument("Attempted to dereference null pointer.");
        }
        return *px;
      }

      // conversion to wrapped pointer type
      operator pointer() const
      {
        return px;
      }
    private:
      pointer px;
    };

    template<typename T>
    T get_pointer(const throwing<T> &p)
    {
      return p.get();
    }

    // casts
    template<typename ToValueType, typename U>
    typename rebind<throwing<U>, ToValueType>::other static_pointer_cast
    (
      throwing<U> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return static_pointer_cast(p.get(), to_type_iden);
    }
    template<typename ToValueType, typename U>
    typename rebind<throwing<U>, ToValueType>::other const_pointer_cast
    (
      throwing<U> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return const_pointer_cast(p.get(), to_type_iden);
    }
    template<typename ToValueType, typename U>
    typename rebind<throwing<U>, ToValueType>::other dynamic_pointer_cast
    (
      throwing<U> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return dynamic_pointer_cast(p.get(), to_type_iden);
    }

    // comparisons
    template<class T, class U> inline bool operator==(throwing<T> const & a, throwing<U> const & b)
    {
      return a.get() == b.get();
    }
    template<class T, class U> inline bool operator!=(throwing<T> const & a, throwing<U> const & b)
    {
      return a.get() != b.get();
    }
    template<class T, class U> inline bool operator==(throwing<T> const & a, U const & b)
    {
      return a.get() == b;
    }
    template<class T, class U> inline bool operator!=(throwing<T> const & a, U const & b)
    {
      return a.get() != b;
    }
    template<class T, class U> inline bool operator==(T const & a, throwing<U> const & b)
    {
      return a == b.get();
    }
    template<class T, class U> inline bool operator!=(T const & a, throwing<U> const & b)
    {
      return a != b.get();
    }
    #if __GNUC__ == 2 && __GNUC_MINOR__ <= 96
    // Resolve the ambiguity between our op!= and the one in rel_ops
    template<class T> inline bool operator!=(throwing<T> const & a, throwing<T> const & b)
    {
      return a.get() != b.get();
    }
    #endif
    template<class T> inline bool operator<(throwing<T> const & a, throwing<T> const & b)
    {
      return std::less<typename throwing<T>::pointer>()(a.get(), b.get());
    }
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_THROWING_HPP_INCLUDED
