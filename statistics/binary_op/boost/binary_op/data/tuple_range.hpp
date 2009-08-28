///////////////////////////////////////////////////////////////////////////////
// binary_op::data::tuple_range.hpp                                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_DATA_TUPLE_RANGE_HPP_ER_2009
#define BOOST_BINARY_OP_DATA_TUPLE_RANGE_HPP_ER_2009
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/range.hpp>
#include <boost/call_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/iterator/zip_iterator.hpp>

namespace boost{
namespace binary_op{

// Creates a range of tuples from two ranges
template<typename Rx,typename Ry>
struct tuple_range{

    typedef typename remove_reference<Rx>::type const_rx_;
    typedef typename remove_reference<Ry>::type const_ry_;
    typedef typename range_iterator<const_rx_>::type it_x_;
    typedef typename range_iterator<const_ry_>::type it_y_;

    typedef boost::tuple<
        it_x_,
        it_y_
    > iterator_tuple_;

    typedef boost::zip_iterator<
        iterator_tuple_
    > zip_iterator_;

    typedef iterator_range<zip_iterator_> type;
    
    BOOST_MPL_ASSERT(( is_reference<Rx> ));
    BOOST_MPL_ASSERT(( is_reference<Ry> ));

    static type make(
        typename call_traits<Rx>::param_type rx,
        typename call_traits<Ry>::param_type ry
    ){
        return type(
            make_zip_iterator(
                make_tuple(
                    boost::begin(rx),
                    boost::begin(ry)
                )
            ),
            make_zip_iterator(
                make_tuple(
                    boost::end(rx),
                    boost::end(ry)
                )
            )
        );
    }
    
};

}//binary_op
}//boost

#endif