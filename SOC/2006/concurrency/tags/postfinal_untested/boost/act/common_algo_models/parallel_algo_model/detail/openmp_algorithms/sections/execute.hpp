//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

template< typename ExtendedParamsType
          BOOST_PP_ENUM_TRAILING_PARAMS( BOOST_PP_ITERATION()
                                       , typename Function
                                       )
        >
static void
execute( ExtendedParamsType const& extended_params
         BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( BOOST_PP_ITERATION()
                                             , Function, fun
                                             )
       )
{
  #pragma omp parallel sections
  {
    #define BOOST_PP_ITERATION_LIMITS ( 0, BOOST_PP_ITERATION() - 1 )
    #define BOOST_PP_FILENAME_2 <boost/act/common_algo_models/parallel_algo_model/detail/openmp_algorithms/sections/individual_section.hpp>
    #include BOOST_PP_ITERATE()
  }
}