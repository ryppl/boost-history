// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_DETAIL_MAKE_REF_HPP
#define BOOST_DATAFLOW_DETAIL_MAKE_REF_HPP

#include <boost/type_traits/add_reference.hpp>

namespace boost { namespace dataflow {

namespace detail
{
    
    struct make_ref
    {
        template<typename Sig>
        struct result;
        
        template<typename T>
        struct result<make_ref(T&)>
            : boost::add_reference<T>
        {};

        template<typename T>
        struct result<make_ref(T)>
            : boost::add_reference<T>
        {};
        
        template<typename T>
        typename boost::add_reference<T>::type operator()(T& t) const
        {
            return t;
        }
    };

} // namespace detail

} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_DETAIL_MAKE_REF_HPP
