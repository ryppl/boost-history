///////////////////////////////////////////////////////////////////////////////
// statistics::survival::model::algorithm::prepare_weights.hpp               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_MODEL_FUNCTIONAL_PREPARE_WEIGHTS_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_MODEL_FUNCTIONAL_PREPARE_WEIGHTS_HPP_ER_2009
#include <ostream>
#include <boost/importance_sampling/detail/prepare_weights_impl.hpp> 
#include <boost/model/wrap/aggregate/prior_model_dataset.hpp>
#include <boost/model/algorithm/log_posteriors.hpp>

namespace boost{
namespace survival{
namespace model{  

    // TODO is this deprecated?
    //
    // Maps proposal values and their log_pdfs to importance sampling weights.
    //
    // Warning: Elements in [b_p,e_p) will be rearranged
    template<typename T>
    class prepare_weights : public is::prepare_weights_impl<T> {
        typedef is::prepare_weights_impl<T> super_;
        public:
        typedef T value_type;

        prepare_weights();
        prepare_weights(const T& max_log);

        template<
            typename D,typename M,typename Rx,typename Re,
            typename ItP, typename ItLw
        >
        void operator()(
            boost::model::prior_model_dataset_<D,M,Rx,Re> pmd,
            ItP b_p,    // Proposal values
            ItP e_p,        
            ItLw b_lw,  // Proposal log pdfs
            ItLw o_w     // Outputs weights - not back_inserter
        );
    };

    // Implementation

    template<typename T>
    prepare_weights<T>::prepare_weights():super_(){}

    template<typename T>
    prepare_weights<T>::prepare_weights(const T& max_log):super_(max_log){}

    template<typename T>
    template<
        typename D,typename M,typename Rx,typename Re,
        typename ItP, typename ItLw
    >
    void
    prepare_weights<T>::operator()(
        boost::model::prior_model_dataset_<D,M,Rx,Re> pmd,
        ItP b_p,    // Proposal values
        ItP e_p,        
        ItLw b_lw,  // Proposal log pdfs
        ItLw o_w    // Outputs weights
    ){
        typedef boost::model::prior_model_dataset_<D,M,Rx,Re>   pmd_;
        ItLw b_w = o_w;
        ItLw e_w = boost::model::log_posteriors<value_type>(
            pmd,
            b_p,
            e_p,
            b_lw,
            o_w
        );
        return super_::impl(
            b_w,
            e_w,
            b_p
        ); // This affects the ordering of [b_w,e_w) and [b_p,e_p)
    }

        
}// model
}// survival
}// boost

#endif