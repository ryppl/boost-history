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
#include <boost/range/chain.hpp>
#include <boost/assign/auto_size/range/convert_range.hpp>
#include <boost/assign/auto_size/traits/reference_traits.hpp>
#include <boost/assign/auto_size/reference_wrapper/conversion_traits.hpp>
#include <boost/assign/auto_size/range/result_of_chain.hpp>
#include <boost/assign/auto_size/range/hold_previous.hpp>
#include <boost/assign/auto_size/range/hold_converted_range.hpp>
#include <boost/assign/auto_size/range/chain_l.hpp>

// Usage : Let r1, r2 and r3 denote lvalue-ranges.
//     boost::copy( from, boost::begin( chain_convert_l(r1)(r2)(r3) ) );
// This function does not have the restriction of chain_l that none or all
// of r1, r2, r3 must be reference wrappers.

// TODO follow the model of chain_impl to allow rvalues.

namespace boost{
namespace assign{
namespace detail{
namespace chain_convert_l_impl{

//    template<typename E,typename Rng1,bool is_first,typename V,typename R> class expr;

    template<typename L,typename E,typename Rng1,
        bool is_first,typename V,typename R> class expr;

    // This is the implementation of
    // expr<L,E,Rng1,is_first,V,R> for any valid E
    template<typename E,typename Rng1,bool is_first,typename V,typename R>
    struct impl_of_expr
    {
        typedef typename 
            detail::result_of::convert_range<Rng1,V,R>::type conv_r1_;
        typedef typename E::template result_impl<conv_r1_>::type type;
    }; 

    template<typename E,typename Rng1,typename V,typename R>
    struct impl_of_expr<E,Rng1,true,V,R>
    {
        typedef typename result_of::convert_range<Rng1,V,R>::type conv_r1_;
        typedef typename chain_impl::first_expr<conv_r1_,false>::type type;
    };

    template<typename E,typename Rng1,bool is_first,typename V,typename R>
    struct facade_of_expr
    {
        typedef typename impl_of_expr<E,Rng1,is_first,V,R>::type impl_;
        typedef typename impl_::facade_ type;
    };

    // This is the result of expr<L,E,...>::operator(Rng2&)
    template<typename L,typename E,typename Rng2>
    struct next_expr{
        typedef typename L::conversion_value val_;
        typedef typename L::conversion_reference ref_;
        typedef expr<L,E,Rng2,false,val_,ref_> type;
    };

    typedef boost::mpl::void_ top_;

    template<typename L,typename E,typename Rng1,
        bool is_first,typename V,typename R>
	class expr : 
        converted_range::list<L,Rng1,is_first,V,R>,
        public impl_of_expr<E,Rng1,is_first,V,R>::type
    {
        typedef expr<L,E,Rng1,is_first,V,R> this_;

        public:

        // types
        typedef V conversion_value;
        typedef R conversion_reference;
        typedef typename impl_of_expr<E,Rng1,is_first,V,R>::type impl_;
        
        // bases
        typedef converted_range::list<L,Rng1,is_first,V,R> list_;
        typedef typename facade_of_expr<E,Rng1,is_first,V,R>::type facade_;
                
        // constructors
        explicit expr(Rng1& r1)
            :list_(r1),
            impl_(this->converted_range)
        {}
        explicit expr(L& l,E& e,Rng1& r1)
            :list_(l,r1),
            impl_(e,this->converted_range)
        {}
                
        // unary operators
        template<typename Rng2>
        struct result_impl 
            : chain_convert_l_impl::next_expr<list_,impl_,Rng2>{};

        template<typename Rng2> // Warning : overrides base.
        typename result_impl<Rng2>::type
        operator()(Rng2& r2)
        {
            typedef typename result_impl<Rng2>::type res_;
            return res_(
                static_cast<list_&>(*this),
                static_cast<impl_&>(*this),
                r2
            );            
        }

    };

    template<typename Rng1,typename V,
        typename R = typename detail::convert_range_reference<Rng1,V>::type>
    struct first_expr{ 
        typedef expr<top_,top_,Rng1,1,V,R> type; 
        static type call(Rng1& r1){ return type(r1); }
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