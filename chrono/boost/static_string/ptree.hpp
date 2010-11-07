//  ptree.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_STATIC_STRING_PTREE_HPP
#define BOOST_STATIC_STRING_PTREE_HPP
#include <boost/mpl/int.hpp>

#include <ios>

namespace boost {
namespace static_string {


template <typename Tree=nil, typename Forest=nil>
struct forest;

//template <typename K, int V>
//struct pair {
//    typedef K key;
//    static const int value = V ;
//};

template <typename Key, typename Index=mpl::int_<-1>, typename Forest=nil >
struct basic_ptree {
    typedef Key key_type;
    typedef Index index_type;
    typedef Forest childs;
};

template <typename Tree, typename Forest>
struct forest {
    typedef Tree head;
    typedef Forest tail;
};

} // namespace static_string
} // namespace boost

#endif // BOOST_STATIC_STRING_PTREE_HPP
