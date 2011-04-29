#if !defined(PROPERTY_NOTIFICATION_BOOST_SIGNAL_HPP)
#define PROPERTY_NOTIFICATION_BOOST_SIGNAL_HPP

#include <boost/signal.hpp>
#include <boost/optional.hpp>
#include "PropertyInterface.h"

namespace properties
{

  template <class T> class notification_boost_signal
    {
    
    public:
    
    boost::signal<void (const i_property_write<T> &,boost::optional<T>,T)> sig;
    
    protected:
      
    void property_changed(const i_property_write<T> & signaller,boost::optional<T> oldValue,T newValue) 
      {
      sig(signaller,oldValue,newValue);
      }
    
    };

}

#endif
