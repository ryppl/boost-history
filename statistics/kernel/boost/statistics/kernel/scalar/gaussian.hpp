///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::scalar::gaussian.hpp                                  //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#ifndef BOOST_STATISTICS_KERNEL_SCALAR_GAUSSIAN_H_ER_2009
#define BOOST_STATISTICS_KERNEL_SCALAR_GAUSSIAN_H_ER_2009
#include <cmath>
#include <boost/statistics/kernel/scalar/crtp.hpp>
#include <boost/math/constants/constants.hpp>

namespace boost{
namespace statistics{
namespace kernel{
namespace scalar{

template<typename T>
struct gaussian : scalar::crtp<gaussian<T>,T >{ 
    typedef gaussian<T> this_type;
    typedef scalar::crtp<this_type,T> crtp_;
    public:
    typedef typename crtp_::result_type result_type;
    
    gaussian();
    gaussian(const result_type& bandwidth);
    
    static result_type core_profile(const result_type& x);
    static result_type core_nc;
};

template<typename T>  gaussian<T>::gaussian():crtp_(){}
template<typename T> gaussian<T>::gaussian(const result_type& bandwidth)
:crtp_(bandwidth){}
    
template<typename T>
typename gaussian<T>::result_type
gaussian<T>::core_profile(const result_type& x){
    static result_type two = static_cast<T>(2);
    return exp(- x * x / two);
}
    
template<typename T>
typename gaussian<T>::result_type
gaussian<T>::core_nc = math::constants::root_two_pi<T>();

}// scalar
}// kernel
}// statistics
}// boost

#endif