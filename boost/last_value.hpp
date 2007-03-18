// last_value function object (documented as part of Boost.Signals)

// Copyright Frank Mori Hess 2007.
// Copyright Douglas Gregor 2001-2003. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/signals

#ifndef BOOST_LAST_VALUE_HPP
#define BOOST_LAST_VALUE_HPP

#include <cassert>
#include <boost/optional.hpp>
#include <stdexcept>

namespace boost {
  class expired_slot;
  // no_slots_error is thrown when we are unable to generate a return value
  // due to no slots being connected to the signal.
  class no_slots_error: public std::exception
  {
  public:
	  virtual const char* what() {return "boost::no_slots_error";}
  };
  namespace last_value_detail {
    template<typename T>
    T default_construct(const T *resolver)
    {
      throw no_slots_error();
    }
    template<typename T>
    optional<T> default_construct(const optional<T> *resolver)
    {
      return optional<T>();
    }
  }
  template<typename T>
  struct last_value {
    typedef T result_type;

    template<typename InputIterator>
    T operator()(InputIterator first, InputIterator last) const
    {
      T * resolver = 0;
      if(first == last)
      {
        return last_value_detail::default_construct(resolver);
      }
      optional<T> value;
      while (first != last)
      {
        try
        {
          value = *first;
        }
        catch(const expired_slot &)
        {}
        ++first;
      }
      if(value) return value.get();
      return last_value_detail::default_construct(resolver);
    }
  };

  template<>
  struct last_value<void> {
    struct unusable {};

  public:
    typedef unusable result_type;

    template<typename InputIterator>
    result_type
    operator()(InputIterator first, InputIterator last) const
    {
      while (first != last)
      {
        try
        {
          *first;
        }
        catch(const expired_slot &)
        {}
        ++first;
      }
      return result_type();
    }
  };
}
#endif // BOOST_SIGNALS_LAST_VALUE_HPP
