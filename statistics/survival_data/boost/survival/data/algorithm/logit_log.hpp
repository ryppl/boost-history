///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::algorithm::logit_log.hpp                      //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_ALGORITHM_LOGIT_LOG_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_ALGORITHM_LOGIT_LOG_HPP_ER_2009
#include <stdexcept>
#include <iterator>
#include <boost/format.hpp>
#include <boost/survival/data/algorithm/detail/log_shift.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{

// Transforms an alternating sequence of (usually mean) failures 
// and event times to their logit and log, with an offset that prevents nan.
//
// The rationale for a logit is that 0<= mean failure <=1
template<typename It,typename ItO,typename T>
ItO logit_log(
    It b,    // Alternating sequence, f[0],t[0],f[1],t[1],...
    It e,
    ItO o,   // Output iterator
    T t0,
    T t1
){
    static const char* str = "logit_log : distance(b,e) = %1% mod 2 !=2";

   if( std::distance(b,e)%2 != 0 ){
        format f(str); f%distance(b,e);
        throw std::runtime_error(f.str());
   }
   while(b!=e){
        (*o++) = detail::logit_shift((*b++),t0);
        (*o++) = detail::log_shift((*b++),t1);
    }
    return o;
}

}// data
}// survival
}// statistics
}// boost

#endif