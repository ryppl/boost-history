//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::test::function_ptrs.cpp                            //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_TEST_FUNCTION_PTRS_H_ER_2009
#define LIBS_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_TEST_FUNCTION_PTRS_H_ER_2009
#include <ostream>
#include <boost/concept/assert.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/concept/aggregate/function_ptrs.hpp>

template <typename T>
void test_function_ptrs(std::ostream& os)
{
    os << "test_function_ptrs - >" << std::endl;

   using namespace boost;
   using namespace math;
   
   namespace tk = boost::statistics::detail::distribution_toolkit;
   namespace tk_c = tk::concept; 
    {
        typedef math::chi_squared_distribution<T>           d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtrs<d_>
        ));
    }
    {
        typedef math::exponential_distribution<T>            d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtrs<d_>
        ));
    }
    {

        typedef math::gamma_distribution<T>                  d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtrs<d_>
        ));
    }
    {
        typedef math::normal_distribution<T>                 d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtrs<d_>
        ));
    }
    {
        typedef math::uniform_distribution<T>                d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtrs<d_>
        ));
    }
    {
        typedef math::students_t_distribution<T>             d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtrs<d_>
        ));
    }
    {
        typedef math::normal_distribution<T>                d0_;
        typedef tk::wrapper< const d0_& >                   d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtrs<d_>
        ));
    }
    {
        typedef math::normal_distribution<T>                 d0_;
        typedef tk::location_scale_distribution< d0_ >       d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtrs<d_>
        ));
    }
    {
        typedef math::normal_distribution<T>                 d0_;
        typedef tk::inverse_pdf< d0_ >                       d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtr<d_,tk::fun_wrap::log_unnormalized_pdf_>
        ));
    }
    {
        typedef math::students_t_distribution<T>             d0_;
        typedef math::normal_distribution<T>                 d1_;
        typedef tk::product_pdf<d0_,d1_ >                    d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtr<d_,tk::fun_wrap::log_unnormalized_pdf_>
        ));
    }
    {
        typedef math::students_t_distribution<T>             d0_;
        typedef math::normal_distribution<T>                 d1_;
        typedef typename tk::meta_ratio_pdf<d0_,d1_>::type   d_;
        BOOST_CONCEPT_ASSERT((
            tk_c::FunctionPtr<d_,tk::fun_wrap::log_unnormalized_pdf_>
        ));
    }
    
    os << "<-" << std::endl;
}

#endif