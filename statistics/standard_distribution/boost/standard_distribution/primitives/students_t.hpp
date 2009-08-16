//////////////////////////////////////////////////////////////////////////////
// standard_distribution::primitives::students_t.hpp                        //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_STUDENTS_T_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_PRIMITIVES_STUDENTS_T_HPP_ER_2009
#include <boost/arithmetic/equal.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/math/distributions/students_t.hpp>
#include <boost/standard_distribution/primitives/meta.hpp>
#include <boost/operators.hpp>

namespace boost {
namespace math{
    
    template<typename T>
    struct students_t_distribution_primitives 
        : equality_comparable< students_t_distribution_primitives<T> >{
        T degrees_of_freedom;

        students_t_distribution_primitives();
        template<typename T1,typename P>
        students_t_distribution_primitives(
            const students_t_distribution<T1,P>& that
        );

        template<class A>
        void serialize(
            A & ar, 
            const unsigned int version
        );
        
        bool operator==(const students_t_distribution_primitives&)const;

        template<typename P> operator students_t_distribution<T,P> ()const;

    };

    template<typename T,typename P>
    struct meta_distribution_primitives< students_t_distribution<T,P> >{
        typedef students_t_distribution_primitives<T> type;
    };

    template<typename T,typename P>
    students_t_distribution_primitives<T>
    make_distribution_primitives(const students_t_distribution<T,P>& d);
    
    template<typename T,typename T1,typename P>
    students_t_distribution_primitives<T>& 
    operator<<(
        students_t_distribution_primitives<T>& a,
        const students_t_distribution<T1,P>& b
    );

    // Implementation

    template<typename T>
    template<typename P>
    students_t_distribution_primitives<T>::operator 
        students_t_distribution<T,P> ()const{
        return students_t_distribution<T,P>(this->degrees_of_freedom);
    }


    template<typename T>
    students_t_distribution_primitives<T>::students_t_distribution_primitives()
    :degrees_of_freedom(static_cast<T>(0)){}
        
    template<typename T>
    template<typename T1,typename P>
    students_t_distribution_primitives<T>::students_t_distribution_primitives(
        const students_t_distribution<T1,P>& that
    )
    :degrees_of_freedom(that.degrees_of_freedom()){}

    template<typename T>
    bool students_t_distribution_primitives<T>::operator==(
        const students_t_distribution_primitives& b
    )const{
        return arithmetic_tools::equal(
            this->degrees_of_freedom,
            b.degrees_of_freedom
        );
    }

    template<typename T>
    template<class A>
    void students_t_distribution_primitives<T>::serialize(
        A & ar, 
        const unsigned int version
    )
    {
        ar & degrees_of_freedom;
    }

    template<typename T,typename P>
    students_t_distribution_primitives<T>
    make_distribution_primitives(const students_t_distribution<T,P>& d){
        return students_t_distribution_primitives<T>(d);
    }

    template<typename T,typename T1,typename P>
    students_t_distribution_primitives<T>& 
    operator<<(
        students_t_distribution_primitives<T>& a,
        const students_t_distribution<T1,P>& b
    ){
        typedef students_t_distribution_primitives<T> a_;
        a = a_(b.degrees_of_freedom());
        return a;
    }

}// math
}// boost

#endif

