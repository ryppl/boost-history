//////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                //
//                                                                          //
//  Copyright (C) 2005 Eric Niebler                                         //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_KOLMOGOROV_SMIRNOV_OUTPUT_ER_2011_HPP
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_KOLMOGOROV_SMIRNOV_OUTPUT_ER_2011_HPP
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/accumulators/statistics/ecdf/kolmogorov_smirnov_statistic.hpp>
#include <boost/format.hpp>
#include <boost/numeric/conversion/bounds.hpp>

namespace boost{
//[syntax_ecdf_aux_kolmogorov_smirnov_output
namespace accumulators{
namespace ecdf{
namespace aux_{

    // Template parameters:
    // D distribution
    // T value-type
    template<typename D, typename T = typename D::value_type>
    class kolmogorov_smirnov_output
    {
//<-
        typedef boost::numeric::bounds<T> bounds_;
//->
        public:

        typedef boost::format result_type;
        typedef T value_type;

        kolmogorov_smirnov_output(
            D dist
            , result_type custom_format = result_type( "(%1%, %2%)" )
            , value_type old_stat = bounds_::highest()
        )/*<-*/
            :     d_( dist ), 
                f_( custom_format ), 
                old_stat_( old_stat )
        {}BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        result_type heading()const/*<-*/
        { 
            return result_type( "(count, statistic, decreased)" ); 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        
        
        template<typename AccSet>
        result_type operator()( const AccSet& acc_set )const/*<-*/
        {
            namespace acc = accumulators;
            value_type new_stat = ecdf::kolmogorov_smirnov_statistic<
                value_type
            >( acc_set, this->distribution() );

            this->old_stat_ = new_stat;
            result_type result = this->f_;
            result
                % acc::extract::count( acc_set )
                % this->statistic();
            return result;
            
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        D const& distribution()const/*<-*/
        { 
            return this->d_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        value_type const& statistic()const/*<-*/
        { 
            return this->old_stat_; 
        }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
        private:
        
        D d_;
        result_type f_;
        mutable value_type old_stat_;
//->
    };

}// aux_
}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_KOLMOGOROV_SMIRNOV_OUTPUT_ER_2011_HPP

