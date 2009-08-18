//
//  generic_ptr/pointer_cast.hpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/generic_ptr for documentation.
//

#ifndef BOOST_GENERIC_PTR_POINTER_CAST_HPP_INCLUDED
#define BOOST_GENERIC_PTR_POINTER_CAST_HPP_INCLUDED

#include <boost/generic_ptr/pointer_traits.hpp>

namespace std
{
  template<typename T> class shared_ptr;
  template<typename T, typename U>
  shared_ptr<T> static_pointer_cast(const shared_ptr<U> &r);
  template<typename T, typename U>
  shared_ptr<T> const_pointer_cast(const shared_ptr<U> &r);
  template<typename T, typename U>
  shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &r);
} // namespace std

namespace boost
{
  template<typename T> class shared_ptr;
  template<typename T, typename U>
  shared_ptr<T> static_pointer_cast(const shared_ptr<U> &r);
  template<typename T, typename U>
  shared_ptr<T> const_pointer_cast(const shared_ptr<U> &r);
  template<typename T, typename U>
  shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &r);

  template<typename T> class intrusive_ptr;
  template<typename T, typename U>
  intrusive_ptr<T> static_pointer_cast(const intrusive_ptr<U> &r);
  template<typename T, typename U>
  intrusive_ptr<T> const_pointer_cast(const intrusive_ptr<U> &r);
  template<typename T, typename U>
  intrusive_ptr<T> dynamic_pointer_cast(const intrusive_ptr<U> &r);

  namespace generic_ptr
  {
    // two-argument cast overloads for raw pointers
    template<typename T, typename U>
    T* static_pointer_cast(U *r, boost::mpl::identity<T>)
    {
      return static_cast<T*>(r);
    }
    template<typename T, typename U>
    T* const_pointer_cast(U *r, boost::mpl::identity<T>)
    {
      return const_cast<T*>(r);
    }
    template<typename T, typename U>
    T* dynamic_pointer_cast(U *r, boost::mpl::identity<T>)
    {
      return dynamic_cast<T*>(r);
    }

    // two-argument cast overloads for some external smart pointer types
    template<typename T, typename U>
    shared_ptr<T> static_pointer_cast(const shared_ptr<U> &r, boost::mpl::identity<T>)
    {
      return boost::static_pointer_cast<T>(r);
    }
    template<typename T, typename U>
    shared_ptr<T> const_pointer_cast(const shared_ptr<U> &r, boost::mpl::identity<T>)
    {
      return boost::const_pointer_cast<T>(r);
    }
    template<typename T, typename U>
    shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &r, boost::mpl::identity<T>)
    {
      return boost::dynamic_pointer_cast<T>(r);
    }
    template<typename T, typename U>
    std::shared_ptr<T> static_pointer_cast(const std::shared_ptr<U> &r, boost::mpl::identity<T>)
    {
      return std::static_pointer_cast<T>(r);
    }
    template<typename T, typename U>
    std::shared_ptr<T> const_pointer_cast(const std::shared_ptr<U> &r, boost::mpl::identity<T>)
    {
      return std::const_pointer_cast<T>(r);
    }
    template<typename T, typename U>
    std::shared_ptr<T> dynamic_pointer_cast(const std::shared_ptr<U> &r, boost::mpl::identity<T>)
    {
      return std::dynamic_pointer_cast<T>(r);
    }
    template<typename T, typename U>
    intrusive_ptr<T> static_pointer_cast(const intrusive_ptr<U> &r, boost::mpl::identity<T>)
    {
      return boost::static_pointer_cast<T>(r);
    }
    template<typename T, typename U>
    intrusive_ptr<T> const_pointer_cast(const intrusive_ptr<U> &r, boost::mpl::identity<T>)
    {
      return boost::const_pointer_cast<T>(r);
    }
    template<typename T, typename U>
    intrusive_ptr<T> dynamic_pointer_cast(const intrusive_ptr<U> &r, boost::mpl::identity<T>)
    {
      return boost::dynamic_pointer_cast<T>(r);
    }

    template<typename ToValueType, typename U>
    typename rebind<U, ToValueType>::other static_pointer_cast(const U &r)
    {
      return static_pointer_cast(r, mpl::identity<ToValueType>());
    }
    template<typename ToValueType, typename U>
    typename rebind<U, ToValueType>::other const_pointer_cast(const U &r)
    {
      return const_pointer_cast(r, mpl::identity<ToValueType>());
    }
    template<typename ToValueType, typename U>
    typename rebind<U, ToValueType>::other dynamic_pointer_cast(const U &r)
    {
      return dynamic_pointer_cast(r, mpl::identity<ToValueType>());
    }
  } // namespace generic_ptr
} // namespace boost

#endif  // #ifndef BOOST_GENERIC_PTR_POINTER_CAST_HPP_INCLUDED
