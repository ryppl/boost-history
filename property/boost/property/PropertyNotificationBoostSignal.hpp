#pragma once

#include <boost/signal.hpp>
#include <boost/optional.hpp>
#include "PropertyInterface.h"

namespace properties
{

  template <class T> class NotificationBoostSignal
    {
    
    public:
    
    boost::signal<void (const IPropertyWrite<T> &,boost::optional<T>,T)> sig;
    
    protected:
      
    void PropertyChanged(const IPropertyWrite<T> & signaller,boost::optional<T> oldValue,T newValue) 
      {
      sig(signaller,oldValue,newValue);
      }
    
    };

}
