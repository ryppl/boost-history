//////////////////////////////////////////////////////////////////////////////
//  accumulator_ecdf                                                        //
//                                                                          //
//  Copyright (C) 2005 Eric Niebler                                         //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_SIMULATOR_ER_2011_HPP
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_SIMULATOR_ER_2011_HPP
#include <cstddef>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>

namespace boost{
//[syntax_ecdf_aux_simulator
namespace accumulators{
namespace ecdf{    
namespace aux_{

    template<typename N = std::size_t>
    struct delta_geometric_sequence
    {
        
        delta_geometric_sequence()/*<-*/
            :n_( 1 )
            , factor_( 2 )
            , start( true )
        {}BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        
        delta_geometric_sequence(N n, N factor)/*<-*/
            :n_( n )
            , factor_( factor )
            , start( true )
        {}BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        typedef N result_type;

        N operator()()const/*<-*/
        {
        	if( start )
            {
            	start = false;
            	return this->n();
            }else
            {
            	N m = this->n();
            	this->n_ *= this->factor();
            	return ( this->n() - m );
            }
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        
        N const& n()const/*<-*/
        { 
            return this->n_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        N const& factor()const/*<-*/
        { 
            return this->factor_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        
//<-
        private:
        mutable N n_;
        N factor_;
        mutable bool start;
//->
    };
    
    template<    
        typename Generator, 
        typename AccumulatorSet, 
        typename SizeGenerator = delta_geometric_sequence<> 
    >
    class simulator
    {
//<-
        typedef typename Generator::distribution_type dist_;
        typedef typename dist_::result_type sample_type; 
//->
        public:

        simulator(
            Generator gen, 
            AccumulatorSet acc, 
            SizeGenerator size_gen
        )/*<-*/
            : gen_( gen ), 
            acc_( acc ),
            size_gen_( size_gen )
        {}BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        simulator(Generator gen, AccumulatorSet acc)/*<-*/
            : gen_( gen ), 
            acc_( acc )
        {}BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        typedef AccumulatorSet const& result_type;

        // User specified sample size
        template<typename N>
        result_type operator()(N const& n)const/*<-*/
        {
            for( N i = 0; i < n; i++ )
            {
                sample_type x = this->gen_();
                this->acc_( x );
            }
            return this->accumulator();
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        // Internally generated sample size
        result_type operator()()const/*<-*/
        {
            typedef typename SizeGenerator::result_type n_;
            return (*this)( this-> size_gen_() );
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        Generator const& generator()const/*<-*/
        { 
            return this->gen_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        AccumulatorSet const& accumulator()const/*<-*/
        { 
            return this->acc_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        SizeGenerator const& size_generator()const/*<-*/
        { 
            return this->size_gen_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

//<-        
        private:
        mutable Generator gen_;
        mutable AccumulatorSet acc_;
        mutable SizeGenerator size_gen_;
//->
    };

}// aux_
}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_SIMULATOR_ER_2011_HPP

