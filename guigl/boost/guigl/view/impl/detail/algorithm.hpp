/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__DETAIL__ALGORITHM_HPP
#define BOOST__GUIGL__VIEW__IMPL__DETAIL__ALGORITHM_HPP


#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/accumulate.hpp>
#include <boost/utility/enable_if.hpp>

//#include <boost/range/algorithm.hpp>
#include <algorithm>
#include <numeric>

namespace boost { namespace guigl { namespace view {

namespace detail {

    template<typename Sequence, typename F>
    typename boost::enable_if<fusion::traits::is_sequence<Sequence> >::type
        for_each(Sequence &sequence, const F&f)
    {
        fusion::for_each(sequence, f);
    }
    
    template<typename Sequence, typename F>
    typename boost::disable_if<fusion::traits::is_sequence<Sequence> >::type
        for_each(Sequence &sequence, const F&f)
    {
        std::for_each(sequence.begin(), sequence.end(), f);
    }
    
    template<typename Sequence, typename Init, typename F>
    typename boost::enable_if<fusion::traits::is_sequence<Sequence>, Init >::type
        accumulate(Sequence &sequence, const Init &init, const F&f)
    {
        return fusion::accumulate(sequence, init, f);
    }
    
    template<typename Sequence, typename Init, typename F>
    typename boost::disable_if<fusion::traits::is_sequence<Sequence>, Init >::type
        accumulate(Sequence &sequence, const Init &init, const F&f)
    {
        return std::accumulate(sequence.begin(), sequence.end(), init, boost::bind(f,_2,_1));
    }

}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__DETAIL__COMPOUND_EVENT_PROCESSING_HPP