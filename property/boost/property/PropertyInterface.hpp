#if !defined(PROPERTY_INTERFACE_HPP)
#define PROPERTY_INTERFACE_HPP

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_const.hpp>

namespace properties
{
  
  template <class T>
  struct IPropertyRead
  {

    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    
    virtual T get() const = 0;

  };

  template <class T>
  struct IPropertyWrite
  {

    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    BOOST_MPL_ASSERT_NOT((boost::is_const<T>));
    
    virtual void set(T) = 0;

  };
  
  template <class T>
  struct IPropertyReadWrite : 
    virtual IPropertyRead<T>, 
    virtual IPropertyWrite<T>
  {
  
    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    BOOST_MPL_ASSERT_NOT((boost::is_const<T>));
    
  };
  
}

#endif
