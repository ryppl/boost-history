// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef INHERITANCE_QUERY_DWA2004922_HPP
# define INHERITANCE_QUERY_DWA2004922_HPP

# include <boost/langbinding/util/type_id.hpp>

namespace boost { namespace langbinding { namespace classes { namespace aux {

BOOST_LANGBINDING_DECL void* find_static_type(void* p, util::type_info src, util::type_info dst);
BOOST_LANGBINDING_DECL void* find_dynamic_type(void* p, util::type_info src, util::type_info dst);

}}}} // namespace boost::langbinding::classes

#endif // INHERITANCE_QUERY_DWA2004922_HPP
