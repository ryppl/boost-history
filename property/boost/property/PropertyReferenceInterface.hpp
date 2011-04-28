#if !defined(PROPERTY_REFERENCE_INTERFACE_HPP)
#define PROPERTY_REFERENCE_INTERFACE_HPP

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_reference.hpp>

namespace properties
{
  
  template <class T>
  struct IPropertyReferenceRead
  {

    BOOST_MPL_ASSERT_NOT((boost::is_reference<T>));
    
    virtual T const & get() const = 0;
    virtual T & get() = 0;

  };

}

#endif
