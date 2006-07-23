// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file detail/operator_bracket_proxy.hpp
/// \brief Metafunction to obtain the value_type typedef of a type.

#ifndef BOOST_BIMAP_DETAIL_OPERATOR_BRACKET_PROXY_HPP
#define BOOST_BIMAP_DETAIL_OPERATOR_BRACKET_PROXY_HPP

namespace boost {
namespace bimap {
namespace detail {

/// \brief Return type of operator[] for the unique map views.

template< class View >
struct operator_bracket_proxy
{
    operator_bracket_proxy(View & v, const typename View::key_type & k) :
        view(v), key(k)
    {}

    operator typename View::data_type const & ()
    {
        // TODO
        // Add index check?
        return view.find(key)->second;
    }

    typename View::data_type const &
        operator=( typename View::data_type const & new_data )
    {
        typename View::iterator iter = view.find(key);
        if (iter == view.end() )
        {
            view.insert(typename View::value_type(key,new_data));
        }
        else
        {
            view.replace(iter,typename View::value_type(key,new_data));
        }
        return new_data;
    }

    private:
    View & view;
    const typename View::key_type & key;
};


} // namespace detail
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_DETAIL_OPERATOR_BRACKET_PROXY_HPP
