/*
//////////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::random::generator.hpp                                  //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DISTRIBUTION_TOOLKIT_RANDOM_GENERATOR_HPP_ER_2009
#define BOOST_DISTRIBUTION_TOOLKIT_RANDOM_GENERATOR_HPP_ER_2009
#include <boost/random/variate_generator.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_scalar_distribution.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/random.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{

namespace random{
namespace meta{

    // U = V or U = V& allowed where V models UniformRandomNumberGenerator
    template<typename D,typename U>
    struct generator{
        typedef distribution::toolkit::meta::random<D> meta_;
        typedef typename meta_::type r_;;
        typedef variate_generator<U,r_> type;
        static type call(U urng,const D& d){
            return type(
                urng,
                make_random(d)
            );
        }

    };
}// meta
}// random
}// distribution
}// toolkit

}// detail
}// statistics


    template<typename D,typename U>
    typename lazy_enable_if<
        statistics::detail::distribution::toolkit
            ::meta::is_scalar_distribution<D>,
        statistics::detail::distribution::toolkit::random::meta::generator<D,U&> 
    >::type
    make_random_generator(U& urng,const D& dist){
        typedef statistics::detail::distribution::toolkit::random
            ::meta::generator<D,U&> meta_;
        return meta_::call(urng,dist);
    }

        
}// boost


#endif
*/
