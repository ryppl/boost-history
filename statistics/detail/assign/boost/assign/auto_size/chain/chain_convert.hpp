//////////////////////////////////////////////////////////////////////////////
// assign::detail::chain_convert.hpp                                        //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_CHAIN_CONVERT_ER_MPG_2010_HPP
#define BOOST_ASSIGN_DETAIL_CHAIN_CONVERT_ER_MPG_2010_HPP
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/range.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/assign/auto_size/chain/pair_range_traits.hpp>
#include <boost/assign/auto_size/chain/chain_convert_impl.hpp>

namespace boost{
namespace assign{

// Adds range conversion (needed for assignment in the presence of reference
// wrappers) and support for lvalue to the range adaptor here : 
// http://gist.github.com/287791
// developed by MPG.

// Usage:
// Let r1, r2, r3 denote lvalue ranges:
//    chain_convert_l(r1)(r2)(r3)
// If either of the 3 is an rvalue,
//    chain_convert_r(r1)(r2)(r3)

namespace adaptor{

namespace impl{

template<typename R1,typename Conv>
class chain_converter 
    : public boost::sub_range<R1>
{

    typedef sub_range<R1> super_;

    template<typename R,bool add_const>
    struct qual : boost::mpl::if_c<add_const,
         typename boost::add_const<R>::type,
         R
    >{};

    template<typename X1,typename X2>
    struct result_impl_generic{
        typedef typename result_of::chain_convert_impl<Conv,X1,X2>::type 
            facade_;
        typedef impl::chain_converter<facade_,Conv> type;
    };

    public:

    // This is more helpful than result<F(R&)> so keep it public.
    template<typename R2,bool add_const>
    struct result_impl : result_impl_generic<
       typename qual<super_,add_const>::type,
       typename qual<R2,add_const>::type
    >{};

    template<typename S>
    struct result{};
    
    template<typename F,typename R2> 
    struct result<F(R2&)> : result_impl<R2,false>{};

    template<typename F,typename R2> 
    struct result<F(const R2&)> : result_impl<R2,true>{};

    chain_converter(const R1 & r) : super_(r){}
    
    template<typename R2>
    typename result_impl<R2,false>::type
    operator()(R2 & r2)const
    {
        typedef typename result_impl<R2,false>::type result_;
        this->copy = (*this); 
        return result_(chain_convert_impl<Conv>(this->copy,r2));
        // is it legit ?
    }

    template<typename R2>
    typename result_impl<R2,true>::type
    operator()(const R2 & r2)const
    {
        typedef typename result_impl<R2,true>::type result_;
        this->copy = (*this); 
        return result_(chain_convert_impl<Conv>(this->copy,r2));
        // is it legit ?
    }
    
    private:
    mutable super_ copy;
};
}// impl
}// adaptor

namespace result_of{
    template<typename R,
        typename Conv = detail::pair_traits::meta::apply_conversion>
    struct chain_convert_l{
        typedef adaptor::impl::chain_converter<R,Conv> type;   
    };
    template<typename R,
        typename Conv = detail::pair_traits::meta::apply_conversion>
    struct chain_convert_r{
        typedef adaptor::impl::chain_converter<const R,Conv> type;  
    };
    
}// result_of

template<typename Conv,typename R>
typename result_of::chain_convert_l<R,Conv>::type 
chain_convert_l(R & r)
{
    typedef typename result_of::chain_convert_l<R,Conv>::type result_;
    return  result_(r);
}

template<typename Conv,typename R>
typename result_of::chain_convert_r<R,Conv>::type 
chain_convert_r(const R & r)
{
    typedef typename result_of::chain_convert_r<R,Conv>::type result_;
    return  result_(r);
}

// default

template<typename R>
typename result_of::chain_convert_l<R>::type 
chain_convert_l(R & r)
{
    typedef typename result_of::chain_convert_l<R>::type result_;
    return  result_(r);
}

template<typename R>
typename result_of::chain_convert_r<R>::type 
chain_convert_r(const R & r) 
{
    typedef typename result_of::chain_convert_r<R>::type result_;
    return  result_(r);
}

}// assign
}// boost

#endif

