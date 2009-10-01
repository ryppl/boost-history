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
#include <boost/concept/assert.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/concept/aggregate/function_ptrs.hpp>


template <typename T>
void test_function_ptrs()
{
   using namespace boost;
   using namespace math;
   
   namespace tk = boost::statistics::detail::distribution_toolkit;
   namespace tk_c = tk::concept; 

   typedef math::chi_squared_distribution<T>            chi_squared_;
   BOOST_CONCEPT_ASSERT((
        tk_c::FunctionPtrs<chi_squared_>
   ));

   typedef math::exponential_distribution<T>            exponential_;
   BOOST_CONCEPT_ASSERT((
        tk_c::FunctionPtrs<exponential_>
   ));

   typedef math::gamma_distribution<T>                  gamma_;
   BOOST_CONCEPT_ASSERT((
        tk_c::FunctionPtrs<gamma_>
   ));

   typedef math::normal_distribution<T>                 normal_;
   BOOST_CONCEPT_ASSERT((
        tk_c::FunctionPtrs<normal_>
   ));

   typedef math::uniform_distribution<T>                uniform_;
   BOOST_CONCEPT_ASSERT((
        tk_c::FunctionPtrs<uniform_>
   ));

   typedef math::students_t_distribution<T>             students_t_;
   BOOST_CONCEPT_ASSERT((
        tk_c::FunctionPtrs<students_t_>
   ));

   //typedef tk::wrapper< const normal_& >                wrapper_;
   //BOOST_CONCEPT_ASSERT((
   //     tk_c::FunctionPtrs<wrapper_>
   //));

   typedef tk::location_scale_distribution< normal_ >   loc_scale_;
   BOOST_CONCEPT_ASSERT((
        tk_c::FunctionPtrs<loc_scale_>
   ));

   // Weaker requirements 
   typedef tk::inverse_pdf< normal_ >                   inv_;
   BOOST_CONCEPT_ASSERT((
        tk_c::LogUnnormalizedPdf<inv_>
   ));

   typedef tk::product_pdf< normal_,students_t_ >       prod_;
   BOOST_CONCEPT_ASSERT((
        tk_c::LogUnnormalizedPdf<prod_>
   ));

   typedef typename
    tk::meta_ratio_pdf< normal_,students_t_ >::type     ratio_;
   BOOST_CONCEPT_ASSERT((
        tk_c::LogUnnormalizedPdf<ratio_>
   ));

}

#endif