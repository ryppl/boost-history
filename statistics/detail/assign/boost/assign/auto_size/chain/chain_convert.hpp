//////////////////////////////////////////////////////////////////////////////
// assign::detail::chain_convert_impl.hpp                                   //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_CHAIN_CONVERT_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_CHAIN_CONVERT_ER_2010_HPP
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

namespace adaptor{

template<typename R1,
    typename Conv = detail::pair_traits::meta::apply_conversion>
class chain_convert 
    : public boost::sub_range<R1>
{

    // typedef typename boost::is_const<R1>::type r1_is_const_;
    // typedef typename boost::mpl::not_<r1_is_const_>::type allow_lvalue_;

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
        typedef chain_convert<facade_,Conv> type;
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

    chain_convert(const R1 & r) : super_(r),copy(r){ }
    
    template<typename R2>
    typename result_impl<R2,false>::type
    operator()(R2 & r2)const
    {
        typedef typename result_impl<R2,false>::type result_;
        return result_(chain_convert_impl<Conv>(this->copy,r2));
    }

    template<typename R2>
    typename result_impl<R2,true>::type
    operator()(const R2 & r2)const
    {
        typedef typename result_impl<R2,true>::type result_;
        return result_(chain_convert_impl<Conv>(this->copy,r2));
    }
    
    private:
    mutable boost::sub_range<R1> copy;
};

}// adaptor


template<typename Conv,typename R>
adaptor::chain_convert<Conv,R> 
chain_convert_l(R & r)
{
    typedef adaptor::chain_convert<Conv,R> result_;
    return  result_(r);
}

template<typename Conv,typename R>
adaptor::chain_convert<Conv,const R> 
chain_convert_r(const R & r)
{
    typedef adaptor::chain_convert<Conv,const R> result_;
    return  result_(r);
}

/*

template<typename Conv,typename R1, typename R2>
typename result_of::chain_convert<R2,Conv>::template apply<R1>::type
operator|(R1 & r1, const adaptor::chain_convert<R2,Conv> & adaptor)
{
    return adaptor(r1);
}

template<typename Conv,typename R1, typename R2>
typename result_of::chain_convert<R2,Conv>::template apply<const R1>::type
operator|(const R1 & r1, const adaptor::chain_convert<R2,Conv> & adaptor)
{
    return adaptor(r1);
}
*/

// default

template<typename R>
adaptor::chain_convert<R> 
chain_convert_l(R & r)
{
    typedef adaptor::chain_convert<R> result_;
    return  result_(r);
}

template<typename R>
adaptor::chain_convert<const R> 
chain_convert_r(const R & r) // force const
{
    typedef adaptor::chain_convert<const R> result_;
    return  result_(r);
}


template<typename R1, typename R2>
typename adaptor::chain_convert<R2>::template result_impl<R1,false>::type
operator|(R1 & r1, const adaptor::chain_convert<R2>& adaptor)
{
    return adaptor(r1);
}

template<typename R1, typename R2>
typename adaptor::chain_convert<const R2>::template result_impl<R1,true>::type
operator|(R1 & r1, const adaptor::chain_convert<const R2>& adaptor)
{
    return adaptor(r1);
}


}// assign
}// boost

#endif

