///////////////////////////////////////////////////////////////////////////////
// boost::functional::clock.hpp                                              //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_FUNCTIONAL_CLOCK_HPP_ER_2009
#define BOOST_FUNCTIONAL_CLOCK_HPP_ER_2009
#include <iostream>

namespace boost{
namespace functional{

    // Models Generator
    //
    // This clock generates equally spaced times
    template<typename T>
    struct clock{
        public:
        clock();
        clock(const T& start_time,const T& delta_time);
        clock(const clock&);
        clock& operator=(const clock&);

        typedef T result_type;

        result_type operator()();

        template<typename T1>
        friend std::istream& operator>>(std::istream& is, clock<T1>& c);

        private:
        T start_time_;
        T delta_time_;  
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const clock<T>& c);
   
    // Implementation //

    template<typename T>
    std::ostream& operator<<(std::ostream& out,const clock<T>& c){
        out << '(' << c.start_time_ << ',' << c.delta_time_ << ')';
        return out;
    }
    
    template<typename T>
    std::istream& operator>>(std::istream& is, clock<T>& c){
        is >> std::ws >> c.start_time_ >> std::ws >> c.delta_time_ >> std::ws;
        return is;
    }
    
    template<typename T>
    clock<T>::clock()
    :start_time_(static_cast<T>(0)),delta_time_(static_cast<T>(0)){}
    
    template<typename T>
    clock<T>::clock(const T& start_time,const T& delta_time)
    :start_time_(start_time),delta_time_(delta_time){}

    template<typename T>
    clock<T>::clock(const clock& that)
    :start_time_(that.start_time_),delta_time_(that.delta_time_){}
    
    template<typename T>
    clock<T>& 
    clock<T>::operator=(const clock& that){
        if(&that!=this){
            start_time_ = (that.start_time_);
            delta_time_ = (that.delta_time_);
        }
        return *this;
    }

    template<typename T>
    typename clock<T>::result_type 
    clock<T>::operator()(){
        result_type res = start_time_;
        start_time_ += delta_time_;
        return res;
    }

}// functional
}// boost

#endif