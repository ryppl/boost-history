///////////////////////////////////////////////////////////////////////////////
// importance_sampling::effective_sample_size.hpp                            //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_IMPORTANCE_SAMPLING_WEIGHTS_EFFECTIVE_SAMPLE_SIZE_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_IMPORTANCE_SAMPLING_WEIGHTS_EFFECTIVE_SAMPLE_SIZE_HPP_ER_2009
#include <algorithm>
#include <numeric>
#include <iterator>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/ref.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/functional/mean_var_accumulator.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace importance_sampling{
    
    // Finds the number of iid observations whose standard error equates that
    // of the importance sample mean. The number is reported in proportion to
    // the input sample size (m). See sources [3] or [4].
    //
    // n = m / (1+Var(w)) where the w's are standardized weights 
    // i.e. w[j]<- uw[j]/mean(uw[j]:j=1,...,m), solves A = B, where
    //  A = V((1/n) sum{y[i],i=1,...,n}), y~p(y) (iid) 
    //  B = V(sum{y[j]uw[j],i=1,...,m}/sum{uw[j]:j=1,...,m}), y~q, uw propto p/q
    template<typename InIt>
    typename iterator_value<InIt>::type
    percentage_effective_sample_size(
        InIt b_w, // un-normalized weights 
        InIt e_w
    );

    // Implementation //
    
    template<typename InIt>
    typename iterator_value<InIt>::type
    percentage_effective_sample_size(
        InIt b_w, // un-normalized weights 
        InIt e_w
    ){
        typedef typename iterator_value<InIt>::type val_;
        typedef typename functional::mean_var_accumulator<val_>::type acc_;

        // Var(w/c) = Var(w) / c^2

        static val_ one = static_cast<val_>(1);

        acc_ acc;
        std::for_each(
            b_w,
            e_w, 
            lambda::bind<void>(boost::ref(acc),lambda::_1)
        );
        val_ v = static_cast<val_>(
            boost::accumulators::variance(acc)
        );
        val_ c = static_cast<val_>(
            boost::accumulators::mean(acc)
        );
        
        v /= (c*c);

        return one / (one + v);
    }

}// importance_weights       
}// detail     
}// statistics
}// boost

#endif 