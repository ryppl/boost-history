// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/module/config.hpp>

namespace boost { namespace langbinding { namespace module { namespace aux {

BOOST_LANGBINDING_DECL int get_module_id()
{
    static int n = 0;
    return n++;
}

}}}} // namespace boost::langbinding::module::aux

