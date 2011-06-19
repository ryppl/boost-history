//////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                //
//                                                                          //
//  Copyright (C) 2005 Eric Niebler                                         //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_GEOMETRIC_SERIES_RANGE_ER_2011_HPP
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_GEOMETRIC_SERIES_RANGE_ER_2011_HPP
#include <cstddef>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/iterator_range.hpp>

namespace boost{
//[syntax_ecdf_aux_geometric_series_range
namespace accumulators{
namespace ecdf{
namespace aux_{

    template<typename N = std::size_t>
    struct geometric_series
    {
        
        geometric_series()/*<-*/
            :n_( 1 )
            , factor_( 2 )
        {}BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/
        
        geometric_series(N n, N factor)/*<-*/
            :n_( n )
            , factor_( factor )
        {}BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

        typedef N result_type;

        template<typename T>
        N operator()(T const& anything)const/*<-*/
        {
            N m = this->n();
            this->n_ *= this->factor();
            return m;
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
//->
    };

namespace result_of
{

    template<typename N>
    struct geometric_series_iterator/*<-*/
    {
        typedef transform_iterator<
            geometric_series<N>,
            counting_iterator<N>
        > type;
    }/*->*/;

    template<typename N>
    struct geometric_series_range/*<-*/
    {
        typedef typename geometric_series_iterator<N>::type it_;
        typedef boost::iterator_range<it_> type;
    }/*->*/;
    
}// result_of

    // n *= factor at each of size iterations
    template<typename N>
    typename result_of::geometric_series_range<N>::type
    geometric_series_range(N n, N factor, N size)/*<-*/
    {
        typedef geometric_series<N> geom_;
    
        return boost::adaptors::transform(
            boost::make_iterator_range(
                boost::counting_iterator<N>( 0 ),
                boost::counting_iterator<N>( size )
            ), 
            geom_( n, factor )
        );
    }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

}// aux_
}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_AUX_GEOMETRIC_SERIES_RANGE_ER_2011_HPP
