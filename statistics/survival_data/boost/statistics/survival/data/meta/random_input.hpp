///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::meta::random_input.hpp                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_META_RANDOM_INPUT_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_META_RANDOM_INPUT_HPP_ER_2009
#include <boost/statistics/survival/data/meta/failure_distribution.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{
namespace meta{

    // Model + Covariate + Parameter ---> InputType
    //
    // This is a default implementation that can be overriden with a specializ-
    // ation on M.
    template<typename M> 
    struct random_input{
        typedef typename meta_failure_random<M>::type rdist_;
        typedef typename rdist_::input_type type;
    };
    
}// meta
}// data
}// survival
}// statistics
}// boost

#endif