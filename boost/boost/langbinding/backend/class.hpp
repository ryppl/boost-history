// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef CLASS_DWA2004922_HPP
# define CLASS_DWA2004922_HPP

# include <boost/noncopyable.hpp>

namespace boost { namespace langbinding { namespace backend { 

class plugin;

// For each plugin there is a unique class derived from this one, and
// for each wrapped C++ class there is be an instance of that derived
// class.
struct class_ : boost::noncopyable
{
    class_(plugin const& back_end)
      : back_end(back_end) {}
    
    plugin const& back_end;
};

}}} // namespace boost::langbinding::backend

#endif // CLASS_DWA2004922_HPP
