///////////////////////////////////////////////////////////////////////////////
// statistics::importance_weights::algorithm::prepare_weights.hpp            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_IMPORTANCE_WEIGHTS_ALGORITHM_PREPARE_WEIGHTS_HPP_ER_2009
#define BOOST_STATISTICS_IMPORTANCE_WEIGHTS_ALGORITHM_PREPARE_WEIGHTS_HPP_ER_2009
#include <iterator>
#include <functional>
#include <boost/format.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/math/tools/precision.hpp>
//#include <boost/binary_op/algorithm/sort_on_head.hpp>
#include <boost/statistics/empirical_cdf/algorithm/proportion_less_than.hpp>
#include <boost/importance_weights/algorithm/apply_exp_offset.hpp>
#include <boost/importance_weights/algorithm/scale_to_finite_sum.hpp>
#include <boost/importance_weights/algorithm/effective_sample_size.hpp>

namespace boost{
namespace statistics{
namespace importance_weights{

    // Warning: read side effects carefully.
    template<typename T>
    class prepare_weights{
        public:
        typedef T value_type;
        typedef std::size_t size_type;

        prepare_weights();
        prepare_weights(value_type max_log);
        // Default copy/assign

        // [ Input ]
        // max_log controls precision hence raising it should decr pc_lt_eps
        // but also incr risk that cum_sum isinf. 
        value_type max_log;  
 
        // [ Output ]
        value_type offset;        // lw <- lw + offset, max{lw}+offset = max_log
        value_type scaling_factor;// w <- w/c such that sum{w/c}<inf
        value_type pc_ess;        // pc effective sample size
        value_type pc_lt_eps;     // pc w<eps
        
        // [ Side effect ] 
        // 1) w <- exp(lw+offset)
        // 2) if needed, w <- w/c such that sum{w} < inf
        template<typename ItW>
        void operator()(
            ItW b_w,    // log( unnormalized weights )
            ItW e_w    
        );

        public:
        static value_type zero;
        static value_type eps;
        static value_type default_max_log;
        static const char* header;

    };

    template<typename T>
    std::ostream& operator<<(std::ostream& out, 
        const prepare_weights<T>& that);

    // Implementation

    template<typename T>
    std::ostream& operator<<(
        std::ostream& out, 
        const prepare_weights<T>& that
    ){
        out << 
            (
                format("(%1%,%2%,%3%,%4%)")
                % that.offset
                % that.scaling_factor
                % that.pc_ess
                % that.pc_lt_eps
            ).str();
        return out;
    }

    template<typename T>
    const char* prepare_weights<T>::header 
        = "(offset,scaling_factor,pc_ess,pc_lt_eps)";

    template<typename T>
    typename prepare_weights<T>::value_type
    prepare_weights<T>::eps = math::tools::epsilon<value_type>();

    template<typename T>
    typename prepare_weights<T>::value_type
    prepare_weights<T>::default_max_log = static_cast<value_type>(0);

    template<typename T>
    typename prepare_weights<T>::value_type
    prepare_weights<T>::zero = static_cast<value_type>(0);

    template<typename T>
    prepare_weights<T>::prepare_weights()
    :max_log(default_max_log),
    offset(zero),scaling_factor(zero),pc_ess(zero),pc_lt_eps(zero){}

    template<typename T>
    prepare_weights<T>::prepare_weights(value_type ml)
    :max_log(ml),
    offset(zero),scaling_factor(zero),pc_ess(zero),pc_lt_eps(zero){}
    
    template<typename T>
    template<typename ItW>
    void
    prepare_weights<T>::operator()(
        ItW b_w,
        ItW e_w
    ){
        offset = apply_exp_offset(
            b_w,
            e_w,
            max_log
        );

        // if max_log is small enough (which costs precision), this does not 
        // nothing i.e. scaling_factor = 1
        scaling_factor = scale_to_finite_sum(
            b_w,
            e_w
        ); 

        ItW i_lt_eps = std::lower_bound(
            b_w,
            e_w,
            eps,
            ( boost::lambda::_1 >= boost::lambda::_2 )
        );
        
        value_type n_gt_eps 
            = static_cast<value_type>( std::distance(b_w,i_lt_eps) );
        value_type n_lt_eps 
            = static_cast<value_type>( std::distance(i_lt_eps,e_w) );
        
        
        // Increasing max_log should decrease this number
        pc_lt_eps = n_lt_eps / ( n_lt_eps + n_gt_eps ) ;

        // Beware that pc_lt_eps >0  may distort ess
        pc_ess = percentage_effective_sample_size(
            b_w,
            e_w
        );
        
    }

}// importance_weights
}// statistics
}// boost

#endif