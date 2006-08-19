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

/// \file relation/const_standard_pair_view.hpp
/// \brief Defines the const_standard_pair_view class.

#ifndef BOOST_BIMAP_RELATION_CONST_STANDARD_PAIR_VIEW_HPP
#define BOOST_BIMAP_RELATION_CONST_STANDARD_PAIR_VIEW_HPP

#include <boost/call_traits.hpp>
#include <boost/operators.hpp>
#include <boost/bimap/relation/pair_layout.hpp>
#include <boost/bimap/relation/symmetrical_base.hpp>

#include <boost/bimap/relation/detail/totally_ordered_pair.hpp>

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
    typedef symmetrical_base<FirstType,SecondType> base_;

    public:

    typedef const_normal_reference_binder reference_binder_;
    typedef standard_relation<FirstType,SecondType> relation_;

    typedef typename base_::left_value_type  first_type;
    typedef typename base_::right_value_type second_type;

    first_type  const & first;
    second_type const & second;

    const_normal_reference_binder(relation_ const & r) :
        first(r.left), second(r.right) {}

    const_normal_reference_binder(first_type const & f, second_type const & s) :
        first(f), second(s) {}

    typename base_::left_value_type & get_left()
    {
        return first;
    }

    const typename base_::left_value_type & get_left() const
    {
        return first;
    }

    typename base_::right_value_type & get_right()
    {
        return second;
    }

    const typename base_::right_value_type & get_right() const
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
    typedef symmetrical_base<SecondType,FirstType> base_;

    public:

    typedef const_mirror_reference_binder reference_binder_;
    typedef standard_relation<SecondType,FirstType> relation_;

    typedef typename base_::right_value_type first_type;
    typedef typename base_::left_value_type  second_type;

    second_type const & second;
    first_type  const & first;

    const_mirror_reference_binder(relation_ const & r) :
        second(r.left), first(r.right) {}

    const_mirror_reference_binder(first_type const & f, second_type const & s) :
        second(s), first(f) {}

    typename base_::left_value_type & get_left()
    {
        return second;
    }

    const typename base_::left_value_type & get_left() const
    {
        return second;
    }

    typename base_::right_value_type & get_right()
    {
        return first;
    }

    const typename base_::right_value_type & get_right() const
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

    public const_reference_binder_finder<FirstType,SecondType,Layout>::type

{
    typedef typename const_reference_binder_finder<FirstType,SecondType,Layout>::type base_;

    public:

    typedef const_standard_pair_view
    <
        FirstType,SecondType,
        typename inverse_layout<Layout>::type

    > mirror_pair_type;

    const_standard_pair_view(typename base_::relation_ const & r) :
        base_(r) {}

    const_standard_pair_view(typename base_::first_type  const & f,
                             typename base_::second_type const & s) :
        base_(f,s) {}


    // Interaction with std::pair

    typedef std::pair
    <
        typename base_::first_type,
        typename base_::second_type

    > std_pair;

    typedef std::pair
    <
        const typename base_::first_type,
        typename base_::second_type

    > std_map_pair;

    explicit const_standard_pair_view(std_pair const & p) :
        base_::reference_binder_(p.first,p.second) {}

    explicit const_standard_pair_view(std_map_pair const & p) :
        base_::reference_binder_(p.first,p.second) {}

    // Interaction with structured pair

    private:

    typedef structured_pair<FirstType,SecondType,normal_layout> normal_structured_pair;
    typedef structured_pair<FirstType,SecondType,mirror_layout> mirror_structured_pair;

    public:

    const_standard_pair_view(normal_structured_pair const & p) :
        base_::reference_binder_(p.first,p.second) {}

    const_standard_pair_view(mirror_structured_pair const & p) :
        base_::reference_binder_(p.first,p.second) {}

    operator const normal_structured_pair ()
    {
        return normal_structured_pair(
            base_::first, base_::second
        );
    }

    operator const mirror_structured_pair ()
    {
        return mirror_structured_pair(
            base_::first, base_::second
        );
    }

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        base_::first, base_::second,

        const_standard_pair_view,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        base_::first, base_::second,

        mirror_pair_type,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        base_::first, base_::second,

        normal_structured_pair,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        base_::first, base_::second,

        mirror_structured_pair,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        base_::first, base_::second,

        std_pair,
        first,second
    );

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        base_::first, base_::second,

        std_map_pair,
        first,second
    );
};

} // namespace relation
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_RELATION_CONST_STANDARD_PAIR_VIEW_HPP
