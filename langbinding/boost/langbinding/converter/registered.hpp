// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef REGISTERED_040925_HPP
#define REGISTERED_040925_HPP

#include <boost/langbinding/converter/registry.hpp>

namespace boost { namespace langbinding { namespace converter {

template<class T>
struct registered
{
    static registry::registration& instance;
};

template<class T>
registry::registration& registered<T>::instance 
    = registry::acquire(util::type_id<T>());

}}} // namespace boost::langbinding::converter

#endif // REGISTERED_040925_HPP

