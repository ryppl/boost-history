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

class BOOST_LANGBINDING_DECL plugin
{
 public:
    plugin()
      : id_(count++)
    {}
    
    virtual ~plugin();
    
    virtual void* call(
        void* function
      , call_xxx_data& signature_constants
      , void* result_storage
      , void* const* arg_storage) const = 0;

    virtual override
    find_override(char const* function_name, class_instance const&) const = 0;

    langbinding::backend::id id()
    { return id_; }

 private:
    langbinding::backend::id id_;
    
    static unsigned int count;
};

}}} // namespace boost::langbinding::backend


#endif // PLUGIN_DWA2004922_HPP
