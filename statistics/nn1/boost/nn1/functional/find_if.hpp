///////////////////////////////////////////////////////////////////////////////
// nn1::functional::find_if.hpp                                              //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_NN1_FUNCTIONAL_FIND_IF_HPP_ER_2009
#define BOOST_NN1_FUNCTIONAL_FIND_IF_HPP_ER_2009
#include <algorithm>
#include <limits>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/limits.hpp>
#include <boost/nn1/algorithm/find_if.hpp>

namespace boost{
namespace nn1{
namespace functional{ // to avoid ambiguity with algo by the same name

// algorithm::find_if made into a functor
template<template<typename F> class, typename T>
class find_if{
    static bool has_inf;
    static T inf_if; 
    static T highest;
    static T inf_;

    public:
    
    // Constructor
    find_if();
    find_if(const T& t);
    find_if(const find_if& that);
    find_if& operator=(const find_if& that);
    
    // Update
    template<typename It,typename X>
    It operator()(It b,It e,const X& x)const;
    
    // Query
    const T& min_value()const;
    const T& threshold()const;
    
    private:
    mutable T m_;
    T t_;
};

// Static
template<template<typename> class F,typename T>
bool find_if<F,T>::has_inf = std::numeric_limits<T>::has_infinity;

template<template<typename> class F,typename T>
T find_if<F,T>::inf_if = std::numeric_limits<T>::infinity(); 

template<template<typename> class F,typename T>
T find_if<F,T>::highest = boost::numeric::bounds<T>::highest();

template<template<typename> class F,typename T>
T find_if<F,T>::inf_  = find_if<F,T>::has_inf? 
    find_if<F,T>::inf_if : find_if<F,T>::highest;
    
// Constructor
template<template<typename> class F,typename T>
find_if<F,T>::find_if():m_(inf_),t_(inf_){}

template<template<typename> class F,typename T>
find_if<F,T>::find_if(const T& t)
:m_(inf_),t_(t){}

template<template<typename> class F,typename T>
find_if<F,T>::find_if(const find_if& that)
:m_(that.m_),t_(that.t_){}

template<template<typename> class F,typename T>
typename find_if<F,T>::find_if& 
find_if<F,T>::operator=(const find_if& that){
    if(&that!=this){
        m_ = that.m_;
        t_ = that.t_;
    }
    return *this;
}

// Update

template<template<typename> class F,typename T>
template<typename It,typename X>
It find_if<F,T>::operator()(It b,It e,const X& x)const{
    return nn1::find_if<F,It,X,T>(
        b, 
        e, 
        x, 
        m_, 
        this->threshold()
    );
}

// Access

template<template<typename> class F,typename T>
const T& find_if<F,T>::min_value()const{ return m_; }

template<template<typename> class F,typename T>
const T& find_if<F,T>::threshold()const{ return t_; }

}// functional
}// nn1
}// boost

#endif