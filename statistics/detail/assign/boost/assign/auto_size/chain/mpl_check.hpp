//////////////////////////////////////////////////////////////////////////////
// assign::detail::chain_mpl_check.hpp                                      //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_CHAIN_MPL_CHECK_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_CHAIN_MPL_CHECK_ER_2010_HPP
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/array.hpp>
#include <boost/type_traits.hpp>
#include <boost/assign/auto_size/chain/reference_wrapper_traits.hpp>
#include <boost/assign/auto_size/chain/reference_traits.hpp>
#include <boost/assign/auto_size/chain/pair_traits.hpp>
#include <boost/assign/auto_size/chain/pair_range_traits.hpp>

namespace boost{
namespace assign{
namespace detail{

namespace chain_mpl_check{
    template<typename T> struct dummy_wrapper;
}

template<typename T>
struct is_ref_wrapper<
    chain_mpl_check::dummy_wrapper<T> 
    > : boost::mpl::bool_<true>{};

namespace ref_wrapper_traits{
    template<typename T>
    struct tag_of<chain_mpl_check::dummy_wrapper<T> >{ 
        typedef tag::default_ type; 
    };
}// ref_wrapper_traits

namespace chain_mpl_check{

    template<typename T>
    struct dummy_wrapper{ 
        operator T& ()const{ return *this->ref; }
        private:
        T* ref;
    };

    template<typename T,bool is = true>
    struct assert_cond{
        BOOST_MPL_ASSERT((T));    
    };

    template<typename T>
    struct assert_cond<T,false>{
        BOOST_MPL_ASSERT_NOT((T));    
    };

    template<typename T,typename U,bool is = true>
    struct assert_eq  
        : assert_cond<boost::is_same<T,U>,is>{};

namespace convert_wrapper{
    template<typename T>
    struct helper : reference_traits::convert_wrapper<T> {};

    template<typename T,typename U,bool is = true>
    struct assert_eq 
        : chain_mpl_check::assert_eq<typename helper<T>::type,U,is>
    {};


}
namespace pair_traits{
namespace convert_wrapper{

    template<typename T,typename U>
    struct helper : 
        detail::pair_traits::meta::convert_wrapper::template apply<T,U> {};

    template<typename T,typename U,typename V,bool is = true>
    struct assert_eq 
        : chain_mpl_check::assert_eq<typename helper<T,U>::type,V,is>
    {
        assert_eq(){
            typedef helper<T,U> helper_;
            helper_::internal_check();
        }
    };

}// filter_ref_const
}// pair_traits

namespace pair_range_traits{

    template<typename R1,typename R2>
    struct helper : detail::pair_range_traits::convert_wrapper<R1,R2>{};
    
    template<typename R1,typename R2,typename T>
    struct assert_eq : chain_mpl_check::assert_eq<
       typename pair_range_traits::helper<R1,R2>::convertible_to_,
       T 
    >{};

