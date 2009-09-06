///////////////////////////////////////////////////////////////////////////////
// statistics::survival::model::concept::random_failure.hpp                  //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_MODEL_CONCEPT_RANDOM_FAILURE_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_MODEL_CONCEPT_RANDOM_FAILURE_HPP_ER_2009
#include <boost/concept_check.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/statistics/model/wrap/aggregate/model_covariate_parameter.hpp>

namespace boost{

namespace random{ // fwd
    template<typename T,typename M,typename U>
    T failure_time(M m ,U& u);
}

namespace statistics{
namespace survival{
namespace model{

    template<
        typename T,
        typename M,
        typename X,
        typename P,
        typename U,
        typename B = mpl::empty_base
    >
    class RandomFailure : B
    {
        public:
        typedef statistics::model::model_covariate_parameter_<M,X,P> mcp_;
            
        BOOST_CONCEPT_USAGE(RandomFailure){
            
            ft = boost::random::failure_time<T>(mcp,urng);
            
        }
        
        private:
        mcp_ mcp;
        U urng;
        T ft;
        
    };
    
    
}// model
}// survival
}// statistics
}// boost

#endif