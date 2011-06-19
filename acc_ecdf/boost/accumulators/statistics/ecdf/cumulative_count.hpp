///////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                 //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_CUMULATIVE_COUNT_HPP_ER_2011
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_CUMULATIVE_COUNT_HPP_ER_2011
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/accumulators/statistics/ecdf/ordered_sample.hpp>
#include <boost/range.hpp>
#include <boost/numeric/conversion/converter.hpp>

namespace boost{ 
//[syntax_ecdf_cumulative_count
namespace accumulators{
namespace ecdf{
//<-
namespace impl{

    // T can be an integer or a float
    template<typename T,typename Comp = std::less<T> >
    class cumulative_count 
        : public accumulator_base
    {
        typedef Comp comp_;
        typedef std::size_t size_;
        typedef dont_care dont_care_;

        public:

        typedef size_ result_type;
        typedef T sample_type;

        cumulative_count(dont_care_){}

        void operator()(dont_care_){}
        
        template<typename Args>
        result_type result(const Args& args)const{
            typedef ecdf::tag::ordered_sample tag_;
            return this->result_impl(
                extract_result<tag_>(
                    args[ accumulators::accumulator ]
                ),
                args[ sample ]
            ); 
        }

        private:
        
        template<typename Map>
        result_type result_impl(
            Map& map, 
            const sample_type& x
        )const{
           return std::for_each(
                boost::const_begin( map ),
                this->bound( map, x ),
                accumulator()
           ).value; 
        }

        template<typename Map>
        typename boost::range_iterator<const Map>::type
        bound(
            const Map& map,
            const sample_type& x
        )const{
            return map.upper_bound(x);
        }

        struct accumulator{
            mutable size_ value;
            
            accumulator():value(0){}
            accumulator(const accumulator& that)
                :value(that.value){}
            
            template<typename Data>
            void operator()(const Data& data)const{
                value += data.second;
            }
        
        };
    };
    
}// impl
//->
namespace tag
{
    struct cumulative_count: depends_on<
        ecdf::tag::ordered_sample
    >
    {/*<-*/
        struct impl{
            template<typename T,typename W>
            struct apply{
                typedef ecdf::impl::cumulative_count<T> type;
            };
        };
    /*->*/};

}// tag
namespace result_of{

    template<typename AccSet>
    struct cumulative_count/*<-*/
    {
        typedef ecdf::tag::cumulative_count tag_;
        typedef typename
            detail::template 
                extractor_result<AccSet,tag_>::type type; 
    }/*->*/;

}// result_of
namespace extract
{

    template<typename AccSet,typename T>
    typename ecdf::result_of::template cumulative_count<AccSet>::type
    cumulative_count(AccSet const& acc,const T& x)/*<-*/
    { 
        typedef ecdf::tag::cumulative_count tag_;
        return extract_result<tag_>(
            acc,
            (sample = x)
        );
    }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

}// extract
}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_CUMULATIVE_COUNT_HPP_ER_2011
