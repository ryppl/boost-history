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

struct BOOST_LANGBINDING_DECL call_xxx_data
{
    call_xxx_data(unsigned arity, util::type_info const* types);
    ~call_xxx_data();
    
    unsigned const arity;
    util::type_info const* const types;
    std::vector<boost::shared_ptr<void> > cache;
};

inline call_xxx_data::call_xxx_data(unsigned arity, util::type_info const* types)
  : arity(arity), types(types)
{}


}}} // namespace boost::langbinding::backend

#endif // CALL_XXX_DATA_DWA2004922_HPP
