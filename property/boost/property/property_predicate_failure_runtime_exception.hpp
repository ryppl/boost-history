#if !defined(PROPERTY_PREDICATE_FAILURE_RUNTIME_EXCEPTION_HPP)
#define PROPERTY_PREDICATE_FAILURE_RUNTIME_EXCEPTION_HPP

#include <boost/optional.hpp>
#include <boost/function.hpp>
#include "property_interface.hpp"
#include <stdexcept>
#include <sstream>

namespace properties
{

  template <class T> class predicate_failure_runtime_exception
    {
    
    protected:
      
    void PredicateFailure(const i_property_write<T> & ,const boost::function<bool (T)> & ,boost::optional<T> ,T newValue)
      {
      
      std::ostringstream oss;
      
      oss << "Error trying to update a writeable property with the value '" << newValue << "'.";
            
      throw std::runtime_error(oss.str());
      }
    
    };

}

#endif
