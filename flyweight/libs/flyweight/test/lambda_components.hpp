/* Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_TEST_LAMBDA_COMPONENTS_HPP
#define BOOST_FLYWEIGHT_TEST_LAMBDA_COMPONENTS_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/functional/hash.hpp> 
#include <boost/mpl/aux_/lambda_support.hpp>
#include <functional>
#include <set>

/* Lambda-enabled replacements for some std:: and boost:: components.
 * These are not really necessary in conformant compilers.
 */

template<typename T>
struct lambda_greater:std::greater<T>
{
  typedef lambda_greater type;
  BOOST_MPL_AUX_LAMBDA_SUPPORT(1,lambda_greater,(T))
};

template<typename T>
struct lambda_equal_to:std::equal_to<T>
{
  typedef lambda_equal_to type;
  BOOST_MPL_AUX_LAMBDA_SUPPORT(1,lambda_equal_to,(T))
};

template<typename T>
struct lambda_hash:boost::hash<T>
{
  typedef lambda_hash type;
  BOOST_MPL_AUX_LAMBDA_SUPPORT(1,lambda_hash,(T))
};

template<typename T>
struct lambda_less:std::less<T>
{
  typedef lambda_less type;
  BOOST_MPL_AUX_LAMBDA_SUPPORT(1,lambda_less,(T))
};

template<
  typename T,
  typename Compare=std::less<T>,typename Allocator=std::allocator<T>
>
struct lambda_set:std::set<T,Compare,Allocator>
{
  typedef lambda_set type;
  BOOST_MPL_AUX_LAMBDA_SUPPORT(3,lambda_set,(T,Compare,Allocator))
};

#endif