    template<typename R1,typename R2>
    void assert_checks()
    {
        typedef pair_range_traits::helper<R1,R2> helper_;
        helper_::internal_check();
        typedef typename helper_::convertible_to_traits traits_;
        BOOST_MPL_ASSERT_NOT((
           traits_::is_wrapper
        ));
    }


}// pair_range_traits

void compound()
{  
        
    typedef int       v_;
    typedef const v_  cv_;
    typedef v_&       r_;
    typedef cv_&      cr_;

    typedef chain_mpl_check::dummy_wrapper<v_> wv_;
    typedef const wv_ cwv_;
    typedef wv_&      rwv_;
    typedef cwv_&     crwv_;

    typedef chain_mpl_check::dummy_wrapper<cv_> wcv_;
    typedef const wcv_ cwcv_;
    typedef wcv_&      rwcv_;
    typedef cwcv_&     crwcv_;
        
    const std::size_t n = 3;
    typedef boost::array<wv_,n> awv_;
    typedef boost::array<cwv_,n> acwv_;
    typedef boost::array<wcv_,n> awcv_;
    typedef boost::array<cwcv_,n> acwcv_;

    typedef const awv_   cawv_;
    typedef const acwv_  cacwv_;
    typedef const awcv_  cawcv_;
    typedef const acwcv_ cacwcv_;
    
    convert_wrapper::assert_eq< v_,    v_   >();
    convert_wrapper::assert_eq< cv_,   cv_  >();
    convert_wrapper::assert_eq< r_,    r_   >();
    convert_wrapper::assert_eq< cr_,   cr_  >();

    convert_wrapper::assert_eq< wv_,   r_   >();
    convert_wrapper::assert_eq< wcv_,  cr_  >();

    convert_wrapper::assert_eq< cwv_,  r_   >();
    convert_wrapper::assert_eq< cwcv_, cr_  >();

    convert_wrapper::assert_eq< rwv_,  r_   >();
    convert_wrapper::assert_eq< rwcv_, cr_  >();

    convert_wrapper::assert_eq< crwv_,  r_  >();
    convert_wrapper::assert_eq< crwcv_, cr_ >();
  
    namespace pt = pair_traits;
    pt::convert_wrapper::assert_eq< v_,     v_,     v_  >();
    pt::convert_wrapper::assert_eq< v_,     cv_,    cv_ >();
    pt::convert_wrapper::assert_eq< v_,     r_,     v_  >();
    pt::convert_wrapper::assert_eq< v_,     cr_,    cv_ >();
    pt::convert_wrapper::assert_eq< v_,     wv_,    v_  >();
    pt::convert_wrapper::assert_eq< v_,     wcv_,   cv_ >();
    pt::convert_wrapper::assert_eq< v_,     cwv_,   v_  >();
    pt::convert_wrapper::assert_eq< v_,     cwcv_,  cv_ >();
    pt::convert_wrapper::assert_eq< v_,     rwv_,   v_  >();
    pt::convert_wrapper::assert_eq< v_,     rwcv_,  cv_ >();
    pt::convert_wrapper::assert_eq< v_,     crwv_,  v_  >();
    pt::convert_wrapper::assert_eq< v_,     crwcv_, cv_ >();

    pt::convert_wrapper::assert_eq< cv_,    cv_,    cv_ >();
    pt::convert_wrapper::assert_eq< cv_,    r_,     cv_ >();
    pt::convert_wrapper::assert_eq< cv_,    cr_,    cv_ >();
    pt::convert_wrapper::assert_eq< cv_,    wv_,    cv_ >();
    pt::convert_wrapper::assert_eq< cv_,    wcv_,   cv_ >();
    pt::convert_wrapper::assert_eq< cv_,    cwv_,   cv_ >();
    pt::convert_wrapper::assert_eq< cv_,    cwcv_,  cv_ >();
    pt::convert_wrapper::assert_eq< cv_,    rwv_,   cv_ >();
    pt::convert_wrapper::assert_eq< cv_,    rwcv_,  cv_ >();
    pt::convert_wrapper::assert_eq< cv_,    crwv_,  cv_ >();
    pt::convert_wrapper::assert_eq< cv_,    crwcv_, cv_ >();

    pt::convert_wrapper::assert_eq< r_,     r_,     r_  >();
    pt::convert_wrapper::assert_eq< r_,     cr_,    cr_ >();
    pt::convert_wrapper::assert_eq< r_,     wv_,    r_  >();
    pt::convert_wrapper::assert_eq< r_,     wcv_,   cr_ >();
    pt::convert_wrapper::assert_eq< r_,     cwv_,   r_  >();
    pt::convert_wrapper::assert_eq< r_,     cwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< r_,     rwv_,   r_  >();
    pt::convert_wrapper::assert_eq< r_,     rwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< r_,     crwv_,  r_  >();
    pt::convert_wrapper::assert_eq< r_,     crwcv_, cr_ >();

    pt::convert_wrapper::assert_eq< cr_,    cr_,    cr_ >();
    pt::convert_wrapper::assert_eq< cr_,    wv_,    cr_ >();
    pt::convert_wrapper::assert_eq< cr_,    wcv_,   cr_ >();
    pt::convert_wrapper::assert_eq< cr_,    cwv_,   cr_ >();
    pt::convert_wrapper::assert_eq< cr_,    cwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< cr_,    rwv_,   cr_ >();
    pt::convert_wrapper::assert_eq< cr_,    rwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< cr_,    crwv_,  cr_ >();
    pt::convert_wrapper::assert_eq< cr_,    crwcv_, cr_ >();

    pt::convert_wrapper::assert_eq< wv_,    wv_,    r_  >();
    pt::convert_wrapper::assert_eq< wv_,    wcv_,   cr_ >();
    pt::convert_wrapper::assert_eq< wv_,    cwv_,   r_  >();
    pt::convert_wrapper::assert_eq< wv_,    cwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< wv_,    rwv_,   r_  >();
    pt::convert_wrapper::assert_eq< wv_,    rwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< wv_,    crwv_,  r_  >();
    pt::convert_wrapper::assert_eq< wv_,    crwcv_, cr_ >();

    pt::convert_wrapper::assert_eq< wcv_,   wcv_,   cr_ >();
    pt::convert_wrapper::assert_eq< wcv_,   cwv_,   cr_ >();
    pt::convert_wrapper::assert_eq< wcv_,   cwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< wcv_,   rwv_,   cr_ >();
    pt::convert_wrapper::assert_eq< wcv_,   rwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< wcv_,   crwv_,  cr_ >();
    pt::convert_wrapper::assert_eq< wcv_,   crwcv_, cr_ >();

    pt::convert_wrapper::assert_eq< cwv_,   cwv_,   r_  >();
    pt::convert_wrapper::assert_eq< cwv_,   cwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< cwv_,   rwv_,   r_  >();
    pt::convert_wrapper::assert_eq< cwv_,   rwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< cwv_,   crwv_,  r_  >();
    pt::convert_wrapper::assert_eq< cwv_,   crwcv_, cr_ >();

    pt::convert_wrapper::assert_eq< cwcv_,  cwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< cwcv_,  rwv_,   cr_ >();
    pt::convert_wrapper::assert_eq< cwcv_,  rwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< cwcv_,  crwv_,  cr_ >();
    pt::convert_wrapper::assert_eq< cwcv_,  crwcv_, cr_ >();

    pt::convert_wrapper::assert_eq< rwv_,   rwv_,   r_  >();
    pt::convert_wrapper::assert_eq< rwv_,   rwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< rwv_,   crwv_,  r_  >();
    pt::convert_wrapper::assert_eq< rwv_,   crwcv_, cr_ >();

    pt::convert_wrapper::assert_eq< rwcv_,  rwcv_,  cr_ >();
    pt::convert_wrapper::assert_eq< rwcv_,  crwv_,  cr_ >();
    pt::convert_wrapper::assert_eq< rwcv_,  crwcv_, cr_ >();

    pt::convert_wrapper::assert_eq< crwv_,  crwv_,  r_  >();
    pt::convert_wrapper::assert_eq< crwv_,  crwcv_, cr_ >();

    pt::convert_wrapper::assert_eq< crwv_,  crwcv_, cr_ >();

    namespace prt = pair_range_traits;
    prt::assert_checks<awv_,awv_>();
    prt::assert_checks<awv_,acwv_>();
    prt::assert_checks<awv_,awcv_>();
    prt::assert_checks<awv_,acwcv_>();
    prt::assert_checks<awv_,cawv_>();
    prt::assert_checks<awv_,cacwv_>();
    prt::assert_checks<awv_,cawcv_>();
    prt::assert_checks<awv_,cacwcv_>();

    prt::assert_checks<acwv_,acwv_>();
    prt::assert_checks<acwv_,awcv_>();
    prt::assert_checks<acwv_,acwcv_>();
    prt::assert_checks<acwv_,cawv_>();
    prt::assert_checks<acwv_,cacwv_>();
    prt::assert_checks<acwv_,cawcv_>();
    prt::assert_checks<acwv_,cacwcv_>();

    prt::assert_checks<awcv_,awcv_>();
    prt::assert_checks<awcv_,acwcv_>();
    prt::assert_checks<awcv_,cawv_>();
    prt::assert_checks<awcv_,cacwv_>();
    prt::assert_checks<awcv_,cawcv_>();
    prt::assert_checks<awcv_,cacwcv_>();

    prt::assert_checks<acwcv_,acwcv_>();
    prt::assert_checks<acwcv_,cawv_>();
    prt::assert_checks<acwcv_,cacwv_>();
    prt::assert_checks<acwcv_,cawcv_>();
    prt::assert_checks<acwcv_,cacwcv_>();

    prt::assert_checks<cawv_,cawv_>();
    prt::assert_checks<cawv_,cacwv_>();
    prt::assert_checks<cawv_,cawcv_>();
    prt::assert_checks<cawv_,cacwcv_>();

    prt::assert_checks<cacwv_,cacwv_>();
    prt::assert_checks<cacwv_,cawcv_>();
    prt::assert_checks<cacwv_,cacwcv_>();

    prt::assert_checks<cawcv_,cawcv_>();
    prt::assert_checks<cawcv_,cacwcv_>();

    prt::assert_checks<cacwcv_,cacwcv_>();

}

}// chain_mpl_check
}// detail
}// assign
}// boost

#endif
