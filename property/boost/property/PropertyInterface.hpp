#if !defined(PROPERTY_INTERFACE_HPP)
#define PROPERTY_INTERFACE_HPP

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_const.hpp>

namespace properties
{
  
  template <class T>
  struct i_property_read
  {

    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    
    virtual T get() const = 0;

  };

  template <class T>
  struct i_property_write
  {

    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    BOOST_MPL_ASSERT_NOT((boost::is_const<T>));
    
    virtual void set(T) = 0;

  };
  
  template <class T>
  struct i_property_read_write : 
    virtual i_property_read<T>, 
    virtual i_property_write<T>
  {
  
    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    BOOST_MPL_ASSERT_NOT((boost::is_const<T>));
    
  };
  
}

#endif
