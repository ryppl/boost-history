//////////////////////////////////////////////////////////////////////////////
// repeat.hpp                                                               //
//                                                                          //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_DETAIL_REPEAT_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_DETAIL_REPEAT_ER_2010_HPP
#include <boost/mpl/int.hpp>
#include <boost/mpl/divides.hpp>
#include <boost/mpl/modulus.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/assign/auto_size/detail/csv.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{
namespace repeat_aux{

    template<int N,typename T,
        template<typename> class R = boost::assign::detail::auto_size::ref_copy>
    struct result{
        typedef auto_size::tag::array pol_;
        typedef typename auto_size::csv_policy<pol_>::template  
            apply<T,N,R>::type type;
    }; 

    template<int N,typename T, template<typename> class R> 
    struct caller{};

#ifndef BOOST_ASSIGN_AS_REPEAT_MAX
 #define BOOST_ASSIGN_AS_REPEAT_MAX 20
#endif

#define BOOST_ASSING_AS_REPEAT_tmp(z,n,data) ( t ) 
#define BOOST_ASSIGN_AS_REPEAT_caller(z,n,data)                                          \
    template<typename T,template<typename> class R>                                      \
    struct caller<n,T,R>                                                                 \
    {                                                                                    \
        typedef typename auto_size::repeat_aux::result<n,T,R> result_;                   \
        typedef typename result_::type type;                                             \
        static type call(T& t){                                                          \
            return boost::assign::detail::auto_size::make_first_expr_no_policy<          \
                R,T> BOOST_PP_REPEAT(BOOST_PP_ADD(n,1), BOOST_ASSING_AS_REPEAT_tmp, ~ ); \
        }                                                                                \
    };                                                                                   \
/**/

BOOST_PP_REPEAT(BOOST_PP_SUB(BOOST_ASSIGN_AS_REPEAT_MAX,1),BOOST_ASSIGN_AS_REPEAT_caller,~)

#undef BOOST_ASSING_CSV_LIST_tmp
#undef BOOST_ASSING_CSV_LIST_iter

}// repeat_aux
}// auto_size
}// detail

    template<unsigned K,typename T>
    typename detail::auto_size::repeat_aux::result<K,T>::type
    repeat(T& t){
        namespace ns = detail::auto_size;
        typedef ns::repeat_aux::caller<K,T,
            boost::assign::detail::auto_size::ref_copy> caller_;
        return caller_::call(t);
    }

    template<unsigned K,typename T>
    typename detail::auto_size::repeat_aux::result<K,const T>::type
    crepeat(const T& t){
        namespace ns = detail::auto_size;
        typedef ns::repeat_aux::caller<K,const T,
            boost::assign::detail::auto_size::ref_copy> caller_;
        return caller_::call( t );
    }


}// assign
}// boost

#endif




