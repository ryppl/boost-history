// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef CLASS_INSTANCE_DWA2004922_HPP
# define CLASS_INSTANCE_DWA2004922_HPP

# include <boost/langbinding/backend/class.hpp>

namespace boost { namespace langbinding { namespace backend { 

class plugin;

// This is a terrible name, but I can't think of a better one.
struct class_instance
{
    class_instance()
      : class_type(0)
      , instance(0)
    {}
    
    class_instance(class_* wrapper, void* instance)
      : class_type(wrapper)
      , instance(instance)
    {}
    
    // The xxx class that corresponds directly to a
    // held C++ class (not a derived class)        
    backend::class_* class_type;

    // A reference to the xxx instance.
    void* instance;
};

}}} // namespace boost::langbinding::backend

#endif // CLASS_INSTANCE_DWA2004922_HPP
