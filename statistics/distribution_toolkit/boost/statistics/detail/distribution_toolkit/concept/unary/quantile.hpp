//////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::concept::unary::quantile.hpp            //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_CONCEPT_UNARY_QUANTILE_HPP_ER_2009
#define BOOST_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_CONCEPT_UNARY_QUANTILE_HPP_ER_2009
#include <boost/concept_check.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/value.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace concept{

    template<typename D>
    class Quantile{
        public:
    
        BOOST_CONCEPT_USAGE(Quantile){
            val =  quantile(dist,p);
        }

        private:
        typedef typename meta::value<D>::type value_;
        D dist;
        value_ val;
        value_ p;
        
    };

}// concept
}// distribution_toolkit
}// statistics
}// detail
}// boost

#endif