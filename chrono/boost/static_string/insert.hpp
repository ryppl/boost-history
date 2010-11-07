//  insert.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_STATIC_STRING_INSERT_HPP
#define BOOST_STATIC_STRING_INSERT_HPP

#include <boost/static_string/basic_str.hpp>
#include <boost/static_string/ptree.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>

namespace boost {
namespace static_string {

template <typename M, typename X>
struct insert;

namespace detail {

template <typename Seq, typename I, bool empty=false>
struct insert_nil_false
{
    typedef 
    basic_ptree<
        typename mpl::front<Seq>::type, mpl::int_<-1>, 
        typename insert<
            nil,
            mpl::pair<typename mpl::pop_front<Seq>::type,I>
        >::type
    > type;
};

template <typename Seq, typename I>
struct insert_nil_false<Seq, I, true>
{
    typedef basic_ptree<typename mpl::front<Seq>::type, I> type; 
};

template <typename Seq, typename I, bool empty=false>
struct insert_nil
{
    typedef typename insert_nil_false<Seq, I, 
        mpl::empty<typename mpl::pop_front<Seq>::type>::value
    >::type type;
};

template <typename Seq, typename I>
struct insert_nil<Seq, I, true > 
{
    typedef nil type;
};

template <typename PTree, typename Seq, typename I, bool Equals, bool SeqEmpty>
struct insert_basic_ptree_false;

template <typename Key, typename I, typename F, typename Seq, typename I2>
struct insert_basic_ptree_false<basic_ptree<Key, I, F>, Seq, I2, true, true>
{
    // replace index
    typedef basic_ptree<Key, I2, F> type;
};

template <typename Key, typename I, typename F, typename Seq, typename I2>
struct insert_basic_ptree_false<basic_ptree<Key, I, F>, Seq, I2, true, false>
{
    // insert on forest
    typedef basic_ptree<Key, I, 
        typename insert<F, mpl::pair<typename mpl::pop_front<Seq>::type, 
                                     I2> 
            >::type
    > type;
};
template <typename Key, typename I, typename F, typename Seq, typename I2, bool SeqEmpty>
struct insert_basic_ptree_false<basic_ptree<Key, I, F>, Seq, I2, false, SeqEmpty>
{
    // make a forest
    typedef forest<
        basic_ptree<Key, I, F>, 
        typename insert<nil, mpl::pair<Seq, I2> >::type
    > type;

};
template <typename PTree, typename Seq, typename I, bool SeqEmpty>
struct insert_basic_ptree;

template <typename Key, typename I, typename F, typename Seq, typename I2>
struct insert_basic_ptree<basic_ptree<Key, I, F>, Seq, I2, false >
{
    typedef typename insert_basic_ptree_false<basic_ptree<Key, I, F>, Seq, I2, 
        is_same<typename mpl::front<Seq>::type, Key>::value,
        mpl::empty<typename mpl::pop_front<Seq>::type>::value
    >::type type;
};

template <typename BasicPTree, typename Seq, typename I>
struct insert_basic_ptree<BasicPTree, Seq, I, true >
{
    typedef nil type;
};

template <typename Forest, typename Seq, typename I, bool Equal>
struct insert_forest_false;

template <typename Key, typename I, typename F, typename F2, typename Seq, typename I2>
struct insert_forest_false<forest<basic_ptree<Key, I, F>, F2>, Seq, I2, true >
{
    typedef forest<
        basic_ptree<Key, I, 
            typename insert<F, mpl::pair<typename mpl::pop_front<Seq>::type, I2> >::type
        >,
        F2
    > type;
};

template <typename Key, typename I, typename F, typename F2, typename Seq, typename I2>
struct insert_forest_false<forest<basic_ptree<Key, I, F>, F2>, Seq, I2, false >
{
    typedef forest<
        basic_ptree<Key, I, F>, 
        typename insert<F2, mpl::pair<Seq, I2> >::type
    > type;
};


template <typename Forest, typename Seq, typename I, bool SeqEmpty=true>
struct insert_forest
{
    typedef Forest type;
};

template <typename Key, typename I, typename F, typename F2, typename Seq, typename I2>
struct insert_forest<forest<basic_ptree<Key, I, F>, F2>, Seq, I2, false >
{
    typedef typename insert_forest_false<forest<basic_ptree<Key, I, F>, F2>, Seq, I, 
        is_same<typename mpl::front<Seq>::type, Key >::value
    >::type type;  
};

}

template <typename Seq, typename I>
struct insert<nil, mpl::pair<Seq, I > >
{
    typedef typename static_string::detail::insert_nil<Seq, I, 
        mpl::empty<Seq>::value 
    >::type type;
};

// basic_ptree

template <typename Key, typename I, typename F, typename Seq, typename I2>
struct insert<basic_ptree<Key, I, F>, mpl::pair<Seq, I2> >
{
    typedef typename static_string::detail::insert_basic_ptree<
        basic_ptree<Key, I, F>, Seq, I2, 
        mpl::empty<Seq>::value 
    >::type type;
};


// forest

template <typename Seq, typename I>
struct insert<forest<nil,nil>, mpl::pair<Seq, I> >
{
    typedef forest<typename insert<nil, mpl::pair<Seq, I> >::type> type;
};

template <typename Key, typename I, typename F, typename F2, typename Seq, typename I2>
struct insert<forest<basic_ptree<Key, I, F>, F2>, mpl::pair<Seq, I2> >
{
#if 0
    typedef typename static_string::detail::insert_forest<
        forest<basic_ptree<Key, I, F>, F2>, Seq, I2
        mpl::empty<Seq> 
#else    
    typedef typename mpl::if_<
        mpl::empty<Seq>,
        nil,
        typename mpl::if_<
            is_same<typename mpl::front<Seq>::type, Key >,
            forest<
                basic_ptree<Key, I, 
                    typename insert<F, mpl::pair<typename mpl::pop_front<Seq>::type, I2> >::type
                >,
                F2
            >,
            forest<
                basic_ptree<Key, I, F>, 
                typename insert<F2, mpl::pair<Seq, I2> >::type
            >
        >::type
#endif
    >::type type;
};


} // namespace static_string
} // namespace boost

#endif // BOOST_STATIC_STRING_HPP
