///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::data::event.hpp                               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_DATA_EVENT_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_DATA_EVENT_HPP_ER_2009
#include <limits>
#include <ostream>
#include <boost/format.hpp>
#include <boost/operators.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/statistics/survival/constant.hpp>
#include <boost/arithmetic/equal.hpp>

namespace boost {
namespace statistics{
namespace survival {
namespace data {

    // Abstraction for failure indicator and event time.
    //
    // B == T can be useful to represent a proportion of failures (see
    // mean_event)
    template<typename T,typename B = bool>
    class event : equality_comparable<event<T,B> >{
        typedef constant<T> const_;
        public:
        typedef T value_type;
        typedef B failure_type;
        // Construction 
        event(); //  (f = false, t = infinity)
        explicit event(B isf,value_type rt);
        event(const event&);
        event& operator=(const event&);

        // Access 
        B failure()const;
        value_type time()const;
    
        // Operators 
        bool operator==(const event&);
        
        protected:
        friend class boost::serialization::access;
        
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version);
                
        B failure_;         //!censored
        value_type time_;   //since entry time
    };


    template<typename T,typename B>
    std::ostream& operator<<(std::ostream& out,const event<T,B>& e);
    
    // Implementation //
    
    
    //Construct
    template<typename T,typename B>
    event<T,B>::event():failure_(false),time_(const_::inf_){}
    
    template<typename T,typename B>
    event<T,B>::event(B isf,value_type rt)
    :failure_(isf),time_(rt){}

    template<typename T,typename B>
    event<T,B>::event(const event& that)
    :failure_(that.failure_),time_(that.time_){}

    template<typename T,typename B>
    event<T,B>& 
    event<T,B>::operator=(const event& that){
        if(&that!=this){
            failure_ = (that.failure_);
            time_ = (that.time_);            
        }
        return *this;
    }

    template<typename T,typename B>
    std::ostream& operator<<(std::ostream& out,const event<T,B>& e){
        static const char* str = "(%1%,%2%)";
        format f(str);
        f % e.failure() % e.time();
        out << f.str();
        return out;
    }
            
    template<typename T,typename B>
    B event<T,B>::failure()const{ return failure_; }

    template<typename T,typename B>
    typename event<T,B>::value_type event<T,B>::time()const{ 
        return time_;
    }

    template<typename T,typename B>
    template<class Archive>
    void event<T,B>::serialize(Archive & ar, const unsigned int version)
    {
        ar & failure_;
        ar & time_;
    }

    // Operators
    template<typename T,typename B>
    bool event<T,B>::operator==(const event& e){
        bool eq_1 = arithmetic_tools::equal(
            e.time(),
            (this->time())
        );
        bool eq_2 = arithmetic_tools::equal(
            (this->failure_) , (e.failure())
        );
        return  (eq_1 && eq_2);    
    }

}// data
}// survival
}// statistics
}// boost

#endif