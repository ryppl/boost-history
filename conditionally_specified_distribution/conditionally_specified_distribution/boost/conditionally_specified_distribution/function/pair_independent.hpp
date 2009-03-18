/////////////////////////////////////////////////////////////////////////////
// pair_independent.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_FUNCTION_PAIR_IDEPENDENT_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_FUNCTION_PAIR_IDEPENDENT_HPP_ER_2009
#include <boost/conditionally_specified_distribution/crtp/pair_independent.hpp>
#include <boost/conditionally_specified_distribution/result_of/first.hpp>
#include <boost/conditionally_specified_distribution/result_of/second.hpp>
namespace boost{
namespace conditionally_specified_distribution{
namespace function{


    template<typename D,typename Args>
    typename function_value<D>::type
    unnormalized_log_pdf(
        const crtp::pair_independent<D>& d,
        const Args& args,
        typename function_argument<D>::type x
    ){
        const D& d_ = static_cast<const D&>(d);

        return
            unnormalized_log_pdf(d_.first(args),args,x)
           + unnormalized_log_pdf(d_.second(args),args,x);
    };

    template<typename D,typename Args>
    typename function_value<D>::type
    derivative_unnormalized_log_pdf(
        const crtp::pair_independent<D>& d,
        const Args& args,
        typename function_argument<D>::type x
    ){
        const D& d_ = static_cast<const D&>(d);

        // TODO find out why this fails
        /*
        typedef typename
            result_of::template first<D(const Args&)>::type  cref0_t;
        typedef typename
            result_of::template second<D(const Args&)>::type cref1_t;

        cref0_t cref0 = d_.first(args);
        cref1_t cref1 = d_.second(args);

        return
            derivative_unnormalized_log_pdf(cref0,args,x)
            + derivative_unnormalized_log_pdf(cref1,args,x);
        */
        return
            derivative_unnormalized_log_pdf(d_.first(args),args,x)
            + derivative_unnormalized_log_pdf(d_.second(args),args,x);
    };


}
}
}

#endif
