#if !defined(PROPERTY_INDEX_INTERFACE_HPP)
#define PROPERTY_INDEX_INTERFACE_HPP

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_const.hpp>

namespace properties
{

  template <class T,class I>
  struct i_property_index_read
  {

    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    
    virtual T get(I) const = 0;

  };

  template <class T,class I>
  struct i_property_index_write
  {
  
    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    BOOST_MPL_ASSERT_NOT((boost::is_const<T>));
    
    virtual void set(I,T) = 0;

  };
  
  template <class T,class I,class R>
  struct i_property_index_read_write_lvalue : i_property_index_read<T,I>, public i_property_index_write<T,I>
  {
  
    virtual R operator [] (I) = 0;

  };

  template <class T,class I,class R>
  struct i_property_index_read_immutable : i_property_index_read<T,I>
  {

    virtual R operator [] (I) const = 0;

  };

}

#endif
