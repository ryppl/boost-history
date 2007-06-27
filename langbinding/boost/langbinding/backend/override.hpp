// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef OVERRIDE_DWA2004923_HPP
# define OVERRIDE_DWA2004923_HPP

namespace boost { namespace langbinding { namespace backend { 

struct override
{
    void* data;
    void(*destroy)(void*);
};

}}} // namespace boost::langbinding::backend

#endif // OVERRIDE_DWA2004923_HPP
