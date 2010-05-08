// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MPL_ADAPTED_STD_PAIR_HPP
#define BOOST_MPL_ADAPTED_STD_PAIR_HPP

#include <utility>
#include <boost/mpl/int.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/iterator_tags.hpp>

namespace boost {
namespace mpl {

template <typename T1, typename T2, typename I>
struct std_pair_iterator {
    typedef bidirectional_iterator_tag category;
};

template <typename T1, typename T2>
struct deref< std_pair_iterator< T1, T2, int_<0> > > {
    typedef T1 type;
};

template <typename T1, typename T2>
struct deref< std_pair_iterator< T1, T2, int_<1> > > {
    typedef T2 type;
};

template <typename T1, typename T2, typename I>
struct next< std_pair_iterator<T1,T2,I> > {
    typedef std_pair_iterator<T1,T2,typename next<I>::type> type;
};

template <typename T1, typename T2, typename I>
struct prior< std_pair_iterator<T1,T2,I> > {
    typedef std_pair_iterator<T1,T2,typename prior<I>::type> type;
};

template <typename T1, typename T2>
struct begin< std::pair<T1,T2> > {
    typedef std_pair_iterator< T1, T2, int_<0> > type;
};

template <typename T1, typename T2>
struct end< std::pair<T1,T2> > {
    typedef std_pair_iterator< T1, T2, int_<2> > type;
};

template <typename T1, typename T2>
struct front< std::pair<T1,T2> > {
    typedef T1 type;
};

template <typename T1, typename T2>
struct back< std::pair<T1,T2> > {
    typedef T2 type;
};

template <typename T1, typename T2>
struct size< std::pair<T1,T2> > {
    typedef int_<2> type;
};

template <typename T1, typename T2>
struct empty< std::pair<T1,T2> > {
    typedef bool_<false> type;
};

} // namespace boost::mpl
} // namespace boost

#endif // !BOOST_MPL_ADAPTED_STD_PAIR_HPP
