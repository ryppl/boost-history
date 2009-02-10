//////////////////////////////////////////////////////////////////////////////
// ifgt/call_wrapper.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_CALL_WRAPPER_HPP_ER_2009
#define BOOST_MATH_IFGT_CALL_WRAPPER_HPP_ER_2009
#include <boost/mpl/assert.hpp>
namespace boost{namespace math{namespace ifgt{

   /// For use by for_each_evaluator
   template<template<typename> class Method>
   struct call{
        call(){};
   };

}}}
#endif
