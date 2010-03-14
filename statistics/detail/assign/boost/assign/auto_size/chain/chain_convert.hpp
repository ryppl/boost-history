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
#include <boost/range.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/assign/auto_size/chain/pair_range_traits.hpp>
#include <boost/assign/auto_size/chain/chain_convert_impl.hpp>

namespace boost{
namespace assign{

// This adaptor adds a range conversion operation to that of 
// http://gist.github.com/287791
// developed by MPG.

namespace adaptor{

template<typename R1,
    typename Conv = detail::pair_traits::meta::apply_conversion>
class chain_convert_l 
    : public boost::sub_range<R1>
{

    typedef sub_range<R1> super_;

    public:

    template<typename R2>
    struct result_impl_l{
        typedef typename result_of::chain_convert_impl<Conv,super_,R2>::type facade_;
        typedef chain_convert_l<facade_,Conv> type;
    };

    template<typename R2>
    struct result_impl_r{
        typedef typename result_of::chain_convert_impl<Conv,const super_,const R2>::type facade_;
        typedef chain_convert_l<facade_,Conv> type;
    };

    chain_convert_l(const R1 & r) : super_(r),copy(r){ }
    
    template<typename R2>
    typename result_impl_l<R2>::type
    operator()(R2 & r2)const
    {
        typedef typename result_impl_l<R2>::type result_;
        return result_(chain_convert_impl<Conv>(this->copy,r2));
    }

    template<typename R2>
    typename result_impl_r<R2>::type
    operator()(const R2 & r2)const
    {
        typedef typename result_impl_r<R2>::type result_;
        return result_(chain_convert_impl<Conv>(this->copy,r2));
    }
    
    private:
    mutable boost::sub_range<R1> copy;
};

}// adaptor

template<typename Conv,typename R>
adaptor::chain_convert_l<Conv,R> 
chain_convert(R & r)
{
    typedef adaptor::chain_convert_l<Conv,R> result_;
    return  result_(r);
}

template<typename Conv,typename R>
adaptor::chain_convert_l<Conv,const R> 
chain_convert(const R & r)
{
    typedef adaptor::chain_convert_l<Conv,const R> result_;
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
adaptor::chain_convert_l<R> 
chain_convert(R & r)
{
    typedef adaptor::chain_convert_l<R> result_;
    return  result_(r);
}

template<typename R>
adaptor::chain_convert_l<const R> 
chain_convert(const R & r)
{
    typedef adaptor::chain_convert_l<const R> result_;
    return  result_(r);
}

/*
template<typename R1, typename R2>
typename result_of::chain_convert<R2>::template apply<R1>::type
operator|(R1 & r1, const adaptor::chain_convert<R2>& adaptor)
{
    return adaptor(r1);
}

template<typename R1, typename R2>
typename result_of::chain_convert<R2>::template apply<const R1>::type
operator|(const R1 & r1, const adaptor::chain_convert<R2> & adaptor)
{
    return adaptor(r1);
}
*/

}// assign
}// boost

#endif

