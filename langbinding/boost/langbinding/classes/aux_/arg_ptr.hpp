// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef ARG_PTR_DWA2004923_HPP
# define ARG_PTR_DWA2004923_HPP

# include <boost/type_traits/remove_cv.hpp>

namespace boost { namespace langbinding { namespace classes { namespace aux { 

template <class T>
T* arg_ptr(T const& x)
{
    return const_cast<T*>(&x);
}
    
template <class T>
typename boost::remove_cv<T>::type*
arg_ptr(T* x)
{
    return const_cast<typename boost::remove_cv<T>::type*>(x);
}
    
}}}} // namespace boost::langbinding::classes::aux

#endif // ARG_PTR_DWA2004923_HPP
