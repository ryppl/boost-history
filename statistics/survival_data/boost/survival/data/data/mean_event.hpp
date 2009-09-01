///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data:data::mean_event.hpp                           //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_DATA_MEAN_EVENT_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_DATA_MEAN_EVENT_HPP_ER_2009
#include <stdexcept>
#include <boost/type_traits.hpp>
#include <boost/range.hpp>
#include <boost/format.hpp>
#include <boost/operators.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/survival/constant.hpp>
#include <boost/math/special_functions/fpclassify.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{


    // This class can be used to accumulate the proportion of failures and 
    // mean event time in a sequence of events.
    //
    // In the case of the exponential, a collection of mean_events can
    // form a sufficient statistic,  assuming a discretized domain for
    // the covariate: domain(x) = {x[k]:k=0,...,K-1}
    //
    // $\log L_i(\beta) = \nu_i \eta_i - t_i \exp(\eta_i)$
    // where $\eta_i = \langle x_i,beta \rangle$
    // log L(D,\beta) = sum{
    //    <x[k],\beta> sum{\nu[i]:x[i]==x[k]}
    //     -exp(<x[k],\beta>) sum{t[i] : x[i]==x[k]}
    //  :k=1,...,K
    // }
    // Sufficient statistic:
    // {(sum{\nu[i]:x[i]==x[k]},sum{t[i] : x[i]==x[k]}):k=0,...,K-1}
    template<typename T,typename B = bool>
    class mean_event : 
        public event<T,T>, 
        boost::addable<
            mean_event<T,B>, 
            addable<event<T,B> > 
        >
        // TODO equality_comparable<mean_event<T,B> >
    {
    public:
        typedef event<T,B>  event_;
        typedef event<T,T>  super_;
        typedef typename super_::value_type value_type;
        typedef std::size_t                 size_type;
            
        // Construction
        mean_event();
        mean_event(const event_&);
        mean_event(const mean_event&);
        mean_event& operator=(const mean_event&);
            
        // Access
        size_type count()const;
            
        // Operators
        mean_event& operator+=( const mean_event& e );
        mean_event& operator+=( const event_& e );
        mean_event& operator()( const event_& e ); //same as +=
        
        // I/O
        template<class Archive> 
        void serialize(Archive & ar, const unsigned int version);
        
        private:
        value_type impl(size_type n_a,value_type a,size_type n_b,value_type b);
        size_type  count_;
        static super_ convert(const event_&);
    };

    template<typename T,typename B>
    std::ostream& operator<<(std::ostream& out,const mean_event<T,B>& e);

    template<typename E>
    struct meta_mean_event{
        typedef typename E::value_type value_type;
        typedef mean_event<value_type> type;
    };
    
    
    // Implementation //
    
    // Constructor
    template<typename T,typename B>
    mean_event<T,B>::mean_event()
    :super_(static_cast<T>(false),static_cast<T>(0)),
    count_(0){
        // Using super_() instead, would initialize time to inf.
    }

    template<typename T,typename B>
    mean_event<T,B>::mean_event(const event_& e)
    :super_(convert(e)),count_(1){
        const char* msg = "mean_event::mean_event(e), isinf(e.time())";
        if(math::isinf(this->time())){
            throw std::runtime_error(
                msg
            );
        }
    }

    template<typename T,typename B>
    mean_event<T,B>::mean_event(const mean_event& that)
    :super_(that),count_(that.count_){}

    template<typename T,typename B>
    mean_event<T,B>& 
    mean_event<T,B>::operator=(const mean_event& that){
        if(&that!=this){
            super_::operator=( that );
            count_ = that.count_;
        }
        return *this;
    }
    
    // Access
    template<typename T,typename B>
    typename mean_event<T,B>::size_type 
    mean_event<T,B>::count()const{ return count_; }
    
    // Update
    template<typename T,typename B>
    mean_event<T,B>& 
    mean_event<T,B>::operator+=(const event_& e){
        mean_event other(e);
        return ( (*this) += other );
    }
    
    template<typename T,typename B>
    mean_event<T,B>& 
    mean_event<T,B>::operator+=(const mean_event& other){
        size_type n_a = this->count();
        size_type n_b = other.count();
        (this->failure_) 
            = (this->impl(n_a,this->failure(),n_b,other.failure()));
        (this->time_) 
            = (this->impl(n_a,this->time(),n_b,other.time()));
        (this->count_) += n_b;
        return *this;
    }

    template<typename T,typename B>
    mean_event<T,B>& 
    mean_event<T,B>::operator()(const event_& e){
        return (*this)+=e;
    }
    
    //Private
    template<typename T,typename B>
    template<class Archive>
    void mean_event<T,B>::serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<super_>(*this);
        ar & count_;
    }

    template<typename T,typename B>
    typename mean_event<T,B>::value_type 
    mean_event<T,B>::impl(
            size_type n_a,
            value_type a,
            size_type n_b,
            value_type b
    ){
        return ( (n_a * a) + (n_b * b) ) / (n_a + n_b);
    }
    
    template<typename T,typename B>
    std::ostream& operator<<(std::ostream& out,const mean_event<T,B>& that){
        typedef mean_event<T,B> that_;
        typedef typename that_::super_ super_;
        static const char* str = "(%1%,%2%)";
        format f(str);
        f % that.count() % static_cast<const super_&>(that) ;
        out << f.str();
        return out;
    }

    template<typename T,typename B>
    typename mean_event<T,B>::super_ 
    mean_event<T,B>::convert(const event_& e){
        static value_type zero = static_cast<value_type>(0);
        static value_type one = static_cast<value_type>(1);
        value_type t = e.time();
        if(e.failure()){
            return super_( one, t );
        }else{
            return super_( zero, t );
        }
    }

}// data
}// survival
}// statistics
}// boost

#endif
