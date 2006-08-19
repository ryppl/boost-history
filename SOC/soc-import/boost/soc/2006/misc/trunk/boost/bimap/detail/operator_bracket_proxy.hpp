// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// This code may be used under either of the following two licences:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/operator_bracket_proxy.hpp
/// \brief Metafunction to obtain the value_type typedef of a type.

#ifndef BOOST_BIMAP_DETAIL_OPERATOR_BRACKET_PROXY_HPP
#define BOOST_BIMAP_DETAIL_OPERATOR_BRACKET_PROXY_HPP

#include <stdexcept>

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
