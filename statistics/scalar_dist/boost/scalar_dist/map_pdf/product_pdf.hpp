//////////////////////////////////////////////////////////////////////////////
// scalar_dist::map_pdf::product_pdf.hpp                                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_MAP_PDF_PRODUCT_PDF_HPP_ER_2009
#define BOOST_SCALAR_DIST_MAP_PDF_PRODUCT_PDF_HPP_ER_2009
#include <boost/type_traits/is_reference.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/utility.hpp>
#include <boost/call_traits.hpp>
#include <boost/scalar_dist/meta/has_pdf.hpp>

namespace boost{
namespace math{

    // Represents the product of the pdf of two distributions
    //
    // A = T or A = T& or A = T*. Likewise for B.
    template<typename A,typename B>
    class product_pdf{
        typedef is_reference<A> is_ref_a_;
        typedef is_reference<B> is_ref_b_;

        typedef typename remove_reference<
            typename remove_const<
                A
            >::type
        >::type plain_a_t;

        public:
        typedef typename plain_a_t::value_type value_type;
        typedef
            typename call_traits<A>::const_reference result_of_first_type;
        typedef
            typename call_traits<B>::const_reference result_of_second_type;

        product_pdf(){
            BOOST_MPL_ASSERT(( mpl::not_<is_ref_a_> ));
            BOOST_MPL_ASSERT(( mpl::not_<is_ref_b_> ));
        }
        explicit product_pdf(
            typename call_traits<A>::param_type a,
            typename call_traits<B>::param_type b
        ):a_(a),b_(b){}
        product_pdf(const product_pdf& that):a_(that.a_),b_(that.b_){}

        product_pdf&
        operator=(const product_pdf& that){
            if(&that!=this){
                BOOST_MPL_ASSERT(( mpl::not_<is_ref_a_> ));
                BOOST_MPL_ASSERT(( mpl::not_<is_ref_b_> ));
                a_ = that.a_;
                b_ = that.b_;
            }
            return *this;
        }

        result_of_first_type first()const{ return a_; }
        result_of_second_type second()const{ return b_; }

        protected:
        typename call_traits<A>::value_type a_;
        typename call_traits<B>::value_type b_;
    };

    template<typename A,typename B>
    struct has_pdf< product_pdf<A,B> > : mpl::bool_<true> {};

    template<typename A,typename B>
    product_pdf<A,B>
    make_product_pdf(const A& a,const B& b){ return product_pdf<A,B>(a,b); }

    template<typename A,typename B>
    typename product_pdf<A,B>::value_type
    log_unnormalized_pdf(
        const product_pdf<A,B>& dist,
        const typename product_pdf<A,B>::value_type& x
    ){
        return
            log_unnormalized_pdf(dist.first(),x)
                + log_unnormalized_pdf(dist.second(),x);
    }

}
}

#endif