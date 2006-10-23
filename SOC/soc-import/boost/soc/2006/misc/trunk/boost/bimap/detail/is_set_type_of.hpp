// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
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

template< class Type, class Enable = void >
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


template< class Type, class Enable = void >
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

