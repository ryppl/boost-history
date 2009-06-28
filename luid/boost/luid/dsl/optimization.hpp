/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_DSL_OPTIMIZATION_HPP_
#define BOOST_LUID_DSL_OPTIMIZATION_HPP_

//#include "boost/dsl/thread_model.hpp"
//#include "boost/dsl/error_handling.hpp"
//#include "boost/dsl/expr.hpp"
//#include "boost/luid/iccl/posix_time_traits.hpp"
//
//#include <memory>
#include <boost/detail/is_xxx.hpp>
//#include "boost/mpl/bool.hpp"
//#include "boost/interprocess/allocators/allocator.hpp"
//#include "boost/interprocess/managed_shared_memory.hpp"
//#include "boost/interprocess/managed_mapped_file.hpp"
#include <boost/parameter/aux_/void.hpp>
#include <boost/parameter/parameters.hpp>
//#include <boost/parameter/binding.hpp>


namespace boost
{
namespace luid
{
namespace dsl
{

    
// Optimization
struct optimization_marker{};

template<typename T>
struct is_optimization:is_base_and_derived<optimization_marker,T>
{};

template<typename T=parameter::void_>
struct optimization: parameter::template_keyword<optimization<>,T>
                   , optimization_marker
{};

template <typename dummy=parameter::void_> 
struct speed: optimization_marker {};
BOOST_DETAIL_IS_XXX_DEF(speed, speed, 1);

template <typename dummy=parameter::void_> 
struct space: optimization_marker {};
BOOST_DETAIL_IS_XXX_DEF(space, space, 1);

 
} // dsl
} // luid
} // boost
#endif // BOOST_LUID_DSL_OPTIMIZATION_HPP_
