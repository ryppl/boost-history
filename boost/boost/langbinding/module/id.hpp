// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_LANGBINDING_MODULE_040921_HPP
#define BOOST_LANGBINDING_MODULE_040921_HPP

#include <boost/langbinding/module/config.hpp>

namespace boost { namespace langbinding { namespace module {

namespace aux {

    BOOST_LANGBINDING_DECL int get_module_id();
    
    template<class> struct id_
    {
        static int id;
    };

    template<class T>
    int id_<T>::id = get_module_id();

} // namespace aux
    
inline int id()
{
    return aux::id_<int>::id;
}

}}} // namespace boost::langbinding::module

#endif // BOOST_LANGBINDING_MODULE_040921_HPP

