#if !defined(PROPERTY_NOTIFICATION_BOOST_FUNCTION_HPP)
#define PROPERTY_NOTIFICATION_BOOST_FUNCTION_HPP

#include <boost/function.hpp>
#include <boost/optional.hpp>
#include "property_interface.hpp"

namespace properties
{

  template <class T> class notification_boost_function
    {
    
    public:
    
    boost::function<void (const i_property_write<T> &,boost::optional<T>,T)> fun;
    
    protected:
      
    void property_changed(const i_property_write<T> & signaller,boost::optional<T> oldValue,T newValue)
      {
      fun(signaller,oldValue,newValue);
      }
    
    };

}

#endif
