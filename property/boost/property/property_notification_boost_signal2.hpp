#if !defined(PROPERTY_NOTIFICATION_BOOST_SIGNAL2_HPP)
#define PROPERTY_NOTIFICATION_BOOST_SIGNAL2_HPP

#include <boost/signals2/signal.hpp>
#include <boost/optional.hpp>
#include "property_interface.hpp"

namespace properties
{

  template <class T> class notification_boost_signal2
    {
    
    public:
    
    boost::signals2::signal<void (const i_property_write<T> &,boost::optional<T>,T)> sig;
    
    protected:
      
    void property_changed(const i_property_write<T> & signaller,boost::optional<T> oldValue,T newValue) 
      {
      sig(signaller,oldValue,newValue);
      }
    
    };

}

#endif
