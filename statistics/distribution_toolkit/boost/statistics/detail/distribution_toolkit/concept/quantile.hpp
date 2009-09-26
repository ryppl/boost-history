//////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::concept::quantile.hpp                   //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_CONCEPT_QUANTILE_HPP_ER_2009
#define BOOST_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_CONCEPT_QUANTILE_HPP_ER_2009
#include <boost/concept_check.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/value.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace concept{

    // Why would this fail?
    // If D is in boost::math, you need to bring the function below into the 
    // scope of distribution_toolkit by 
    // a) including (or if necessary creating) the file, 
    // distribution_toolkit/distributions/D/is_math_distribution.hpp
    // where meta::is_math::distribution<D> specializes to mpl::bool_<true> 
    // b) including the relevant file in distribution_toolkit/fwd_math
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