#pragma once

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_const.hpp>

namespace properties
{

  template <class T,class I>
  struct IPropertyIndexRead
  {

    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    
    virtual T get(I) const = 0;

  };

  template <class T,class I>
  struct IPropertyIndexWrite
  {
  
    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    BOOST_MPL_ASSERT_NOT((boost::is_const<T>));
    
    virtual void set(I,T) = 0;

  };
  
  template <class T,class I,class R>
  struct IPropertyIndexReadWriteLValue : IPropertyIndexRead<T,I>, public IPropertyIndexWrite<T,I>
  {
  
    virtual R operator [] (I) = 0;

  };

  template <class T,class I,class R>
  struct IPropertyIndexReadImmutable : IPropertyIndexRead<T,I>
  {

    virtual R operator [] (I) const = 0;

  };

}
