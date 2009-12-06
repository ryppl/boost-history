/*
//////////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::distributions::aggregate::function_ptrs.hpp            //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DISTRIBUTION_TOOLKIT_CONCEPT_AGGREGATE_FUNCTION_PTRS_HPP_ER_2009
#define BOOST_DISTRIBUTION_TOOLKIT_CONCEPT_AGGREGATE_FUNCTION_PTRS_HPP_ER_2009
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/cdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/quantile.hpp>
#include <boost/statistics/detail/distribution_toolkit/concept/unary/function_ptr.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{

namespace concept{

    template<typename D>
    class FunctionPtrs 
// : 
//        FunctionPtr<D,fun_wrap::cdf_>, 
//        FunctionPtr<D,fun_wrap::pdf_>, 
//        FunctionPtr<D,fun_wrap::log_unnormalized_pdf_>, 
//        FunctionPtr<D,fun_wrap::quantile_>
    {
    };

}// concept
}// distribution
}// toolkit

}// statistics
}// detail
}// boost

#endif
*/