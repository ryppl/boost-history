//////////////////////////////////////////////////////////////////////////////
// cross_validation::estimator::concept::predictor.hpp                      //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_DETAIL_CROSS_VALIDATION_ESTIMATOR_CONCEPT_TRAINER_HPP_ER_2009
#define  BOOST_STATISTICS_DETAIL_CROSS_VALIDATION_ESTIMATOR_CONCEPT_TRAINER_HPP_ER_2009
#include <boost/concept_check.hpp>
#include <boost/mpl/empty_base.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace cross_validation{
namespace estimator{
namespace concept{

    // Type     Object
    // E        estimator type
    // R        range of training points
    template <typename E,typename R,typename B = mpl::empty_base>
    class Trainer : B
    {        
        public:
        BOOST_CONCEPT_USAGE(Trainer)
        {
            e.train( r ); 
        }
    
        private:
        E e;
        R r;
    };

}// concept
}// estimator
}// cross_validation
}// detail
}// statistics
}// boost

#endif