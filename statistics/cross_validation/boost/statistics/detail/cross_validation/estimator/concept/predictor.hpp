//////////////////////////////////////////////////////////////////////////////
// cross_validation::estimator::concept::predictor.hpp                      //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_DETAIL_CROSS_VALIDATION_ESTIMATOR_CONCEPT_PREDICTOR_HPP_ER_2009
#define  BOOST_STATISTICS_DETAIL_CROSS_VALIDATION_ESTIMATOR_CONCEPT_PREDICTOR_HPP_ER_2009
#include <boost/concept_check.hpp>
#include <boost/mpl/empty_base.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace cross_validation{
namespace estimator{
namespace concept{

    // Type     Object
    // E        estimator
    // T        test point 
    template <typename E,typename T, typename B = mpl::empty_base>
    class Predictor : B
    {        
        public:
        
        BOOST_CONCEPT_USAGE(Predictor)
        {
            e.predict( t ); 
        }
    
    private:
        E e;
        T t;
    };

}// concept
}// estimator
}// cross_validation
}// detail
}// statistics
}// boost

#endif