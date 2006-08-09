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

#include <stdexcept>

namespace boost {
namespace bimap {

// Exceptions for operator[] logic errors

class value_not_found : public std::logic_error
{
    public:
    value_not_found() : logic_error("value not found") {}
    value_not_found(const std::string & str) : logic_error(str) {}
};

class duplicate_value : public std::logic_error
{
    public:
    duplicate_value() : logic_error("duplicate value") {}
    duplicate_value(const std::string & str) : logic_error(str) {}
};

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
        typename View::const_iterator i = view.find(key);
        if( i == view.end() )
        {
            ::boost::throw_exception( ::boost::bimap::value_not_found() );
        }
        return i->second;
    }

    typename View::data_type const &
        operator=( typename View::data_type const & new_data )
    {
        typename View::iterator iter = view.find(key);
        if
        (
            !
            (
                ( iter == view.end() ) ?
                view.insert(typename View::value_type(key,new_data)).second :
                view.replace(iter,typename View::value_type(key,new_data))
            )
        )
        {
            ::boost::throw_exception( ::boost::bimap::duplicate_value() );
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
