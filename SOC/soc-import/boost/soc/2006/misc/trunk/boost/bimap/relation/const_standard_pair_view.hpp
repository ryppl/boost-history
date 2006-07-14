// Boost.Bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file relation/const_standard_pair_view.hpp
/// \brief Defines the const_standard_pair_view class.

#ifndef BOOST_BIMAP_RELATION_CONST_STANDARD_PAIR_VIEW_HPP
#define BOOST_BIMAP_RELATION_CONST_STANDARD_PAIR_VIEW_HPP

#include <boost/call_traits.hpp>
#include <boost/operators.hpp>
#include <boost/bimap/relation/pair_layout.hpp>
#include <boost/bimap/relation/symmetrical_base.hpp>

namespace boost {
namespace bimap {
namespace relation {

template< class TA, class TB > class standard_relation;

/// \brief The left side view of a standard relation
/**
See also const_reference_binder_finder, const_mirror_reference_binder.
                                                                        **/

template< class FirstType, class SecondType >
struct const_normal_reference_binder :
    public symmetrical_base<FirstType,SecondType>
{
    public:

    typedef const_normal_reference_binder reference_binder_;
    typedef standard_relation<FirstType,SecondType> relation_;

    typedef typename reference_binder_::left_value_type  first_type;
    typedef typename reference_binder_::right_value_type second_type;

    first_type  & first;
    second_type & second;

    const_normal_reference_binder(relation_ & r) :
        first(r.left), second(r.right) {}

    typename reference_binder_::left_value_type & get_left()
    {
        return first;
    }

    const typename reference_binder_::left_value_type & get_left() const
    {
        return first;
    }

    typename reference_binder_::right_value_type & get_right()
    {
        return second;
    }

    const typename reference_binder_::right_value_type & get_right() const
    {
        return second;
    }
};

/// \brief The right side view of a standard relation
/**
See also const_reference_binder_finder, const_normal_reference_binder.
                                                                        **/

template< class FirstType, class SecondType >
struct const_mirror_reference_binder :
    public symmetrical_base<SecondType,FirstType>
{

    public:

    typedef const_mirror_reference_binder reference_binder_;
    typedef standard_relation<SecondType,FirstType> relation_;

    typedef typename reference_binder_::right_value_type first_type;
    typedef typename reference_binder_::left_value_type  second_type;

    second_type & second;
    first_type  & first;

    const_mirror_reference_binder(relation_ & r) :
        second(r.left), first(r.right) {}

    typename reference_binder_::left_value_type & get_left()
    {
        return second;
    }

    const typename reference_binder_::left_value_type & get_left() const
    {
        return second;
    }

    typename reference_binder_::right_value_type & get_right()
    {
        return first;
    }

    const typename reference_binder_::right_value_type & get_right() const
    {
        return first;
    }
};

/** \struct boost::bimap::relation::const_reference_binder_finder
\brief Obtain the a pair reference binder with the correct layout.

\code
template< class FirstType, class SecondType, class Layout >
struct const_reference_binder_finder
{
    typedef const_{normal/mirror}_reference_binder<FirstType,SecondType> type;
};
\endcode

See also const_normal_reference_binder, const_mirror_reference_binder.
                                                                        **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class FirstType, class SecondType, class Layout >
struct const_reference_binder_finder
{
    typedef const_normal_reference_binder<FirstType,SecondType> type;
};

template< class FirstType, class SecondType >
struct const_reference_binder_finder<FirstType,SecondType,mirror_layout>
{
    typedef const_mirror_reference_binder<FirstType,SecondType> type;
};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

/// \brief A const side view of a relation that is full compliant with the standard.
/**

This view is implemented internally with const references instead of non const
ones.

See also standard_relation, standard_pair_view.
                                                                                **/

template< class FirstType, class SecondType, class Layout >
class const_standard_pair_view :
    public reference_binder_finder<FirstType,SecondType,Layout>::type,

    boost::totally_ordered<
        const_standard_pair_view<FirstType,SecondType,normal_layout>,

    boost::totally_ordered<
        const_standard_pair_view<FirstType,SecondType,mirror_layout>

    > >
{
    public:

    typedef const_standard_pair_view
    <
        FirstType,SecondType,
        typename inverse_layout<Layout>::type

    > mirror_pair_type;

    const_standard_pair_view(typename const_standard_pair_view::relation_ & r) :
        const_standard_pair_view::reference_binder_(r) {}

    const_standard_pair_view& operator=(typename boost::call_traits<const_standard_pair_view>::param_type p)
    {
        const_standard_pair_view::first = p.first;
        const_standard_pair_view::second = p.second;
        return *this;
    }

    // Operators required by Boost.Operators

    bool operator==(typename boost::call_traits<const_standard_pair_view>::param_type p)
    {
        return ( ( const_standard_pair_view::first  == p.first  ) &&
                 ( const_standard_pair_view::second == p.second ) );
    }

    bool operator<(typename boost::call_traits<const_standard_pair_view>::param_type p)
    {
        return ( ( const_standard_pair_view::first  < p.first  ) &&
                 ( const_standard_pair_view::second < p.second ) );
    }

    bool operator==(typename boost::call_traits<mirror_pair_type>::param_type p)
    {
        return ( ( const_standard_pair_view::first  == p.first  ) &&
                 ( const_standard_pair_view::second == p.second ) );
    }

    bool operator<(typename boost::call_traits<mirror_pair_type>::param_type p)
    {
        return ( ( const_standard_pair_view::first  < p.first  ) &&
                 ( const_standard_pair_view::second < p.second ) );
    }
};

} // namespace relation
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_RELATION_CONST_STANDARD_PAIR_VIEW_HPP
