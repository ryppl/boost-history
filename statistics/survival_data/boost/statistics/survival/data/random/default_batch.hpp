///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::random::default_batch.hpp                     //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_RANDOM_DEFAULT_BATCH_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_RANDOM_DEFAULT_BATCH_HPP_ER_2009
#include <boost/random/gen_to_random.hpp>
#include <boost/functional/clock.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/functional/visitor.hpp>
#include <boost/iterator/range_cycle.hpp> 
#include <boost/random/ref_distribution.hpp>
#include <boost/statistics/survival/data/random/batch.hpp>


namespace boost{
namespace statistics{
namespace survival{
namespace data{
namespace random{

    template<
        typename T,
        typename M,
        typename P, 
        typename Rx,    // Range // Covariate values
        typename I = typename meta::random_input<M>::type
    >
    class meta_default_batch{
        // Covariates
        typedef range_cycle<>                                   meta_;
        typedef typename meta_::apply<Rx>::type                 cycle_;
        typedef typename functional::visitor<cycle_>            gcov_;
        public:
        typedef boost::random::gen_to_random<gcov_,I>           rcov_;
        private:
        // Necessary in combination with cycle
        typedef boost::random::ref_distribution<rcov_&>         ref_rcov_;
        typedef shared_ptr<rcov_> shared_; 

        // Clock
        typedef functional::clock<T>                            gc_;
        typedef boost::random::gen_to_random<gc_,I>             rc_;

        public:
        typedef std::size_t                                     size_type;
        typedef gc_                                             clock_type;
    
        typedef batch<T,M,P,rc_,ref_rcov_,I>  type;
        
        typedef typename type::model_parameter_ mp_;

        static rcov_ rcov(
            Rx& rx,             // Not const Rx&? TODO see range_cycle
            size_type offset,
            size_type n

        ){
            return rcov_(
                gcov_(
                    meta_::make(rx,offset,n)
                )
            );
        }
        
        // rcov above must be called from outside.
        static type make(
            const M& m,
            const P& p,
            const clock_type& c,
            rcov_& rcov
        ){
            return type(
                m,p,
                rc_(c),
                ref_rcov_(
                    rcov
                )
            );
        }

    };
    
}// random    
}// data
}// survival
}// statistics
}// boost

#endif