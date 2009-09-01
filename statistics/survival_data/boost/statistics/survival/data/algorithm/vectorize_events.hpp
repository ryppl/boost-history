///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::algorithm::make_mean_events.hpp               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_ALGORITHM_VECTORIZE_EVENTS_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_ALGORITHM_VECTORIZE_EVENTS_HPP_ER_2009
#include <boost/iterator/iterator_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{

// Copies the failure and event time of each event in a sequence to an
// output iterator
template<typename ItE,typename ItO>
ItO vectorize_events(
    ItE b,
    ItE e,
    ItO o
);
    
    // Implementation //
    
template<typename ItE,typename ItO>
ItO vectorize_events(
    ItE b,
    ItE e,
    ItO o
){
    typedef typename iterator_value<ItE>::type event_;
    typedef typename event_::value_type val_;
    typedef typename event_::failure_type fail_;
    BOOST_MPL_ASSERT((
        is_same<val_,fail_>
    ));
    
    while(b!=e){
        *o = (b->failure()); ++o;
        *o = (b->time()); ++o;
        ++b;
    }
    return o;
}

}// data
}// survival
}// statistics
}// boost

#endif