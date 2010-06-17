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

namespace boost { 

namespace details {
struct bft_category
  : boost::fusion::random_access_traversal_tag,
    boost::fusion::associative_tag
{ };


} // end detials

/** Fusion Iterator for bitfield_tuple class.
 *  This is an iterator over an associative array type.
 */
template <typename BitfieldTuple, std::size_t Pos>
struct bitfield_tuple_iterator
    : fusion::iterator_facade<
        bitfield_tuple_iterator<BitfieldTuple, Pos>,
        details::bft_category
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

    /** Fusion Extension: value_of */
    template<typename Iter>
    struct value_of {
        typedef typename Iter::bitfield_tuple_type::template bit_ref<
            typename mpl::at<
                typename Iter::bitfield_tuple_type::members,
                typename Iter::index
            >::type
        >                                       type;
    };

    /** Fusion Extension: deref */
    template <typename Iter>
    struct deref {
        // the type returned by dereferencing the iterator.
        typedef typename value_of<Iter>::type   type;

        // const dereference operation.
        static type call(Iter const& iter) {
            return type( iter._data.data() );
        }

        // non-const dereference operation.
        static type call(Iter& iter) {
            return type( iter._data.data() );
        }
    };

    /** Fusion Extension: next */
    template<typename Iter>
    struct next {
        typedef bitfield_tuple_iterator<
            typename Iter::bitfield_tuple_type,
            Iter::index::value + 1
        >                                       type;
        static type call(Iter const& iter) {
            return type( iter._data );
        }

        // non-const next operation.
        static type call(Iter& iter) {
            return type( iter._data );
        }
    };

    /** Fusion Extension: prior */
    template<typename Iter>
    struct prior {
        typedef bitfield_tuple_iterator<
            typename Iter::bitfield_tuple_type,
            Iter::index::value - 1
        >                                       type;

        // const prior operation
        static type call(Iter const& iter) {
            return type( iter._data );
        }

        // non-const prior operation.
        static type call(Iter& iter) {
            return type( iter._data );
        }
    };

    /** Fusion Extension: distance */
    template <typename Iter1,typename Iter2>
    struct distance {
        // The distance between iterators of type It1 and It2 as 
        // an MPL Integral Constant
        typedef typename mpl::minus<
            typename Iter1::index,
            typename Iter2::index
        >                                       type;

        // const distance
        static type call(Iter1 const&, Iter2 const&) {
            return type();
        }
	};

    /** Fusion Extension: key_of */
    template <typename Iter>
    struct key_of {
        typedef typename mpl::at<
            typename Iter::bitfield_tuple_type::members,
            typename Iter::index
        >::type::name_type                      type;
    };

    /** Fusion Extension: value_of_data */
    template <typename Iter>
    struct value_of_data {
        typedef typename value_of<Iter>::type type;
    };

    /** Fusion Extension: deref_data */
    template <typename Iter>
    struct deref_data {
        // the type returned by dereferencing the iterator.
        typedef typename value_of<Iter>::type   type;

        // const dereference operation.
        static type call(Iter const& iter) {
            return type( iter._data.data() );
        }

        // non-const dereference operation.
        static type call(Iter& iter) {
            return type( iter._data.data() );
        }
    };

};

} // end boost::fusion


#endif
