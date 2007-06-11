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

/// \file detail/is_set_type_of.hpp
/// \brief Is set type of and is set type of relation metafunctions.

#ifndef BOOST_BIMAP_DETAIL_IS_SET_TYPE_OF_HPP
#define BOOST_BIMAP_DETAIL_IS_SET_TYPE_OF_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/bimap/detail/safe_enabler.hpp>
#include <boost/bimap/detail/mpl/bool_result.hpp>

#include <boost/bimap/detail/concept_tags.hpp>

/** \struct boost::bimap::detail::is_set_type_of

\brief Type trait to check if a class is a set_type_of specification

\code
template< class Type >
struct is_set_type_of
{
    typedef {mpl::true_/mpl::false_} type;
};
\endcode
                                                                        **/

/** \struct boost::bimap::detail::is_set_type_of_relation

\brief Type trait to check if a class is a set_type_of_relation specification

\code
template< class Type >
struct is_set_type_of_relation
{
    typedef {mpl::true_/mpl::false_} type;
};
\endcode

                                                                               **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace boost {
namespace bimap {
namespace detail {

template< class Type, class Enable = void > // why it is not working with detail::safe_enabler?
struct is_set_type_of
{
    BOOST_BIMAP_MPL_BOOL_RESULT(false);
};

template
<
    class Type
>
struct is_set_type_of
<
    Type,
    typename enable_if< is_base_of< set_type_of_tag, Type > >::type
>
{
    BOOST_BIMAP_MPL_BOOL_RESULT(true);
};


template< class Type, class Enable = void > // why it is not working with detail::safe_enabler?
struct is_set_type_of_relation
{
    BOOST_BIMAP_MPL_BOOL_RESULT(false);
};

template
<
    class Type
>
struct is_set_type_of_relation
<
    Type,
    typename enable_if< is_base_of< set_type_of_relation_tag, Type > >::type
>
{
    BOOST_BIMAP_MPL_BOOL_RESULT(true);
};


} // namespace detail
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_DETAIL_IS_SET_TYPE_OF_HPP

