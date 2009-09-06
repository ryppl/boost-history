///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::random::batch.hpp                             //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_RANDOM_BATCH_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_RANDOM_BATCH_HPP_ER_2009
#include <iostream> // TODO remove
#include <boost/format.hpp>
#include <boost/range.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/gen_to_random.hpp>
#include <boost/statistics/model/wrap/aggregate/model_covariate_parameter.hpp>
#include <boost/statistics/survival/data/data/record.hpp>
#include <boost/statistics/survival/data/random/failure_time.hpp>
#include <boost/statistics/survival/data/meta/random_input.hpp>

#include <boost/iterator/range_cycle.hpp> 

namespace boost{
namespace statistics{
namespace survival{
namespace data{
namespace random{

    // Given model and parameter, simulates records (et,ft) like this:
    // et is drawn from a random clock
    // x random
    // ft is drawn from the p(ft|x,p,model)
    //
    // Models RandomDistribution
    template<
        typename T,                                         // float
        typename M,                                         // model        
        typename P,                                         // parameter
        typename RdC,  // RandomDistribution                // clock
        typename RdX,  // RandomDistribution                // covariate
        typename I = typename meta::random_input<M>::type
    >
    class batch : public statistics::model::model_parameter_<M,P>{
        public:
        typedef T                                           value_type;
        typedef I                                           input_type; 
        typedef record<value_type>                          result_type;
        typedef typename RdX::result_type                   covariate_type;

        typedef statistics::model::model_parameter_<M,P>    model_parameter_;
        typedef statistics::model::model_covariate_parameter_<M,covariate_type,P> 
                                                model_covariate_parameter_;

        // Constructor
        batch();
        batch( const M&, const P&,const RdC&,const RdX& );
        batch( const batch& );
        batch& operator=( const batch& );

        // Random generation
        template<typename U>
        result_type operator()(U& u);

        // Access
        const covariate_type& covariate()const; //last generated covariate
        
        private:
        RdC rdc_;
        RdX rdx_;
        covariate_type x_;  
        template<typename U> void gen_x(U&);
    };


    // Implementation //

    //Construction
    template<
        typename T,typename M,typename P, typename RdC,typename RdX,typename I>
    batch<T,M,P,RdC,RdX,I>::batch():model_parameter_(){}

    template<
        typename T,typename M,typename P, typename RdC,typename RdX,typename I>
    batch<T,M,P,RdC,RdX,I>::batch( 
        const M& m,const P& p, const RdC& rdc,const RdX& rdx 
    ):model_parameter_(m,p),rdc_(rdc),rdx_(rdx),x_(){}

    template<
        typename T,typename M,typename P, typename RdC,typename RdX,typename I>
    batch<T,M,P,RdC,RdX,I>::batch( 
        const batch& that
    ):model_parameter_(that),rdc_(that.rdc_),rdx_(that.rdx_),x_(that.x_){}

    template<
        typename T,typename M,typename P, typename RdC,typename RdX,typename I>
    batch<T,M,P,RdC,RdX,I>&
    batch<T,M,P,RdC,RdX,I>::operator=( 
        const batch& that
    ){
        if(&that!=this){
            model_parameter_::operator=( that.model_parameter );
            rdc_ = (that.rdc_);
            rdx_ = (that.rdx_);
            x_ = that.x_;
        }
        return (*this);
    }
    
    // Generation
    template<
        typename T,typename M,typename P, typename RdC,typename RdX,typename I>
    template<typename U> 
    typename batch<T,M,P,RdC,RdX,I>::result_type 
    batch<T,M,P,RdC,RdX,I>::operator()(U& u){
        this->gen_x(u);
        value_type et = (this->rdc_)(u); //entry time
        BOOST_ASSERT(!( et < static_cast<value_type>(0) ) );
        const model_parameter_& mp 
            = static_cast<const model_parameter_&>(*this);
        value_type ft = random::failure_time<value_type>(
            boost::statistics::model::make_model_covariate_parameter(
                mp.model(),
                this->covariate(),
                mp.parameter()
            ),  
            u
        );
        return result_type(et,ft);
    }

    // Access
    template<
        typename T,typename M,typename P, typename RdC,typename RdX,typename I>
    const typename batch<T,M,P,RdC,RdX,I>::covariate_type& 
    batch<T,M,P,RdC,RdX,I>::covariate()const{ return this->x_; }
        
    // Private
    template<
        typename T,typename M,typename P, typename RdC,typename RdX,typename I>
    template<typename U> 
    void batch<T,M,P,RdC,RdX,I>::gen_x(U& urng){
        typedef variate_generator<U&,RdX> vg_;
        // Necessary in the likely case that RdX::input_type != I
        vg_ vg(urng,rdx_); 
        this->x_ = vg();
    }
    
}// random    
}// data
}// survival
}// statistics
}// boost

#endif