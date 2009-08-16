///////////////////////////////////////////////////////////////////////////////
// nn1::detail::ui_abs.hpp                                                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_NN1_DETAIL_UI_ABS_HPP_ER_2009
#define BOOST_NN1_DETAIL_UI_ABS_HPP_ER_2009
#include <cmath>
#include <boost/call_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits.hpp>

namespace boost{
namespace nn1{
namespace detail{

// Functor that computes the distance to an internaly stored variable.
// Used by examples in this library.
template<typename T>
class ui_abs_to{
    typedef typename remove_reference<T>::type const_t_;
    public:
    typedef typename remove_const<const_t_>::type result_type;
    BOOST_MPL_ASSERT((
        boost::is_unsigned<result_type>
    ));
    
    ui_abs_to(typename call_traits<T>::param_type x):x_(x){}
    ui_abs_to& operator=(const ui_abs_to& that){
        if(&that!=this){
            typedef is_reference<T> is_ref_;
            BOOST_MPL_ASSERT(mpl::not_<is_ref_>);
        }
        return *this;
    }
    
    template<typename U>
    result_type operator()(const U& y)const{
        typedef typename boost::remove_reference<T>::type arg_;
        BOOST_MPL_ASSERT((
            boost::is_unsigned<U>
        ));
        return (y<x_)? (x_ - y): (y - x_);
    }

    private:
    typename call_traits<T>::value_type x_;
};

        
}// detail
}// nn1
}// boost
#endif