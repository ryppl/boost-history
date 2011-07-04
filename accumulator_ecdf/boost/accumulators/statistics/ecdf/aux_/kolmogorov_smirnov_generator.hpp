//////////////////////////////////////////////////////////////////////////////
//  accumulator_ecdf                                                        //
//                                                                          //
//  Copyright (C) 2005 Eric Niebler                                         //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_KOLMOGOROV_SMIRNOV_GENERATOR_ER_2011_HPP
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_KOLMOGOROV_SMIRNOV_GENERATOR_ER_2011_HPP
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/simulator.hpp>
#include <boost/accumulators/statistics/ecdf/kolmogorov_smirnov_statistic.hpp>
#include <boost/format.hpp>
#include <boost/numeric/conversion/bounds.hpp>

namespace boost{
//[syntax_ecdf_aux_kolmogorov_smirnov_generator
namespace accumulators{
namespace ecdf{
namespace aux_{

    template<
        typename Distribution, 
        typename T = typename Distribution::value_type
    >
    class kolmogorov_smirnov_output
    {
//<-
        typedef boost::numeric::bounds<T> bounds_;
//->
        public:

        typedef boost::format result_type;
        typedef T value_type;

        kolmogorov_smirnov_output(
            Distribution dist
            , result_type custom_format = result_type( "(%1%, %2%)" )
        )/*<-*/
            :    d_( dist ), 
                f_( custom_format )
        {}BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        result_type heading()const/*<-*/
        { 
            return result_type( "(count, statistic)" ); 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        
        template<typename AccumulatorSet>
        result_type operator()( const AccumulatorSet& acc )const/*<-*/
        {
            value_type stat = ecdf::kolmogorov_smirnov_statistic<T>
            ( 
                acc, 
                this->distribution() 
            );

            result_type result = this->f_;
            return result % accumulators::extract::count( acc ) % stat;
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        Distribution const& distribution()const/*<-*/
        { 
            return this->d_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
        private:
        
        Distribution d_;
        result_type f_;
//->
    };

    template<
        typename Generator
        , typename Distribution
        , typename SizeGenerator = delta_geometric_sequence<> 
    >
    class kolmogorov_smirnov_generator
    {
//<-
        typedef typename Generator::distribution_type random_;
        typedef typename random_::result_type sample_type; 

        typedef ecdf::tag::kolmogorov_smirnov_statistic ks_tag_;
        typedef boost::accumulators::stats<ks_tag_> features_;
    
        typedef accumulators::accumulator_set<
            sample_type, 
            features_
        > accumulator_set_;

        typedef simulator<
        	Generator, accumulator_set_, SizeGenerator
        > simulator_;
        typedef kolmogorov_smirnov_output<Distribution> output_;

//->        
        public:
        typedef /*<-*/
            BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/unspecified/*<-*/)
            typename output_::result_type/*->*/ result_type;

        kolmogorov_smirnov_generator(
            Generator gen
            , Distribution dist
        )/*<-*/
        :
            s_( gen, accumulator_set_( ) )
            , o_( dist )
        {}BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        result_type operator()()const/*<-*/
        {
            return this->o_( this->s_() );
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        
        
        simulator_ const& simulator()const/*<-*/
        { 
            return this->s_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        
        output_ const& output()const/*<-*/
        { 
            return this->o_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        
        template<typename String>
        kolmogorov_smirnov_generator set_format(String str)/*<-*/
        {
            this->o_ = output_( this->o_.distribution(), str );
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        
        template<typename SizeGenerator1>
        kolmogorov_smirnov_generator<
        	Generator, accumulator_set_, SizeGenerator1
        >
        set_size_generator(SizeGenerator1 sg)/*<-*/
        {
            typedef kolmogorov_smirnov_generator<
                Generator, accumulator_set_, SizeGenerator1
            > result_;
            typedef aux_::simulator<
            	Generator, 
                accumulator_set_, 
                SizeGenerator1
            > simulator1_;
            return result_
            (
                simulator1_( 
                    this->s_.distribution(), 
                    this->s_.accumulator(),
                    sg
                ),
                this->output()
            );
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
        private:
        simulator_ s_;
        output_ o_;
//->
    };
    
    template<typename Generator, typename Distribution>
    kolmogorov_smirnov_generator<Generator, Distribution>
    make_kolmogorov_smirnov_generator( Generator g, Distribution d )/*<-*/
    {
        typedef kolmogorov_smirnov_generator<Generator, Distribution> result_;
        return result_( g, d );
    }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

}// aux_
}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_KOLMOGOROV_SMIRNOV_GENERATOR_ER_2011_HPP

