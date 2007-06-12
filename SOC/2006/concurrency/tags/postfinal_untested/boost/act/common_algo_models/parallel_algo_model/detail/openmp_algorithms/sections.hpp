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

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_OMP_SECTIONS_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_OMP_SECTIONS_HPP

// ToDo: Only do if openmp is supported

//#include <omp.h>

#include "../../../parallel_safe.hpp"

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>

#include "../../../config/max_sections.hpp"

namespace boost
{
namespace act
{

template< typename AlgoModel >
struct sections_impl;

template<>
struct sections_impl< parallel_algo_model >
{
  #define BOOST_PP_ITERATION_LIMITS ( 1, BOOST_ACT_MAX_SECTIONS )
  #define BOOST_PP_FILENAME_1 <boost/act/common_algo_models/parallel_algo_model/detail/openmp_algorithms/sections/execute.hpp>
  #include BOOST_PP_ITERATE()
};

}
}

#endif
