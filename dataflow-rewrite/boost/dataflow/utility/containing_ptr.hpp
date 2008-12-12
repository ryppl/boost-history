/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__UTILITY__CONTAINING_PTR_HPP
#define BOOST__DATAFLOW__UTILITY__CONTAINING_PTR_HPP

#include <boost/pointee.hpp>

namespace boost { namespace dataflow {

namespace utility
{    

    template<typename T>
    class containing_ptr
    {
    public:
        containing_ptr()
        {}
        template<typename T0>
        containing_ptr(T0 &t0)
            : t(t0)
        {}
        template<typename T0>
        containing_ptr(const T0 &t0)
            : t(t0)
        {}
        template<typename T0, typename T1>
        containing_ptr(const T0 &t0, const T1 &t1)
            : t(t0, t1)
        {}
        T *operator->()
        {
            return &t;
        }
        const T *operator->() const
        {
            return &t;
        }
        T &operator*()
        {
            return t;
        }
        const T &operator*() const
        {
            return t;
        }
    private:
        T t;
    };
} // namespace utility

} // namespace dataflow

  template <class T>
  struct pointee<dataflow::utility::containing_ptr<T> >
  {
      typedef T type;
  };

} // namespace boost

#endif // BOOST__DATAFLOW__UTILITY__CONTAINING_PTR_HPP
