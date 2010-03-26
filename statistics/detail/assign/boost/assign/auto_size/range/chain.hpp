//////////////////////////////////////////////////////////////////////////////
// assign::detail::range::chain.hpp                                         //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_RANGE_CHAIN_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_RANGE_CHAIN_ER_2010_HPP
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/void.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/range/chain.hpp>
#include <boost/assign/auto_size/range/convert_range.hpp>
#include <boost/assign/auto_size/traits/reference_traits.hpp>
#include <boost/assign/auto_size/reference_wrapper/conversion_traits.hpp>
#include <boost/assign/auto_size/range/result_of_chain.hpp>
#include <boost/assign/auto_size/range/hold_previous.hpp>
#include <boost/assign/auto_size/range/hold_converted_range.hpp>
#include <boost/assign/auto_size/range/basic_chain.hpp>

#include <boost/mpl/assert.hpp> // tmp
#include <boost/type_traits/is_reference.hpp> // tmp

// Usage : Let r1, r2 and r3 denote lvalue-ranges.
//     boost::copy( input, boost::begin( chain_l(r1)(r2)(r3) ) );
// This function does not have the restriction of chain_l that none or all
// of r1, r2, r3 must be reference wrappers.

namespace boost{
namespace assign{
namespace detail{
namespace chain_impl{

    // ---- template parameters ---- //

    // L : converted_range::list<> or empty base if rvalue 
    // E : basic_chain_impl::expr<> 
    // Rng1 : a range to chain to 
    // is_first : indicator
    // V : a value conversion parameter
    // R : a reference conversion parameter

    // ---- fwd decl ---- //
    template<typename L,typename E,typename Rng1,
        bool is_first,typename V,typename R,bool add_const> class expr;


    // ---- traits ---- //

    // Returns the implementation of expr<L,E,Rng1,is_first,V,R> for any L
    template<typename E,typename Rng1,
        bool is_first,typename V,typename R,bool add_const>
    struct impl_of_expr
    {
        typedef typename basic_chain_impl::sel_const<Rng1,add_const>::type cr1_;
        typedef typename 
            detail::result_of::convert_range<cr1_,V,R>::type conv_r1_;
        typedef typename E::template result_impl<conv_r1_>::type type;
    }; 

    template<typename E,typename Rng1,typename V,typename R,bool add_const>
    struct impl_of_expr<E,Rng1,true,V,R,add_const>
    {
        typedef typename basic_chain_impl::sel_const<Rng1,add_const>::type cr1_;
        typedef typename result_of::convert_range<cr1_,V,R>::type conv_r1_;
        typedef typename basic_chain_impl::first_expr<conv_r1_,add_const>::type type;
    };

    typedef boost::mpl::void_ top_;

    template<typename L,typename Rng1,bool is_first,
        typename V,typename R,bool add_const>
    struct sel_list : boost::mpl::eval_if_c<
        add_const,
        boost::mpl::identity<boost::mpl::inherit<boost::mpl::empty_base> >, 
        boost::mpl::identity<converted_range::list<L,Rng1,is_first,V,R> >
    >{
       // mpl::inherit<> is needed to avoid expr<> inheriting from 2 empty
       // bases.
    };
 
    // ---- chainer ---- //
    template<typename L,typename E,typename Rng1,
        bool is_first,typename V,typename R,bool add_const>
	class expr : 
          sel_list<L,Rng1,is_first,V,R,add_const>::type,
        public impl_of_expr<E,Rng1,is_first,V,R,add_const>::type
    {
        typedef expr<L,E,Rng1,is_first,V,R,add_const> this_;

        public:
        
        // bases
        typedef typename sel_list<L,Rng1,is_first,V,R,add_const>::type list_;
        typedef typename impl_of_expr<E,Rng1,
           is_first,V,R,add_const>::type impl_;
                
        // types
        typedef V conversion_value;
        typedef R conversion_reference;

        // lvalue constructors
        explicit expr(Rng1& r1)
            :list_(r1),
            impl_(this->converted_range)
        {}
        explicit expr(L& l,E& e,Rng1& r1)
            :list_(l,r1),
            impl_(e,this->converted_range)
        {}

        // rvalue constructors
        explicit expr(const Rng1& r1)
            :impl_(convert_range<V,R,Rng1>(r1))
        {}
        explicit expr(const E& e,const Rng1& r1)
            :impl_(e,convert_range<V,R,Rng1>(r1))
        {}
                
        // unary operators
        template<typename Rng2>
        struct result_impl 
            { typedef expr<list_,impl_,Rng2,false,V,R,add_const> type; };
 
        template<typename R2,bool b>
        struct enable : boost::enable_if_c<b>{};
 
 
        template<typename Rng2> // Warning : overrides base.
        typename result_impl<Rng2>::type
        operator()(Rng2& r2,typename enable<Rng2,!add_const>::type* = 0)
        {
            typedef typename result_impl<Rng2>::type res_;
            return res_(
                static_cast<list_&>(*this),
                static_cast<impl_&>(*this),
                r2
            );            
        }

        template<typename Rng2> // Warning : overrides base.
        typename result_impl<Rng2>::type
        operator()(const Rng2& r2,
            typename enable<Rng2,add_const>::type* = 0)const
        {
            typedef typename result_impl<Rng2>::type res_;
            return res_(
                static_cast<const impl_&>(*this),
                r2
            );            
        }

        // conversion

        // TODO implement all conversion capabilities

        template< class Container >
        operator Container() const
        {
            return Container(boost::begin(*this),boost::end(*this));
        }

    };

