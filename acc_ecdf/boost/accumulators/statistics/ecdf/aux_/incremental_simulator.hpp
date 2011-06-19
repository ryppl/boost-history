//////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                //
//                                                                          //
//  Copyright (C) 2005 Eric Niebler                                         //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_INCREMENTAL_SIMULATOR_ER_2011_HPP
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_INCREMENTAL_SIMULATOR_ER_2011_HPP
#include <cstddef>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>

namespace boost{
//[syntax_ecdf_aux_incremental_simulator
namespace accumulators{
namespace ecdf{    
namespace aux_{
    
    template<typename Gen, typename AccSet, typename Fun>
    class incremental_simulator
    {
//<-
        typedef typename Gen::distribution_type dist_;
        typedef typename dist_::result_type sample_type; 
//->
        public:

        incremental_simulator(Gen gen, AccSet acc, Fun f)/*<-*/
            : gen_( gen ), acc_( acc ), fun_( f )
        {}BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        typedef typename Fun::result_type result_type;

        template<typename N> 
        result_type operator()(N const& n)const/*<-*/
        {
            for( N i = 0; i < n; i++ )
            {
                sample_type x = this->gen_();
                this->acc_( x );
            } // increases sample size by n
              
            return this->fun_( this->accumulator() );
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        Gen const& gen()const/*<-*/
        { 
            return this->gen_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        AccSet const& accumulator()const/*<-*/
        { 
            return this->acc_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        Fun    const& fun()const/*<-*/
        { 
            return this->fun_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

//<-        
        private:
        mutable Gen gen_;
        mutable AccSet acc_;
        Fun fun_;
//->
    };

}// aux_
}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_INCREMENTAL_SIMULATOR_ER_2011_HPP

