#if !defined(PROPERTY_EXCEPTION_HPP)
#define PROPERTY_EXCEPTION_HPP

#include <stdexcept>

namespace properties
{

  class property_read_exception : std::runtime_error
  { 
  
    public:
  
    property_read_exception() : std::runtime_error("") { }
  };

  class property_write_exception : std::runtime_error
  { 
  
    public:
  
    property_write_exception() : std::runtime_error("") { }
  };

  class property_index_exception : std::runtime_error
  { 
  
    public:
  
    property_index_exception() : std::runtime_error("") { }
  };
  
}

#endif
