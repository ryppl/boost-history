// Copyright 2007 Johan Torp.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_OBSERVABLE_VALUE_HPP
#define BOOST_OBSERVABLE_VALUE_HPP

#include <boost/signal.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/value_init.hpp>

namespace boost {

/// A simple class which combines a value with a way of listening to changes in it
template <typename T>
class observable_value
{
private:
  typedef boost::signal<void (const T&)> signal_type;
public:
  typedef typename signal_type::slot_type slot_type;
  typedef T value_type;

    /// Default constructor
    /** Value intializes the observable value.
    */
  observable_value() : mValue(boost::value_initialized<T>()) {}
  explicit observable_value(const T& t) : mValue(t) {}
  
    /// Assignment operator
    /** Assigns the observable value and notifies observers if the value changed.
    */
  observable_value<T>& operator=(const T& value)
  {
    set(value);
    return *this;
  }
    /// Implicit type conversion operator
    /** Enables implicit type conversion to T.
    */
  operator const T& () const { return mValue; }
  
    /// connect
    /** Connects a slot which will receive callbacks when the value is changed.
    */
  boost::signals::connection connect(const slot_type& slot) const
  {
    return mSignal.connect(slot);
  }
    /// connect
    /** Connects a slot with a given call priority.
    */
  boost::signals::connection connect(int order, const slot_type& slot) const
  {
    return mSignal.connect(order, slot);
  }
    /// disconnect
    /** Disconnects a slot.
    */
  void disconnect(const slot_type& slot) const
  {
    mSignal.disconnect(slot);
  }

      /// operator+=
    /** Add right hand side to value and notify listeners if the value is changed
    */
  observable_value<T>& operator+=(const T& t)
  {
    T target_value(mValue);
    target_value+=t;
    set(target_value);
    return *this;
  }

  observable_value<T>& operator-=(const T& t);
  observable_value<T>& operator*=(const T& x);    
  observable_value<T>& operator/=(const T& x);    
  observable_value<T>& operator%=(const T& x);    
  observable_value<T>& operator|=(const T& x);    
  observable_value<T>& operator&=(const T& x);    
  observable_value<T>& operator^=(const T& x);    

private:
  T mValue;
  mutable boost::signal<void (const T&)> mSignal;

  void set(const T& value)
  {
    if (!(value == mValue))
    {
      mValue=value;
      mSignal(mValue);
    }
  }

  BOOST_STATIC_ASSERT((boost::is_same<int, typename signal_type::group_type>::value));
}; 


} // namespace boost


#endif // BOOST_OBSERVABLE_VALUE_HPP
