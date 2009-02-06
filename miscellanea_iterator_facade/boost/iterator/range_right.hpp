//////////////////////////////////////////////////////////////////////////////
// range_right.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ITERATOR_RANGE_RIGHT_HPP_ER_2009
#define BOOST_ITERATOR_RANGE_RIGHT_HPP_ER_2009
#include <boost/range.hpp>
#include <boost/iterator/range_result.hpp>
#include <boost/iterator/range_of_known_size.hpp>
namespace boost{

// Takes a range as input and returns the right portion
template<std::size_t size>
class range_right : public range_result, public range_of_known_size<size>{
    public:

    range_right(){}

    template<typename R>
    typename sub_range<R>::type
    operator()(R& range)const{
        typedef sub_range<R> result_type;
        result_type result(begin(range),end(range));
        BOOST_ASSERT(boost::size(range)-size>=0);
        result.advance_begin(boost::size(range)-size);
        return result;
    };
};

}
#endif
