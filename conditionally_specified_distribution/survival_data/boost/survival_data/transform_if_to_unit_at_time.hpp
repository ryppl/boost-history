///////////////////////////////////////////////////////////////////////////////
//  transform_if_to_unit_at_time.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_DATASET_AT_TIME_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_DATASET_AT_TIME_HPP_ER_2009
#include <algorithm>
#include <vector>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/survival_data/unit.hpp>
#include <boost/survival_data/unit_at_time.hpp>
namespace boost{
namespace survival_data{

    template<typename Unit>
    class predicate{
        typedef Unit                            unit_t;
        public:
        typedef typename unit_t::value_type     value_type;
        typedef bool                            result_type;
        typedef const Unit&                     argument_type;
        predicate(value_type t):t_(t){}
        predicate(const predicate& that):t_(that.t_){}
        predicate& operator=(const predicate& that){
            if(&that!=this){
                t_ = that.t_;
            }
            return *this;
        }
        result_type operator()(argument_type arg){
            return (arg.entry_time()<=t_);
        }
        value_type t_;
    };

    template<typename Unit>
    class factory{
        typedef Unit                            unit_t;
        public:
        typedef typename unit_t::value_type     value_type;
        typedef unit_at_time<unit_t>            result_type;
        typedef const Unit&                     argument_type;

        public:
            factory(value_type t): time_analysis_(t){}

            result_type operator()(argument_type u)const{
                return  result_type(u,time_analysis_);
            }

        private:
            factory();
            value_type time_analysis_;
    };

template<typename InputIter,typename OutputIter>
OutputIter
transform_if_to_unit_at_time(
    InputIter   b,
    InputIter   e,
    OutputIter  out,
    typename factory<typename iterator_value<InputIter>::type>::value_type
    time_analysis
)
{
    typedef typename iterator_value<InputIter>::type    unit_t;
    typedef predicate<unit_t>                           pred_t;
    typedef factory<unit_t>                             fact_t;

    return std::transform(
        boost::make_filter_iterator(
            pred_t(time_analysis),
            b,
            e
        ),
        boost::make_filter_iterator(
            pred_t(time_analysis),
            e,
            e
        ),
        out,
        fact_t(time_analysis)
    );
}

}// survival_data
}// boost

#endif
