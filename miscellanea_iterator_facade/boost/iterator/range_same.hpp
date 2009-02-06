//////////////////////////////////////////////////////////////////////////////
// range_same.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ITERATOR_RANGE_SAME_HPP_ER_2009
#define BOOST_ITERATOR_RANGE_SAME_HPP_ER_2009
#include <boost/range.hpp>
#include <boost/iterator/range_result.hpp>
#include <boost/mpl/size_t.hpp>
namespace boost{

// Takes a range as input and returns the entire range
class range_same : public range_result{
    public:
    range_same(){}

    template<std::size_t in_size>
    struct result_size{
        typedef mpl::size_t<in_size> type;
    };

    template<typename R>
    typename sub_range<R>::type
    operator()(R& range)const{
        typedef sub_range<R> result_type;
        return result_type (begin(range),end(range));
    };
};

}
#endif
