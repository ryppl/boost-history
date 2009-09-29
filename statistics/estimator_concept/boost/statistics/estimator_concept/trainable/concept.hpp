//////////////////////////////////////////////////////////////////////////////
// statistics::estimator_concept::trainable::concept.hpp                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_ESTIMATOR_CONCEPT_TRAINABLE_CONCEPT_HPP_ER_2009
#define  BOOST_STATISTICS_ESTIMATOR_CONCEPT_TRAINABLE_CONCEPT_HPP_ER_2009
#include <boost/concept_check.hpp>
#include <boost/mpl/empty_base.hpp>

namespace boost{
namespace statistics{
namespace estimator_concept{

    // E    : estimator type
    // R    : sequence of training points
    template <typename E,typename R,typename B = mpl::empty_base>
    class Trainable : B
    {        
        public:
        BOOST_CONCEPT_USAGE(Trainable)
        {
            train( e, r ); 
        }
    
        private:
        E e;
        R r;
    };

}// estimator_concept
}// statistics
}// boost

#endif