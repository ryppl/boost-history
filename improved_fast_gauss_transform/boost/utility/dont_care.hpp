//////////////////////////////////////////////////////////////////////////////
// dont_care.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_UTILITY_DONT_CARE_HPP_ER_2009
#define BOOST_UTILITY_DONT_CARE_HPP_ER_2009
namespace boost{namespace utility{
   //borrowed from Boost.Accumulator
   struct dont_care
    {
        template<typename Args>
        dont_care(Args const &)
        {
        }
    };

}}
#endif
