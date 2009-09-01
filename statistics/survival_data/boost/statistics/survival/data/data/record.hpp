///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::record.hpp                                    //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_RECORDER_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_RECORDER_HPP_ER_2009
#include <ostream>
#include <limits>
#include <boost/operators.hpp>
#include <boost/format.hpp>
#include <boost/math/special_functions/fpclassify.hpp> //isinf
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/statistics/survival/constant.hpp>
#include <boost/statistics/survival/data/data/event.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{

// Abstraction for entry time (t) and the failure time (rt). 
//
// If not set during initialization, the failure time can be set later.
template<typename T>
class record : less_than_comparable<record<T> >{
protected:
    typedef constant<T> const_;
public:
    typedef T value_type;
    record(); //inf,inf
    record(value_type t); 
    record(value_type t, value_type rt);
    // default copy/assign
    void set_failure_time(value_type rt);
    value_type entry_time()const;
    value_type failure_time()const;

    template<typename Archive>
    void serialize(Archive & ar, const unsigned int version);

    bool operator<(const record& other);
    
protected:
    value_type entry_time_;     //t
    value_type failure_time_;   //rt
};

template<typename T>
std::ostream& operator<<(std::ostream& out,const record<T>& r);

template<typename T>
bool operator<(const record<T>& a,const record<T>& b);
    
template<typename T>
typename record<T>::value_type 
time_since_entry(const record<T>& r, typename record<T>::value_type t);
        
// TODO is_at_risk
    
template<typename T>    
event<T>  make_event(const record<T>& r, typename record<T>::value_type t);
    
    // Implementation //
    
// Construct
template<typename T> 
record<T>::record():entry_time_(const_::inf_),failure_time_(const_::inf_){}

template<typename T>
record<T>::record(value_type t):entry_time_(t),failure_time_(const_::inf_){}

template<typename T>
record<T>::record(value_type t,value_type rt)
:entry_time_(t),failure_time_(rt){}

// Assign
template<typename T>
void record<T>::set_failure_time(value_type rt){
    static const char* method = "statistics::survival::record::set_failure_time(%1%)";
    if(math::isinf(this->failure_time_)){
        this->failure_time_ = rt;
    }else{
        static const char* err = " error : overriding failure time";
        throw exception(
            format(method,rt).str(),
            err,
            *this
        );
    }
}

template<typename T>
template<typename Archive>
void record<T>::serialize(Archive & ar, const unsigned int version){
    ar & entry_time_;
    ar & failure_time_;
} 
    
// Access

template<typename T>
typename record<T>::value_type 
record<T>::entry_time()const{ return entry_time_; }

template<typename T>
typename record<T>::value_type 
record<T>::failure_time()const{ return failure_time_; }

template<typename T>
std::ostream& operator<<(std::ostream& out,const record<T>& r){
    out << '(' <<  r.entry_time() << ',' << r.failure_time() << ')';
    return out; 
}
    
template<typename T>
bool record<T>::operator<(const record<T>& other){
    return ( (this->entry_time()) < other.entry_time() );
}

template<typename T>
typename record<T>::value_type 
time_since_entry(const record<T>& r, typename record<T>::value_type t)
{
    //Warning may be <0
    return t-r.entry_time();
}

// TODO is_at_risk

template<typename T>    
event<T>  make_event(const record<T>& r, typename record<T>::value_type t){
    static const char* fun = "statistics::survival::make_event(%1%,%2)";
    typedef typename record<T>::value_type value_t;
    value_t eps = math::tools::epsilon<T>();
    value_t dt = time_since_entry(r,t); 
    if(dt>eps){
        typedef event<T> result_type;
        bool b = ( r.failure_time() <= dt );
        value_t rt = b ? r.failure_time() : dt;
        return result_type(b, rt);
    }else{
        static const char* err = "error: time_since_entry = %1%";
        format f(fun); f%r%t;
        std::string str = f.str();
        f = format(err); f%dt; str+= f.str();
        throw std::runtime_error( str );
    }
}    

}// data                
}// survival
}// statistics
}// boost

#endif