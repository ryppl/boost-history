//////////////////////////////////////////////////////////////////////////////
// standard_distribution::primitives::gamma.hpp                            //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_GAMMA_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_GAMMA_HPP_ER_2009
#include <boost/arithmetic/equal.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/math/distributions/gamma.hpp>
#include <boost/standard_distribution/primitives/meta.hpp>
#include <boost/operators.hpp>

namespace boost {
namespace math{
    
    template<typename T>
    struct gamma_distribution_primitives 
        : equality_comparable< gamma_distribution_primitives<T> >{
        T shape;

        gamma_distribution_primitives();
        template<typename T1,typename P>
        gamma_distribution_primitives(const gamma_distribution<T1,P>& that);

        template<class A>
        void serialize(
            A & ar, 
            const unsigned int version
        );
        
        bool operator==(const gamma_distribution_primitives&)const;

        template<typename P> operator gamma_distribution<T,P> ()const;

    };

    template<typename T,typename P>
    struct meta_distribution_primitives< gamma_distribution<T,P> >{
        typedef gamma_distribution_primitives<T> type;
    };

    template<typename T,typename P>
    gamma_distribution_primitives<T>
    make_distribution_primitives(const gamma_distribution<T,P>& d);
    
    template<typename T,typename T1,typename P>
    gamma_distribution_primitives<T>& 
    operator<<(
        gamma_distribution_primitives<T>& a,
        const gamma_distribution<T1,P>& b
    );

    // Implementation

    template<typename T>
    template<typename P>
    gamma_distribution_primitives<T>::operator 
        gamma_distribution<T,P> ()const{
        return gamma_distribution<T,P>(this->shape);
    }

    template<typename T>
    gamma_distribution_primitives<T>::gamma_distribution_primitives()
    :shape(static_cast<T>(0)){}
        
    template<typename T>
    template<typename T1,typename P>
    gamma_distribution_primitives<T>::gamma_distribution_primitives(
        const gamma_distribution<T1,P>& that
    )
    :shape(that.shape()){}

    template<typename T>
    bool gamma_distribution_primitives<T>::operator==(
        const gamma_distribution_primitives& b
    )const{
        return arithmetic_tools::equal(this->shape,b.shape);
    }

    template<typename T>
    template<class A>
    void gamma_distribution_primitives<T>::serialize(
        A & ar, 
        const unsigned int version
    )
    {
        ar & shape;
    }

    template<typename T,typename P>
    gamma_distribution_primitives<T>
    make_distribution_primitives(const gamma_distribution<T,P>& d){
        return gamma_distribution_primitives<T>(d);
    }

    template<typename T,typename T1,typename P>
    gamma_distribution_primitives<T>& 
    operator<<(
        gamma_distribution_primitives<T>& a,
        const gamma_distribution<T1,P>& b
    ){
        typedef gamma_distribution_primitives<T> a_;
        a = a_(b.shape());
        return a;
    }


}// math
}// boost

#endif