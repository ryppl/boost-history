// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/structured_pair.hpp
/// \brief Defines the structured_pair class.

#ifndef BOOST_BIMAP_RELATION_STRUCTURED_PAIR_HPP
#define BOOST_BIMAP_RELATION_STRUCTURED_PAIR_HPP

#include <utility>

#include <boost/type_traits/remove_const.hpp>

#include <boost/call_traits.hpp>
#include <boost/operators.hpp>
#include <boost/bimap/detail/debug/static_error.hpp>
#include <boost/bimap/relation/pair_layout.hpp>
#include <boost/bimap/relation/symmetrical_base.hpp>
#include <boost/bimap/tags/support/value_type_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/bimap/relation/detail/totally_ordered_pair.hpp>

namespace boost {
namespace bimap {
namespace relation {

/// \brief Storage definition of the left view of a mutant relation.
/**

See also storage_finder, mirror_storage.
                                                                            **/

template< class FirstType, class SecondType >
class normal_storage :
    public symmetrical_base<FirstType,SecondType>
{
    typedef symmetrical_base<FirstType,SecondType> base_;

    public:

    typedef normal_storage storage_;

    typedef typename base_::left_value_type  first_type;
    typedef typename base_::right_value_type second_type;

    first_type   first;
    second_type  second;

    normal_storage() {}

    normal_storage(typename ::boost::call_traits<first_type >::param_type f,
                   typename ::boost::call_traits<second_type>::param_type s)

        : first(f), second(s) {}

          typename base_:: left_value_type &  get_left()       { return first;  }
    const typename base_:: left_value_type &  get_left() const { return first;  }
          typename base_::right_value_type & get_right()       { return second; }
    const typename base_::right_value_type & get_right() const { return second; }
};

/// \brief Storage definition of the right view of a mutant relation.
/**

See also storage_finder, normal_storage.
                                                                            **/

template< class FirstType, class SecondType >
class mirror_storage :
    public symmetrical_base<SecondType,FirstType>
{
    typedef symmetrical_base<SecondType,FirstType> base_;

    public:

    typedef mirror_storage storage_;

    typedef typename base_::left_value_type   second_type;
    typedef typename base_::right_value_type  first_type;

    second_type  second;
    first_type   first;

    mirror_storage() {}

    mirror_storage(typename ::boost::call_traits<first_type  >::param_type f,
                   typename ::boost::call_traits<second_type >::param_type s)

        : second(s), first(f)  {}

          typename base_:: left_value_type &  get_left()       { return second; }
    const typename base_:: left_value_type &  get_left() const { return second; }
          typename base_::right_value_type & get_right()       { return first;  }
    const typename base_::right_value_type & get_right() const { return first;  }
};

/** \struct boost::bimap::relation::storage_finder
\brief Obtain the a storage with the correct layout.

\code
template< class FirstType, class SecondType, class Layout >
struct storage_finder
{
    typedef {normal/mirror}_storage<FirstType,SecondType> type;
};
\endcode

See also normal_storage, mirror_storage.
                                                                        **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template
<
    class FirstType,
    class SecondType,
    class Layout
>
struct storage_finder
{
    typedef normal_storage<FirstType,SecondType> type;
};

template
<
    class FirstType,
    class SecondType
>
struct storage_finder<FirstType,SecondType,mirror_layout>
{
    typedef mirror_storage<FirstType,SecondType> type;
};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

/// \brief A std::pair signature compatible class that allows you to control
///        the internal structure of the data.
/**
This class allows you to specify the order in wich the two data types will be
in the layout of the class.
                                                                               **/

template< class FirstType, class SecondType, class Layout = normal_layout >
class structured_pair :

    public storage_finder
    <
        FirstType, SecondType,

        Layout

    >::type

{
    typedef typename storage_finder< FirstType, SecondType, Layout >::type base_;

    public:

    structured_pair() {}

    structured_pair(typename boost::call_traits< typename base_::first_type  >::param_type x,
                    typename boost::call_traits< typename base_::second_type >::param_type y)

        : base_(x,y) {}

    structured_pair(const structured_pair & p)

        : base_(p.first,p.second) {}

    structured_pair& operator=(const structured_pair & p)
    {
        base_::first = p.first;
        base_::second = p.second;
        return *this;
    }

    typedef std::pair
    <
        typename ::boost::remove_const< typename base_:: first_type >::type,
        typename ::boost::remove_const< typename base_::second_type >::type

    > std_pair;

    typedef std::pair
    <
        const typename ::boost::remove_const< typename base_:: first_type >::type,
              typename ::boost::remove_const< typename base_::second_type >::type

    > std_map_pair;

    explicit structured_pair(const std_pair & p) :
        base_(p.first,p.second)
    {}

    explicit structured_pair(const std_map_pair & p) :
        base_(p.first,p.second)
    {}

    structured_pair& operator=(const std_pair & p)
    {
        base_::first  = p.first;
        base_::second = p.second;
        return *this;
    }

    structured_pair& operator=(const std_map_pair & p)
    {
        base_::first  = p.first;
        base_::second = p.second;
        return *this;
    }

    BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(
        base_::first, base_::second,

        structured_pair,
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

#endif // BOOST_BIMAP_RELATION_STRUCTURED_PAIR_HPP

