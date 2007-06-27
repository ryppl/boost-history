// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "boost/langbinding/backend/plugin.hpp"

namespace boost { namespace langbinding { namespace backend { 

plugin::~plugin() {}

unsigned int plugin::count; // do *not* initialize!

}}} // namespace boost::langbinding::backend
