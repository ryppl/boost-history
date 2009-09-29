//////////////////////////////////////////////////////////////////////////////
// cross_validation::k_fold::train_predict.hpp                              //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_DETAIL_CROSS_VALIDATION_K_FOLD_TRAIN_PREDICT_HPP_ER_2009
#define  BOOST_STATISTICS_DETAIL_CROSS_VALIDATION_K_FOLD_TRAIN_PREDICT_HPP_ER_2009
#include <string>
#include <stdexcept>
#include <boost/concept/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/foreach.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <boost/statistics/detail/cross_validation/estimator/concept/predictor.hpp> 
#include <boost/statistics/detail/cross_validation/estimator/concept/trainer.hpp> 
#include <boost/statistics/detail/cross_validation/k_fold/partition.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace cross_validation{
namespace k_fold{ 

    template<
        typename U,typename Ft,typename Fi,typename Fo,typename E,typename It
    >
    It train_predict(
        partition<U,Ft,Fi,Fo>& kfp,
        E estimator,
        It out_iter
    )
    {
        typedef partition<U,Ft,Fi,Fo> part_;
        typedef typename part_::meta_training_range::type   tr_;
        typedef typename part_::meta_input_range::type      ir_;
        typedef typename range_value<ir_>::type             i_;

        BOOST_CONCEPT_ASSERT(( estimator::concept::Trainer<E,tr_> ));
        BOOST_CONCEPT_ASSERT(( 
            cross_validation::estimator::concept::Predictor<E,i_> 
        ));

        // std::back_inserter cannot work
        BOOST_CONCEPT_ASSERT((boost_concepts::IncrementableIterator<It>));
        BOOST_CONCEPT_ASSERT((boost_concepts::WritableIterator<It>));

        estimator.train( kfp.training_range() );

        // typedef typename add_reference<i_>::type ref_i_;
        typedef typename call_traits<i_>::param_type ref_i_;

        BOOST_FOREACH(ref_i_ i, kfp.input_range())
        {
    
            *out_iter = estimator.predict(i);
            ++out_iter;
        }

        return out_iter;
    }

}// k_fold
}// cross_validation
}// detail
}// statistics
}// boost

#endif
