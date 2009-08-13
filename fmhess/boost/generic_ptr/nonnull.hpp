//
//  generic_ptr/nonnull.hpp
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

#ifndef BOOST_GENERIC_PTR_NONNULL_HPP_INCLUDED
#define BOOST_GENERIC_PTR_NONNULL_HPP_INCLUDED

#include <boost/assert.hpp>
#include <boost/generic_ptr/pointer_cast.hpp>
#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/swap.hpp>

namespace boost
{
  namespace generic_ptr
  {
    template<typename T>
    class nonnull
    {
      typedef nonnull this_type; // for detail/operator_bool.hpp
    public:
      typedef typename pointer_traits<T>::value_type value_type;
      typedef T pointer;
      typedef typename pointer_traits<T>::reference reference;

      template<typename ValueType>
      struct rebind
      {
        typedef nonnull<typename generic_ptr::rebind<pointer, ValueType>::other> other;
      };

      template<typename U>
      nonnull
      (
        U p
#ifndef BOOST_NO_SFINAE
        , typename enable_if<is_convertible<U, T> >::type * = 0
#endif // BOOST_NO_SFINAE
      ): px( p )
      {
        if(get_plain_old_pointer(px) == 0)
        {
          throw std::invalid_argument("Attempted to create a generic_ptr::nonnull with a null pointer.");
        }
      }
#ifndef BOOST_NO_SFINAE
      template<typename U>
      explicit nonnull
      (
        U p
        , typename disable_if<is_convertible<U, T> >::type * = 0
      ): px( p )
      {
        if(get_plain_old_pointer(px) == 0)
        {
          throw std::invalid_argument("Attempted to create a generic_ptr::nonnull with a null pointer.");
        }
      }
#endif // BOOST_NO_SFINAE
      template<typename U>
      nonnull
      (
        const nonnull<U> & other
#ifndef BOOST_NO_SFINAE
        , typename enable_if<is_convertible<U, T> >::type * = 0
#endif // BOOST_NO_SFINAE
      ): px(other.px)
      {}

      // no move from nonnull constructor because they could violate the nonnull invariant of the moved-from object

      // default copy constructor and assignment operator are fine

      void swap(nonnull & other)
      {
        boost::swap(px, other.px);
      }

      template<typename U>
      nonnull & operator=(const nonnull<U> & other)
      {
        nonnull(other).swap(*this);
        return *this;
      }
#ifndef BOOST_NO_RVALUE_REFERENCES
      nonnull & operator=(nonnull && other)
      {
        other.swap(*this);
        return *this;
      }
      template<typename U>
      nonnull & operator=(nonnull<U> && other)
      {
        other.swap(*this);
        return *this;
      }
#endif
      template<typename U> void reset(U p)
      {
        nonnull(p).swap(*this);
      }

      pointer get() const {return px;}

// implicit conversion to "bool"
#include <boost/generic_ptr/detail/operator_bool.hpp>

      pointer operator->() const
      {
        return px;
      }

      reference operator*() const
      {
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
      template<typename Y> friend class nonnull;
#endif

      pointer px;
    };

    template<typename T>
    T get_pointer(const nonnull<T> &p)
    {
      return p.get();
    }

    // casts
    template<typename ToValueType, typename U>
    typename rebind<nonnull<U>, ToValueType>::other static_pointer_cast
    (
      nonnull<U> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return static_pointer_cast(p.get(), to_type_iden);
    }
    template<typename ToValueType, typename U>
    typename rebind<nonnull<U>, ToValueType>::other const_pointer_cast
    (
      nonnull<U> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        return const_pointer_cast(p.get(), to_type_iden);
    }
    template<typename ToValueType, typename U>
    typename rebind<nonnull<U>, ToValueType>::other dynamic_pointer_cast
    (
      nonnull<U> const & p,
      mpl::identity<ToValueType> to_type_iden = mpl::identity<ToValueType>()
    )
    {
        //FIXME: should check and throw proper exception on cast failure (cannot return zero for nonnull)
        return dynamic_pointer_cast(p.get(), to_type_iden);
    }

    // comparisons
    template<class T, class U> inline bool operator==(nonnull<T> const & a, nonnull<U> const & b)
    {
      return a.get() == b.get();
    }
    template<class T, class U> inline bool operator!=(nonnull<T> const & a, nonnull<U> const & b)
    {
      return a.get() != b.get();
    }
    template<class T, class U> inline bool operator==(nonnull<T> const & a, U const & b)
    {
      return a.get() == b;
    }
    template<class T, class U> inline bool operator!=(nonnull<T> const & a, U const & b)
    {
      return a.get() != b;
    }
    template<class T, class U> inline bool operator==(T const & a, nonnull<U> const & b)
    {
      return a == b.get();
    }
    template<class T, class U> inline bool operator!=(T const & a, nonnull<U> const & b)
    {
      return a != b.get();
    }
    #if __GNUC__ == 2 && __GNUC_MINOR__ <= 96
    // Resolve the ambiguity between our op!= and the one in rel_ops
    template<class T> inline bool operator!=(nonnull<T> const & a, nonnull<T> const & b)
    {
      return a.get() != b.get();
    }
    #endif
    template<class T> inline bool operator<(nonnull<T> const & a, nonnull<T> const & b)
    {
      return std::less<typename nonnull<T>::pointer>()(a.get(), b.get());
    }
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_NONNULL_HPP_INCLUDED
