///////////////////////////////////////////////////////////////////////////////
// functional::fixed_value.hpp                                               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_FUNCTIONAL_FIXED_VALUE_HPP_ER_2009
#define BOOST_FUNCTIONAL_FIXED_VALUE_HPP_ER_2009
#include <boost/call_traits.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost{
namespace random{

    // Returns a fixed value 
    template<typename T>
    struct fixed_value{
    public:
        typedef typename remove_cv<
            typename remove_reference<
                T
            >::type
        >::type value_type;
    
        fixed_value();
        fixed_value(typename call_traits<T>::param_type value);
        fixed_value(const fixed_value&);
        fixed_value& operator=(const fixed_value&);
        
        
        typedef I                                           input_type;
        typedef typename call_traits<T>::const_reference    result_type;
                
        result_type operator()()const;
        result_type value()const;
        
        // TODO os/is
        
    private:
        typename call_traits<T>::value_type value_;
    };

    // Implementation //
                    
    template<typename T>
    fixed_value<T>::fixed_value()
    :value_(static_cast<value_type>(0)){}
            
    template<typename T>
    fixed_value<T>::fixed_value(typename call_traits<T>::param_type value)
    :value_(value){}
            
    template<typename T>
    fixed_value<T>::fixed_value(const fixed_value& that)
    :value_(that.value_){}
            
    template<typename T>
    fixed_value<T>& 
    fixed_value<T>::operator=(const fixed_value& that){
        if(&that!=this){
            value_ = (that.value_);
        }
        return *this;
    }
            
    template<typename T>
    typename fixed_value<T>::result_type 
    fixed_value<T>::operator()()const{
        return (this->value());
    }
                
    template<typename T>
    typename fixed_value<T>::result_type 
    fixed_value<T>::value()const{
        return (this->value_);
    }

}// functional
}// boost

#endif