    // ---- traits ---- //

    template<typename Rng1,typename V,typename R, bool add_const>
    struct first_expr{ 
        typedef chain_impl::expr<top_,top_,Rng1,1,V,R,add_const> type; 
        typedef typename basic_chain_impl::sel_const<Rng1,add_const>::type cr1_;
        static type call(cr1_& r1){ return type(r1); }
    };

    template<typename Rng1,typename V,
      typename R = typename detail::convert_range_reference<Rng1,V>::type>
    struct first_expr_l : chain_impl::first_expr<Rng1,V,R,false>{};

    // It's necessary to take add_const<V> or else non-rvalues may occur e.g.
    // Rng1 = array<ref_wrapper<int>,1> and V = int, then R = int*.
    template<typename Rng1,typename V,
      typename R = typename detail::convert_range_reference<const Rng1,
      typename boost::add_const<V>::type>::type>
    struct first_expr_r 
        : chain_impl::first_expr<
            Rng1,typename boost::add_const<V>::type,R,true>{};

    template<typename Rng1,bool add_const>
    struct deduce_value{
        typedef typename boost::range_reference<Rng1>::type from_;
        typedef typename reference_traits::convert_to<from_>::type ref_to_;
        typedef typename boost::remove_reference<ref_to_>::type to_;
        typedef typename basic_chain_impl::sel_const<to_,add_const>::type type;
    };

}// chain_l_impl
}// detail

    // lvalue

    template<typename V,typename R,typename Rng1>
    typename detail::chain_impl::first_expr_l<Rng1,V,R>::type    
    chain_l(Rng1& r1,
        typename boost::disable_if<boost::is_same<R,use_default> >::type* = 0
    ){
        typedef detail::chain_impl::first_expr_l<Rng1,V,R> caller_;
        return caller_::call(r1);
    }

    template<typename V,typename R,typename Rng1>
    typename detail::chain_impl::first_expr_l<Rng1,V>::type    
    chain_l(Rng1& r1,
        typename boost::enable_if<boost::is_same<R,use_default> >::type* = 0
    ){
        typedef detail::chain_impl::first_expr_l<Rng1,V> caller_;
        return caller_::call(r1);
    }

    template<typename Rng1>
    typename detail::chain_impl::first_expr_l<
        Rng1,
        typename detail::chain_impl::deduce_value<Rng1,false>::type
    >::type    
    chain_l(Rng1& r1)
    {
        typedef typename detail::chain_impl::deduce_value<
            Rng1,false
        >::type val_;
        typedef detail::chain_impl::first_expr_l<Rng1,val_> caller_;
        return caller_::call(r1);
    }

    // rvalue : Warning: if is_const<V> != true, R must be value, not a ref. 

    template<typename V,typename R,typename Rng1>
    typename detail::chain_impl::first_expr_r<Rng1,V,R>::type    
    chain_r(const Rng1& r1,
        typename boost::disable_if<boost::is_same<R,use_default> >::type* = 0
    ){
        typedef detail::chain_impl::first_expr_r<Rng1,V,R> caller_;
        return caller_::call(r1);
    }

    template<typename V,typename R,typename Rng1>
    typename detail::chain_impl::first_expr_r<Rng1,V>::type    
    chain_r(const Rng1& r1,
        typename boost::enable_if<boost::is_same<R,use_default> >::type* = 0
    ){
        typedef detail::chain_impl::first_expr_r<Rng1,V> caller_;
        return caller_::call(r1);
    }

    template<typename Rng1>
    typename detail::chain_impl::first_expr_r<
        Rng1,
        typename detail::chain_impl::deduce_value<
            const Rng1,true>::type
    >::type    
    chain_r(const Rng1& r1)
    {
        typedef typename detail::chain_impl::deduce_value<
            const Rng1,true
        >::type val_;
        typedef detail::chain_impl::first_expr_r<Rng1,val_> caller_;
        return caller_::call(r1);
    }

}// assign
}// boost

#endif