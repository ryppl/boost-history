///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::algorithm::events.hpp                         //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_ALGORITHM_EVENTS_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_ALGORITHM_EVENTS_HPP_ER_2009
#include <algorithm>
#include <ext/algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/survival/data/data/record.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{

    // Converts sorted records to events excluding those that are past 
    // entry_bound
    template<typename ItR,typename T,typename ItE>
    ItE events(
        ItR b_r,
        ItR e_r,
        const T& entry_bound,
        ItE out
    ){
        BOOST_ASSERT(
            is_sorted(
                b_r,
                e_r
            )
        );
        typedef typename iterator_value<ItR>::type      record_;
        typedef typename record_::value_type            val_;
        typedef event<val_>                             event_;

        ItR i =  std::lower_bound(
            b_r,
            e_r,
            record_(static_cast<val_>(entry_bound))
        );

        typedef event_ (*ptr_)(const record_&,val_);
        ptr_ f = make_event; 

        return std::transform(
            b_r,
            i,
            out,
            lambda::bind(
                f, 
                lambda::_1,
                entry_bound
            )
        );
    }

}// data
}// survival
}// statistics
}// boost

#endif
