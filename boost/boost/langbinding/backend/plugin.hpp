// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef PLUGIN_DWA2004922_HPP
# define PLUGIN_DWA2004922_HPP

# include <boost/langbinding/backend/id.hpp>
# include <boost/langbinding/backend/call_xxx_data.hpp>

namespace boost { namespace langbinding { namespace backend { 

class BOOST_LANGBINDING_DECL plugin
{
 public:
    virtual ~plugin();
    
    virtual void* call(
        call_xxx_data& signature_constants
      , void* result_storage
      , void** arg_storage) = 0;

    langbinding::backend::id id();
    
 private:
    langbinding::backend::id id_;
};

}}} // namespace boost::langbinding::backend


#endif // PLUGIN_DWA2004922_HPP
