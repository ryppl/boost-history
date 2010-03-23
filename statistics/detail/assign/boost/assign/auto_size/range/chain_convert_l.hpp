//////////////////////////////////////////////////////////////////////////////
// assign::detail::range::chain_convert_l.hpp                               //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_RANGE_CHAIN_CONVERT_L_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_RANGE_CHAIN_CONVERT_L_ER_2010_HPP
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/void.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/range.hpp>
#include <boost/range/chain.hpp>
#include <boost/assign/auto_size/range/convert_range.hpp>
#include <boost/assign/auto_size/traits/reference_traits.hpp>
#include <boost/assign/auto_size/reference_wrapper/conversion_traits.hpp>
#include <boost/assign/auto_size/range/result_of_chain.hpp>

// Usage : Let r1, r2 and r3 denote ranges.
//     boost::copy( from, boost::begin( chain_convert_l(r1)(r2)(r3) ) );
// This function does not have the restriction of chain_l that none or all
// of r1, r2, r3 must be reference wrappers.

namespace boost{
namespace assign{
namespace detail{
namespace chain_convert_l_impl{

    template<typename E,typename Rng1,int N,typename V,typename R> class expr;

    template<typename E,typename Rng2>
    struct next_expr{
        typedef typename E::conversion_value val_;
        typedef typename E::conversion_reference ref_;
        typedef expr<E,Rng2,E::static_size+1,val_,ref_> type;
    };

    template<typename E,typename Rng1,int N,typename V,typename R>
    struct super_of_expr : result_of::chain<
        typename E::super_,
        typename result_of::convert_range<Rng1,V,R>::type
    >{}; 

    template<typename E,typename Rng1,typename V,typename R>
    struct super_of_expr<E,Rng1,1,V,R>{
        typedef typename result_of::convert_range<Rng1,V,R>::type type;
    };

    typedef boost::mpl::void_ top_;
    
    template<typename E,typename Rng1,int N,typename V,typename R>
	class expr : public super_of_expr<E,Rng1,N,V,R>::type{
        typedef boost::mpl::int_<1> int_1_;
        typedef boost::mpl::int_<N> int_n_;
        typedef typename boost::mpl::equal_to<int_1_,int_n_>::type is_first_;
        typedef typename boost::mpl::if_<is_first_,E,E&>::type previous_; 

        typedef expr<E,Rng1,N,V,R> this_;
        typedef typename 
            result_of::convert_range<Rng1,V,R>::type converted_r1_impl_;

        public:

        typedef typename super_of_expr<E,Rng1,N,V,R>::type super_;
        typedef typename boost::mpl::if_<
             is_first_,top_,converted_r1_impl_>::type converted_r1_;
        
        typedef V conversion_value;
        typedef R conversion_reference;
        
        template<typename Rng2>
        struct result_impl : next_expr<this_,Rng2>{};
        
        BOOST_STATIC_CONSTANT(int,static_size = N);
        
        super_& super(){ return (*this); }
        const super_& super()const{ return (*this); }
            
        explicit expr(super_ s):super_(s){}
        explicit expr(E& p, Rng1& r1)
            :previous(p),
            converted_r1(convert_range<V,R>(r1))
            {
                // Do not put in the intialization list bec. super may be
                // intialized first whereas last is required.
                this->super() = boost::chain(
                    this->previous.super(),
                    this->converted_r1
                );
            }

        template<typename Rng2>
        typename result_impl<Rng2>::type 
        operator()(Rng2& r2){
            typedef typename result_impl<Rng2>::type res_;
            return res_( *this, r2 );
        }
     
        mutable previous_ previous;
        mutable converted_r1_ converted_r1;
    };

    
    template<typename Rng1,typename V,
        typename R = typename detail::convert_range_reference<Rng1,V>::type>
    struct first_expr{ 
        typedef expr<top_,Rng1,1,V,R> type; 
        typedef typename type::super_ super_;
        static type call(Rng1& r1){
            return type(
                super_(
                    convert_range<V,R>(r1)
                )
            );
        }
    };

    template<typename Rng1>
    struct deduce_value{
        typedef typename boost::range_reference<Rng1>::type from_;
        typedef typename reference_traits::convert_to<from_>::type to_;
        typedef typename boost::remove_reference<to_>::type type;
    };

}// chain_convert_l_impl
}// detail

    template<typename V,typename R,typename Rng1>
    typename detail::chain_convert_l_impl::first_expr<Rng1,V,R>::type    
    chain_convert_l(Rng1& r1,
        typename boost::disable_if<boost::is_same<R,use_default> >::type* = 0
    ){
        typedef detail::chain_convert_l_impl::first_expr<Rng1,V,R> caller_;
        return caller_::call(r1);
    }

    template<typename V,typename R,typename Rng1>
    typename detail::chain_convert_l_impl::first_expr<Rng1,V>::type    
    chain_convert_l(Rng1& r1,
        typename boost::enable_if<boost::is_same<R,use_default> >::type* = 0
    ){
        typedef detail::chain_convert_l_impl::first_expr<Rng1,V> caller_;
        return caller_::call(r1);
    }

    template<typename Rng1>
    typename detail::chain_convert_l_impl::first_expr<
        Rng1,
        typename detail::chain_convert_l_impl::deduce_value<Rng1>::type
    >::type    
    chain_convert_l(Rng1& r1)
    {
        typedef typename detail::chain_convert_l_impl::deduce_value<
            Rng1
        >::type val_;
        typedef detail::chain_convert_l_impl::first_expr<Rng1,val_> caller_;
        return caller_::call(r1);
    }

}// assign
}// boost

#endif