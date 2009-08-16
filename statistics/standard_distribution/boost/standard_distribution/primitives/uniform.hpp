//////////////////////////////////////////////////////////////////////////////
// standard_distribution::primitives::uniform.hpp                           //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_UNIFORM_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_UNIFORM_HPP_ER_2009
#include <boost/arithmetic/equal.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/math/distributions/uniform.hpp>
#include <boost/standard_distribution/primitives/meta.hpp>
#include <boost/operators.hpp>

namespace boost {
namespace math{
    
    template<typename T>
    struct uniform_distribution_primitives 
        : equality_comparable< uniform_distribution_primitives<T> >{
        T lower;
        T upper; 

        uniform_distribution_primitives();
        template<typename T1,typename P>
        uniform_distribution_primitives(const uniform_distribution<T1,P>& that);

        template<class A>
        void serialize(
            A & ar, 
            const unsigned int version
        );
        
        bool operator==(const uniform_distribution_primitives&)const;

        template<typename P> operator uniform_distribution<T,P> ()const;

    };

    template<typename T,typename P>
    struct meta_distribution_primitives< uniform_distribution<T,P> >{
        typedef uniform_distribution_primitives<T> type;
    };

    template<typename T,typename P>
    uniform_distribution_primitives<T>
    make_distribution_primitives(const uniform_distribution<T,P>& d);
    
    template<typename T,typename T1,typename P>
    uniform_distribution_primitives<T>& 
    operator<<(
        uniform_distribution_primitives<T>& a,
        const uniform_distribution<T1,P>& b
    );

    // Implementation

    template<typename T>
    template<typename P>
    uniform_distribution_primitives<T>::operator 
        uniform_distribution<T,P> ()const{
        return uniform_distribution<T,P>(this->lower,this->upper);
    }

    template<typename T>
    uniform_distribution_primitives<T>::uniform_distribution_primitives()
    :lower(static_cast<T>(0)),upper(static_cast<T>(0)){}
        
    template<typename T>
    template<typename T1,typename P>
    uniform_distribution_primitives<T>::uniform_distribution_primitives(
        const uniform_distribution<T1,P>& that
    )
    :lower(that.lower()),upper(that.upper()){}

    template<typename T>
    bool uniform_distribution_primitives<T>::operator==(
        const uniform_distribution_primitives& b
    )const{
        return arithmetic_tools::equal(this->lower,b.lower) && 
            arithmetic_tools::equal(this->upper,b.upper);
    }

    template<typename T>
    template<class A>
    void uniform_distribution_primitives<T>::serialize(
        A & ar, 
        const unsigned int version
    )
    {
        ar & lower;
        ar & upper;
    }

    template<typename T,typename P>
    uniform_distribution_primitives<T>
    make_distribution_primitives(const uniform_distribution<T,P>& d){
        return uniform_distribution_primitives<T>(d);
    }

    template<typename T,typename T1,typename P>
    uniform_distribution_primitives<T>& 
    operator<<(
        uniform_distribution_primitives<T>& a,
        const uniform_distribution<T1,P>& b
    ){
        typedef uniform_distribution_primitives<T> a_;
        a = a_(b.lower(),b.upper());
        return a;
    }


}// math
}// boost

#endif