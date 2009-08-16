//////////////////////////////////////////////////////////////////////////////
// standard_distribution::primitives::location_scale.hpp                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_LOCATION_SCALE_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_LOCATION_SCALE_HPP_ER_2009
#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/not.hpp>
#include <boost/arithmetic/equal.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/standard_distribution/transformation/location_scale.hpp>
#include <boost/standard_distribution/primitives/meta.hpp>
#include <boost/operators.hpp>

namespace boost {
namespace math{
    
    // Z is a primitives
    template<typename Z,typename T>
    struct location_scale_distribution_primitives 
        : equality_comparable< location_scale_distribution_primitives<Z,T> >{
        Z z;
        T mu;
        T sigma;

        location_scale_distribution_primitives();
        
        // Z1 is a distribution
        template<typename Z1>
        location_scale_distribution_primitives(
            const location_scale_distribution<Z1>& that
        );

        template<class A>
        void serialize(
            A & ar, 
            const unsigned int version
        );
        
        bool operator==(const location_scale_distribution_primitives&)const;
        
        template<typename Z1>
        operator location_scale_distribution<Z1> ()const;

    };

    template<typename Z1>
    struct meta_distribution_primitives< location_scale_distribution<Z1> >{
        typedef typename Z1::value_type val_;
        typedef typename meta_distribution_primitives<Z1>::type prim_;
        typedef location_scale_distribution_primitives<prim_,val_> type;
    };

    template<typename Z,typename T>
    location_scale_distribution_primitives<Z,T>
    make_distribution_primitives(const location_scale_distribution<Z>& d);
    
    template<typename Z,typename T,typename Z1>
    location_scale_distribution_primitives<Z,T>& 
    operator<<(
        location_scale_distribution_primitives<Z,T>& a,
        const location_scale_distribution<Z1>& b
    );

    // Implementation

    template<typename Z,typename T>
        template<typename Z1>
    location_scale_distribution_primitives<Z,T>::operator 
        location_scale_distribution<Z1> ()const{
        Z1 z1 = this->z; //conversion
        typedef location_scale_distribution<Z1> res_;
        return res_(z1,this->mu,this->sigma);
    }


    template<typename Z,typename T>
    location_scale_distribution_primitives<Z,T>
        ::location_scale_distribution_primitives()
    :z(),
    mu(static_cast<T>(0)),
    sigma(static_cast<T>(0)){}
        
    template<typename Z,typename T>
    template<typename Z1>
    location_scale_distribution_primitives<Z,T>
        ::location_scale_distribution_primitives(
        const location_scale_distribution<Z1>& that
    )
    :z(
        make_distribution_primitives(
            that.z()
        )
    ),
    mu(that.mu()),
    sigma(that.sigma()){ 
        typedef typename meta_distribution_primitives<Z1>::type prim_;
        BOOST_MPL_ASSERT((
            is_same<prim_,Z>
        ));
        BOOST_MPL_ASSERT((
            mpl::not_<
                is_reference<Z1>
            >
        ));
        BOOST_MPL_ASSERT((
            mpl::not_<
                is_pointer<Z1>
            >
        ));
    }

    template<typename Z,typename T>
    bool location_scale_distribution_primitives<Z,T>::operator==(
        const location_scale_distribution_primitives& b
    )const{
        return (this->z == b.z) &&
            arithmetic_tools::equal(this->mu,b.mu) &&
            arithmetic_tools::equal(this->sigma,b.sigma);
    }

    template<typename Z,typename T>
    template<class A>
    void location_scale_distribution_primitives<Z,T>::serialize(
        A & ar, 
        const unsigned int version
    )
    {
        ar & z;
        ar & mu;
        ar & sigma;
    }

    template<typename Z1>
    typename meta_distribution_primitives<
        location_scale_distribution<Z1> 
    >::type
    make_distribution_primitives(const location_scale_distribution<Z1>& d){
        typedef typename meta_distribution_primitives<
            location_scale_distribution<Z1> 
        >::type prim_;
        return prim_(d);
    }

    template<typename Z,typename T>
    location_scale_distribution_primitives<Z,T>& 
    operator<<(
        location_scale_distribution_primitives<Z,T>& a,
        const location_scale_distribution<Z>& b
    ){
        typedef location_scale_distribution_primitives<Z,T> a_;
        a = a_(make_distribution_primitives(b.z()),b.mu(),b.sigma());
        return a;
    }


}// math
}// boost

#endif