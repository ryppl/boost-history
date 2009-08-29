//////////////////////////////////////////////////////////////////////////////
// statistics::estimator_concept::estimator::concept.hpp                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_ESTIMATOR_CONCEPT_ESTIMATOR_CONCEPT_HPP_ER_2009
#define  BOOST_STATISTICS_ESTIMATOR_CONCEPT_ESTIMATOR_CONCEPT_HPP_ER_2009
#include <boost/concept_check.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/statistics/estimator_concept/trainable/concept.hpp>

namespace boost{
namespace statistics{
namespace estimator_concept{

    // E is the estimator type
    // X the type of the covariate
    template <typename E,typename X, typename B = mpl::empty_base>
    class Estimator : B
    {        
        public:
        
        BOOST_CONCEPT_USAGE(Estimator)
        {
            estimate( e, x ); 
        }
    
    private:
        E e;
        X x;
    };

}// estimator_concept
}// statistics
}// boost

#endif