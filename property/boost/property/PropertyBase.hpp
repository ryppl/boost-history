#pragma once

namespace properties
{
  
  template <typename T>
  class PropertyRead
  {
    public:

    virtual T getValue() = 0;

  };

  template <typename T>
  class PropertyWrite
  {
    public:

    virtual void setValue(T) = 0;

  };
  
}
