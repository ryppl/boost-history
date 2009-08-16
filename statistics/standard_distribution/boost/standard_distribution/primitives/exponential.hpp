//////////////////////////////////////////////////////////////////////////////
// standard_distribution::primitives::exponential.hpp                       //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_EXPONENTIAL_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_EXPONENTIAL_HPP_ER_2009
#include <boost/arithmetic/equal.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/math/distributions/exponential.hpp>
#include <boost/standard_distribution/primitives/meta.hpp>
#include <boost/operators.hpp>

namespace boost {
namespace math{
    
    template<typename T>
    struct exponential_distribution_primitives 
        : equality_comparable< exponential_distribution_primitives<T> >{
        T lambda;

        exponential_distribution_primitives();
        template<typename T1,typename P>
        exponential_distribution_primitives(
            const exponential_distribution<T1,P>& that
        );

        template<class A>
        void serialize(
            A & ar, 
            const unsigned int version
        );
        
        bool operator==(const exponential_distribution_primitives&)const;

        template<typename P> operator exponential_distribution<T,P> ()const;

    };

    template<typename T,typename P>
    struct meta_distribution_primitives< exponential_distribution<T,P> >{
        typedef exponential_distribution_primitives<T> type;
    };

    template<typename T,typename P>
    exponential_distribution_primitives<T>
    make_distribution_primitives(const exponential_distribution<T,P>& d);
    
    template<typename T,typename T1,typename P>
    exponential_distribution_primitives<T>& 
    operator<<(
        exponential_distribution_primitives<T>& a,
        const exponential_distribution<T1,P>& b
    );

    // Implementation

    template<typename T>
    template<typename P>
    exponential_distribution_primitives<T>::operator 
        exponential_distribution<T,P> ()const{
        return exponential_distribution<T,P>(this->lambda);
    }

    template<typename T>
    exponential_distribution_primitives<T>
        ::exponential_distribution_primitives()
    :lambda(static_cast<T>(0)){}
        
    template<typename T>
    template<typename T1,typename P>
    exponential_distribution_primitives<T>::exponential_distribution_primitives(
        const exponential_distribution<T1,P>& that
    )
    :lambda(that.lambda()){}

    template<typename T>
    bool exponential_distribution_primitives<T>::operator==(
        const exponential_distribution_primitives& b
    )const{
        return arithmetic_tools::equal(
            this->lambda,
            b.lambda
        );
    }

    template<typename T>
    template<class A>
    void exponential_distribution_primitives<T>::serialize(
        A & ar, 
        const unsigned int version
    )
    {
        ar & lambda;
    }

    template<typename T,typename P>
    exponential_distribution_primitives<T>
    make_distribution_primitives(const exponential_distribution<T,P>& d){
        return exponential_distribution_primitives<T>(d);
    }

    template<typename T,typename T1,typename P>
    exponential_distribution_primitives<T>& 
    operator<<(
        exponential_distribution_primitives<T>& a,
        const exponential_distribution<T1,P>& b
    ){
        typedef exponential_distribution_primitives<T> a_;
        a = a_(b.lambda());
        return a;
    }

}// math
}// boost

#endif
