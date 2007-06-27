// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef CALL_XXX_DATA_DWA2004922_HPP
# define CALL_XXX_DATA_DWA2004922_HPP

# include <boost/langbinding/aux_/config.hpp>
# include <boost/langbinding/util/type_id.hpp>
# include <boost/shared_ptr.hpp>
# include <vector>

namespace boost { namespace langbinding { namespace backend { 

// The constant data associated with any given call signature from C++
// to XXX.
struct BOOST_LANGBINDING_DECL call_xxx_data
{
    call_xxx_data(
        unsigned arity                  // The number of arguments
      , util::type_info const* types    // Return type and types of arguments
    );
    ~call_xxx_data();
    
    unsigned const arity;
    util::type_info const* const types;
    
    // Private data associated by each backend with the call, indexed
    // by backend plugin ID.
    std::vector<boost::shared_ptr<void> > cache;
};

inline call_xxx_data::call_xxx_data(unsigned arity, util::type_info const* types)
  : arity(arity), types(types)
{}


}}} // namespace boost::langbinding::backend

#endif // CALL_XXX_DATA_DWA2004922_HPP
