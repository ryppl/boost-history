// Boost.Bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file relation/structured_pair.hpp
/// \brief Defines the structured_pair class.

#ifndef BOOST_BIMAP_RELATION_STRUCTURED_PAIR_HPP
#define BOOST_BIMAP_RELATION_STRUCTURED_PAIR_HPP

#include <boost/call_traits.hpp>
#include <boost/operators.hpp>
#include <boost/bimap/detail/safe_enabler.hpp>
#include <boost/bimap/detail/debug/static_error.hpp>
#include <boost/bimap/relation/pair_layout.hpp>
#include <boost/bimap/relation/symmetrical_base.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost {
namespace bimap {
namespace relation {

/// \brief Storage definition of the left view of a mutant relation.
/**

See also storage_finder, mirror_storage.
                                                                            **/

template< class FirstType, class SecondType >
struct normal_storage :
    public symmetrical_base<FirstType,SecondType>
{
    typedef normal_storage storage_;

    typedef typename storage_::left_value_type  first_type;
    typedef typename storage_::right_value_type second_type;

    first_type   first;
    second_type  second;

    normal_storage() {}

    normal_storage(typename boost::call_traits<first_type >::param_type f,
                   typename boost::call_traits<second_type>::param_type s)

        : first(f), second(s) {}

           typename storage_::left_value_type &  get_left()       { return first;  }
    const  typename storage_::left_value_type &  get_left() const { return first;  }
          typename storage_::right_value_type & get_right()       { return second; }
    const typename storage_::right_value_type & get_right() const { return second; }
};

/// \brief Storage definition of the right view of a mutant relation.
/**

See also storage_finder, normal_storage.
                                                                            **/

template< class FirstType, class SecondType >
struct mirror_storage :
    public symmetrical_base<SecondType,FirstType>
{
    typedef mirror_storage storage_;

    typedef typename storage_::left_value_type   second_type;
    typedef typename storage_::right_value_type  first_type;

    second_type  second;
    first_type   first;

    mirror_storage() {}

    mirror_storage(typename boost::call_traits<first_type  >::param_type f,
                   typename boost::call_traits<second_type >::param_type s)

        : second(s), first(f)  {}

           typename storage_::left_value_type &  get_left()       { return second; }
    const  typename storage_::left_value_type &  get_left() const { return second; }
          typename storage_::right_value_type & get_right()       { return first;  }
    const typename storage_::right_value_type & get_right() const { return first;  }
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
        FirstType,
        SecondType,
        Layout

    >::type,

    totally_ordered<
        structured_pair<FirstType,SecondType,
                        normal_layout>,

    totally_ordered<
        structured_pair<FirstType,SecondType,
                        mirror_layout>

                        > >
{
    public:

    typedef structured_pair
    <
        FirstType, SecondType,
        typename inverse_layout<Layout>::type

    > mirror_pair_type;

    structured_pair() {}

    structured_pair(typename boost::call_traits< typename structured_pair::first_type  >::param_type x,
                    typename boost::call_traits< typename structured_pair::second_type >::param_type y)

        : structured_pair::storage_(x,y) {}

    structured_pair(typename boost::call_traits< structured_pair >::param_type p)

        : structured_pair::storage_(p.first,p.second) {}

    structured_pair& operator=(typename boost::call_traits< structured_pair >::param_type p)
    {
        structured_pair::first = p.first;
        structured_pair::second = p.second;
        return *this;
    }

    // Operators required by Boost.Operators

    bool operator==(typename boost::call_traits< structured_pair >::param_type p)
    {
        return ( ( structured_pair::first  == p.first  ) &&
                 ( structured_pair::second == p.second ) );
    }

    bool operator<(typename boost::call_traits< structured_pair >::param_type p)
    {
        return ( ( structured_pair::first  < p.first  ) &&
                 ( structured_pair::second < p.second ) );
    }

    bool operator==(typename boost::call_traits< mirror_pair_type >::param_type p)
    {
        return ( ( structured_pair::first  == p.first  ) &&
                 ( structured_pair::second == p.second ) );
    }

    bool operator<(typename boost::call_traits< mirror_pair_type >::param_type p)
    {
        return ( ( structured_pair::first  < p.first  ) &&
                 ( structured_pair::second < p.second ) );
    }

};

} // namespace relation
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_RELATION_STRUCTURED_PAIR_HPP

