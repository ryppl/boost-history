// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_ADVANCE_HPP
#define BOOST_DATAFLOW_PHOENIX_ADVANCE_HPP

#include <boost/spirit/phoenix/function/function.hpp>

#include <iterator>

namespace boost { namespace phoenix {
    
    namespace impl
{
    struct advance
{
    template<class InputIterator, class Distance>
    struct result
{
    typedef InputIterator& type;
};

template<class InputIterator, class Distance>
InputIterator operator()(InputIterator& i, Distance n) const
{        
    return std::advance(i, n);
}
};

template<int N>
struct copy_advance_c
{
    template<class InputIterator>
    struct result
    {
        typedef InputIterator type;
    };
    
    template<class InputIterator>
        InputIterator operator()(InputIterator i) const
    {        
        std::advance(i, N);
        return i;
    }
};

}

function<impl::advance> advance = impl::advance();

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_ADVANCE_HPP