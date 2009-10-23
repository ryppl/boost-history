///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::algorithm::mean_events_by_covariate.hpp       //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_ALGORITHM_MEAN_EVENTS_BY_COVARIATE_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_ALGORITHM_MEAN_EVENTS_BY_COVARIATE_HPP_ER_2009
#include <vector>
#include <boost/foreach.hpp>
#include <boost/utility.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/statistics/survival/data/data/mean_event.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/functional/find.hpp>
#include <boost/binary_op/functional/match_accumulator.hpp>
#include <boost/binary_op/algorithm/for_each.hpp>
#include <boost/binary_op/algorithm/heads.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{

    // Updates a collection of mean_events, one for each covariate value.
    //
    // Requirements
    // Let [b_x1,e_x1) denote a sequence of covariate values.
    // i = f(b_x1,e_x1,x) must be in [b_x1,e_x1]. 
    //
    // If i == e_x1, x is put at the end to [b_x1,e_x1)
    //
    // Another valid choice for F is nn1::find 
    template<typename T,typename X,typename F = functional::find>
    class mean_events_by_covariate{
        public:
        typedef mean_event<T> mean_event_;
        typedef tuple<X,mean_event_> tuple_;
        typedef std::vector<tuple_> tuples_type;
        
        // Construct
        mean_events_by_covariate();
        template<typename Rx> mean_events_by_covariate(const Rx&);
        template<typename Rx> mean_events_by_covariate(const Rx&,const F& f);
        mean_events_by_covariate(const mean_events_by_covariate&);
        mean_events_by_covariate& operator=(const mean_events_by_covariate&);

        // Update
        template<typename ItX,typename ItE>
        void operator()(ItX b_x, ItX e_x, ItE b_e);

        // Acces
        const tuples_type& tuples()const;
        template<typename It> It tails(It out); // copies the mean_events

        private:
        F finder_;
        tuples_type tuples_;
        template<typename Rx>
        void set_covs(const Rx& rx);
    };

    // Implementation //

    template<typename T,typename X,typename F>
    mean_events_by_covariate<T,X,F>::mean_events_by_covariate(){}
    
    template<typename T,typename X,typename F>
    template<typename Rx> 
    mean_events_by_covariate<T,X,F>::mean_events_by_covariate(const Rx& rx)
    :finder_(){
        this->set_covs(rx);
    }

    template<typename T,typename X,typename F>
    template<typename Rx>
    mean_events_by_covariate<T,X,F>::mean_events_by_covariate(
        const Rx& rx,const F& f)
    :finder_(f){
        this->set_covs(rx);
    }

    template<typename T,typename X,typename F>
    template<typename Rx>
    void mean_events_by_covariate<T,X,F>::set_covs(const Rx& rx){
        typedef typename range_value<Rx>::type x_;
        mean_event_ me;
        BOOST_FOREACH(const x_& x,rx){
            tuple_ t(x,me);
            tuples_.push_back( t );
        }
    }
        
    template<typename T,typename X,typename F>
    template<typename ItX,typename ItE>
    void mean_events_by_covariate<T,X,F>::operator()(ItX b_x,ItX e_x,ItE b_e)
    {
        typedef binary_op::match_accumulator<tuples_type&,F> functor_;
        std::for_each(
            make_zip_iterator( make_tuple( b_x, b_e ) ),
            make_zip_iterator( 
                make_tuple( 
                    e_x, 
                    boost::next(
                        b_e,
                        std::distance(b_x,e_x)
                    ) 
                ) 
            ),
            functor_(this->tuples_)
        );
    }

    // Access
    template<typename T,typename X,typename F>
    const typename mean_events_by_covariate<T,X,F>::tuples_type& 
    mean_events_by_covariate<T,X,F>::tuples()const{
        return (this->tuples_);
    }

    template<typename T,typename X,typename F>
    template<typename It> 
    It mean_events_by_covariate<T,X,F>::tails(It out){
        return binary_op::tails(
            boost::begin(this->tuples()),
            boost::end(this->tuples()),
            out
        );
    }

}// data        
}// survival
}// statistics
}// boost
    
#endif