///////////////////////////////////////////////////////////////////////////////
// binary_op::functional::untupler.hpp                                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_BINARY_OP_ALGORITHM_UNTUPLER_HPP_ER_2009
#define BOOST_BINARY_OP_ALGORITHM_UNTUPLER_HPP_ER_2009
#include <boost/call_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/not.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/functional.hpp>

namespace boost{
namespace binary_op{


// Forwards members of a tuple to a binary functor.
//
// Usage:
// Let [b,e) and f an iterator range over tuples and f a binary functor,
// algorithm(b,e,untupler<F>(f)) 

template<typename F,unsigned I0 = 0,unsigned I1 = 1>
class untupler{
    public:
    typedef typename F::result_type result_type;

    typedef F base_type;
    
    // Construction
    untupler();
    untupler(F f);
    untupler(const untupler& that);
    untupler& operator=(const untupler& that);
                
    // Evaluate
    template<typename T> result_type operator()(const T& tuple);    
    template<typename T> result_type operator()(const T& tuple)const;    
    
    operator F ()const{ return f_; }
    
    const base_type& base()const;
    
    private:

    F f_;
};

template<typename F,unsigned I0,unsigned I1>
untupler<F,I0,I1>::untupler()
:f_(){}

template<typename F,unsigned I0,unsigned I1>
untupler<F,I0,I1>::untupler(F f)
:f_(f){}

template<typename F,unsigned I0,unsigned I1>
untupler<F,I0,I1>::untupler(const untupler& that):f_(that.f_){}

template<typename F,unsigned I0,unsigned I1>
untupler<F,I0,I1>& 
untupler<F,I0,I1>::operator=(const untupler& that){
    if(&that!=this){
        f_ = that.f_;
    }
    return *this;
}
        
template<typename F,unsigned I0,unsigned I1>
template<typename T>
typename untupler<F,I0,I1>::result_type 
untupler<F,I0,I1>::operator()(const T& t){
    return this->f_(get<I0>(t),get<I1>(t));
}

template<typename F,unsigned I0,unsigned I1>
template<typename T>
typename untupler<F,I0,I1>::result_type 
untupler<F,I0,I1>::operator()(const T& t)const{
    return this->f_(get<I0>(t),get<I1>(t));
}

template<typename F,unsigned I0,unsigned I1>
const typename untupler<F,I0,I1>::base_type& 
untupler<F,I0,I1>::base()const{
    return this->f_;
}

}//functional
}//boost

#endif