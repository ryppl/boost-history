#pragma once

#include <stdexcept>

namespace properties
{

  class PropertyReadException : std::runtime_error
  { 
  
    public:
  
    PropertyReadException() : std::runtime_error("") { }
  };

  class PropertyWriteException : std::runtime_error
  { 
  
    public:
  
    PropertyWriteException() : std::runtime_error("") { }
  };

  class PropertyIndexException : std::runtime_error
  { 
  
    public:
  
    PropertyIndexException() : std::runtime_error("") { }
  };
  
}
