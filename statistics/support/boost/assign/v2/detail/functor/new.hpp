//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_FUNCTOR_NEW_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_FUNCTOR_NEW_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/mpl/always.hpp>
#include <boost/range/reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/assign/v2/detail/traits/container/value.hpp>
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/detail/functor/crtp_unary_and_up.hpp>
#endif

namespace boost{
namespace assign{
namespace v2{
namespace functor_aux{

    template<typename T>
    class new_
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
// do nothing
#else
    : public functor_aux::crtp_unary_and_up<
    	functor_aux::new_<T>,
        ::boost::mpl::always<T*>
    >
#endif
    {
    	typedef T* ptr_;
        typedef functor_aux::new_<T> this_;

    	public:

        typedef ptr_ result_type;

        new_(){}

        result_type operator()()const{ return new T(); }

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
        template<typename... Args>
        result_type operator()(Args&&...args)const
        {
            return new T(std::forward<Args>(args)...);
        }
#else
        protected:
        typedef ::boost::mpl::always< ptr_ > meta_result_;
        typedef functor_aux::crtp_unary_and_up<this_, meta_result_> super_;
        public:

        using super_::operator();

#define MACRO(z, N, data) \
    template<BOOST_PP_ENUM_PARAMS(N, typename T)> \
    result_type impl( BOOST_PP_ENUM_BINARY_PARAMS(N, T, &_) )const{ \
        return new T( BOOST_PP_ENUM_PARAMS(N, _) ); \
    } \
/**/
BOOST_PP_REPEAT_FROM_TO(
	1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    MACRO,
    ~
)
#undef MACRO
#endif

	};

    template<typename V>
    struct deduce_new_
    {
    	typedef typename container_traits::value<V>::type value_;
        typedef functor_aux::new_<value_> type;
        static type call(){ return functor_aux::new_<value_>(); }
    };

}// functor_aux
namespace result_of{

    template<typename T>
    struct new_
    {
        typedef functor_aux::new_<T> type;
    };

}// result_of

    template<typename T>
    typename result_of::new_<T>::type
    new_()
    {
    	typedef typename result_of::new_<T>::type result_;
    	return result_();
    }

}// v2
}// assign
}// boost

#endif
