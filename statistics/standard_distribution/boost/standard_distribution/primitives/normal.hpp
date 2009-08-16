//////////////////////////////////////////////////////////////////////////////
// standard_distribution::primitives::normal.hpp                            //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_NORMAL_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_NORMAL_HPP_ER_2009
#include <boost/arithmetic/equal.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/standard_distribution/primitives/meta.hpp>
#include <boost/operators.hpp>

namespace boost {
namespace math{
    
    template<typename T>
    struct normal_distribution_primitives 
        : equality_comparable< normal_distribution_primitives<T> >{
        T location;
        T scale; 

        normal_distribution_primitives();
        template<typename T1,typename P>
        normal_distribution_primitives(const normal_distribution<T1,P>& that);

        template<class A>
        void serialize(
            A & ar, 
            const unsigned int version
        );
        
        bool operator==(const normal_distribution_primitives&)const;
        
        template<typename P> operator normal_distribution<T,P> ()const;

    };

    template<typename T,typename P>
    struct meta_distribution_primitives< normal_distribution<T,P> >{
        typedef normal_distribution_primitives<T> type;
    };

    template<typename T,typename P>
    normal_distribution_primitives<T>
    make_distribution_primitives(const normal_distribution<T,P>& d);
    
    template<typename T,typename T1,typename P>
    normal_distribution_primitives<T>& 
    operator<<(
        normal_distribution_primitives<T>& a,
        const normal_distribution<T1,P>& b
    );

    // Implementation

    template<typename T>
    template<typename P>
    normal_distribution_primitives<T>::operator 
        normal_distribution<T,P> ()const{
        return normal_distribution<T,P>(this->location,this->scale);
    }

    template<typename T>
    normal_distribution_primitives<T>::normal_distribution_primitives()
    :location(static_cast<T>(0)),scale(static_cast<T>(0)){}
        
    template<typename T>
    template<typename T1,typename P>
    normal_distribution_primitives<T>::normal_distribution_primitives(
        const normal_distribution<T1,P>& that
    )
    :location(that.location()),scale(that.scale()){}

    template<typename T>
    bool normal_distribution_primitives<T>::operator==(
        const normal_distribution_primitives& b
    )const{
        return arithmetic_tools::equal(this->location,b.location) && 
            arithmetic_tools::equal(this->scale,b.scale);
    }

    template<typename T>
    template<class A>
    void normal_distribution_primitives<T>::serialize(
        A & ar, 
        const unsigned int version
    )
    {
        ar & location;
        ar & scale;
    }

    template<typename T,typename P>
    normal_distribution_primitives<T>
    make_distribution_primitives(const normal_distribution<T,P>& d){
        return normal_distribution_primitives<T>(d);
    }

    template<typename T,typename T1,typename P>
    normal_distribution_primitives<T>& 
    operator<<(
        normal_distribution_primitives<T>& a,
        const normal_distribution<T1,P>& b
    ){
        typedef normal_distribution_primitives<T> a_;
        a = a_(b.location(),b.scale());
        return a;
    }


}// math
}// boost

#endif