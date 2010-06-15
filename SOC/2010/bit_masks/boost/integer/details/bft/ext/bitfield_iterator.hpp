//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_FUSION_ITERATOR_HPP
#define BOOST_BITFIELD_FUSION_ITERATOR_HPP
#include <cstddef>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/mpl/void.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/include/value_of.hpp>




namespace boost { 

namespace details {
struct category
  : boost::fusion::random_access_traversal_tag,
    boost::fusion::associative_tag
{ };

typedef mpl::void_ IMPLEMENT_ME;
} // end detials

/** Fusion Iterator for bitfield_tuple class.
 *  This is an iterator over an associative array type.
 */
template <typename BitfieldTuple, std::size_t Pos>
struct bitfield_tuple_iterator
    : fusion::iterator_facade<
        bitfield_tuple_iterator<BitfieldTuple, Pos>,
        details::category
    >
{
    /** Constructor over a bitfield tuple. */
    bitfield_tuple_iterator(BitfieldTuple& bft)
        : _data(bft) { }


    BitfieldTuple& _data;

    /** Important information about the current iterators
     *  template parameters.
     */
    //@{
    typedef mpl::size_t<Pos>    index;
    typedef BitfieldTuple       bitfield_tuple_type;
    //@}

    /** member value Of struct.
     *
     */
    template<typename Iter>
    struct value_of {
        typedef typename Iter::bitfield_tuple_type::template bit_ref<
            typename mpl::at<
                typename Iter::bitfield_tuple_type::members,
                typename Iter::index
            >::type
        >                                   type;
    };

    template <typename Iter>
    struct deref {
        // the type returned by dereferencing the iterator.
        typedef typename value_of<Iter>::type type;

        // dereference operation.
        static type call(Iter& iter) {
            return type( iter._data.data() );
        }
    };

    template<typename Iter>
    struct next {
        typedef typename details::IMPLEMENT_ME type;
        static type call(Iter& iter);
    };

    template<typename Iter>
    struct prior {
        typedef typename details::IMPLEMENT_ME type;
        static type call(Iter& iter);
    };

    template <typename Iter1,typename Iter2>
    struct distance {
        // The distance between iterators of type It1 and It2 as 
        // an MPL Integral Constant
        typedef typename details::IMPLEMENT_ME type;
        static type call(Iter1& it1,Iter2& it2);
	};

    template <typename Iter>
    struct key_of {
        typedef typename details::IMPLEMENT_ME type;
    };

    template <typename Iter>
    struct value_of_data {
        typedef typename details::IMPLEMENT_ME type;
    };

    template <typename Iter>
    struct deref_data {
        typedef typename details::IMPLEMENT_ME type;
        static type call(Iter& it);
    };


};

} // end boost::fusion


#endif
