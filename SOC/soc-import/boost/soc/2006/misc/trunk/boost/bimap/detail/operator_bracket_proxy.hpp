// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/operator_bracket_proxy.hpp
/// \brief Metafunction to obtain the value_type typedef of a type.

#ifndef BOOST_BIMAP_DETAIL_OPERATOR_BRACKET_PROXY_HPP
#define BOOST_BIMAP_DETAIL_OPERATOR_BRACKET_PROXY_HPP

#include <stdexcept>
#include <iostream>
#include <cassert>

namespace boost {
namespace bimap {

/// \brief Exception for operator[] logic errors

class value_not_found : public std::logic_error
{
    public:
    value_not_found() : logic_error("value not found") {}
    value_not_found(const std::string & str) : logic_error(str) {}
};

/// \brief Exception for operator[] logic errors

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

    typename View::data_type const & get_const_data() const
    {
        typename View::const_iterator i = view.find(key);
        if( i == view.end() )
        {
            ::boost::throw_exception( ::boost::bimap::value_not_found() );
        }
        return i->second;
    }

    operator typename View::data_type const & () const
    {
        return get_const_data();
    }

    operator_bracket_proxy & operator=( typename View::data_type const & new_data )
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
        return *this;
    }

    private:

    View & view;
    const typename View::key_type & key;
};


template< class View >
std::ostream& operator<<(std::ostream& os, operator_bracket_proxy<View> const& p)
{
  return os << p.get_const_data();
}


// Boost.CallTraits do not work here

/***************************************************************************/

#define BOOST_BIMAP_OPERATOR_BRACKET_PROXY_COMPARISON_OPERATOR( OP )        \
                                                                            \
template< class View1, class View2 >                                        \
bool operator OP (operator_bracket_proxy<View1> const& p1,                  \
                  operator_bracket_proxy<View2> const& p2)                  \
{                                                                           \
    return p1.get_const_data() OP p2.get_const_data();                      \
}                                                                           \
                                                                            \
template< class View, class T >                                             \
bool operator OP (operator_bracket_proxy<View> const& p1,                   \
                  const T & t)                                              \
{                                                                           \
    return p1.get_const_data() OP t;                                        \
}                                                                           \
                                                                            \
template< class T, class View >                                             \
bool operator OP (T const & t,                                              \
                  operator_bracket_proxy<View> const& p2)                   \
{                                                                           \
    return t OP p2.get_const_data();                                        \
}                                                                           \

/***************************************************************************/


BOOST_BIMAP_OPERATOR_BRACKET_PROXY_COMPARISON_OPERATOR( == )
BOOST_BIMAP_OPERATOR_BRACKET_PROXY_COMPARISON_OPERATOR( != )
BOOST_BIMAP_OPERATOR_BRACKET_PROXY_COMPARISON_OPERATOR( <= )
BOOST_BIMAP_OPERATOR_BRACKET_PROXY_COMPARISON_OPERATOR( >= )
BOOST_BIMAP_OPERATOR_BRACKET_PROXY_COMPARISON_OPERATOR( <  )
BOOST_BIMAP_OPERATOR_BRACKET_PROXY_COMPARISON_OPERATOR( >  )


} // namespace detail
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_DETAIL_OPERATOR_BRACKET_PROXY_HPP
