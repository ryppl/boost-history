// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef PLUGIN_DWA2004922_HPP
# define PLUGIN_DWA2004922_HPP

# include <boost/langbinding/backend/id.hpp>
# include <boost/langbinding/backend/call_xxx_data.hpp>
# include <boost/langbinding/backend/class_instance.hpp>
# include <boost/langbinding/backend/override.hpp>
# include <utility>

namespace boost { namespace langbinding { namespace backend { 

// Base class for plugins implementing backend language-specific
// functionality.
class BOOST_LANGBINDING_DECL plugin
{
 public:
    plugin()
      : id_(count++)            // assign a unique ID
    {}
    
    virtual ~plugin();

    // Implements a call to a backend function.  Used to supply the
    // interface for overridable virtual functions on wrapped classes.
    virtual void* call(
        void* function                          // backend-specific function data
      , call_xxx_data& signature_constants      // constant data associated with the call signature.
      , void* result_storage                    // raw memory in which to store the result.
      , void* const* arg_storage                // an array of pointers to the arguments
    ) const = 0;

    // Find an override for the named function on the given class instance.
    virtual override
    find_override(char const* function_name, class_instance const&) const = 0;

    langbinding::backend::id id() const
    { return id_; }

 private:
    langbinding::backend::id id_;
    
    static unsigned int count;
};

}}} // namespace boost::langbinding::backend


#endif // PLUGIN_DWA2004922_HPP
