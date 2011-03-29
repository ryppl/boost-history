//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PIPE_CSV_PUT_HPP_ER_2010
#define BOOST_ASSIGN_V2_PIPE_CSV_PUT_HPP_ER_2010
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/put.hpp>
#include <boost/assign/v2/option/list.hpp>
#include <boost/assign/v2/ref/array/as_arg_list.hpp>
#include <boost/assign/v2/ref/array/csv_array.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/size.hpp>
#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition.hpp>
#endif


namespace boost{
    struct use_default;
namespace assign{
namespace v2{
//[syntax_pipe_csv_put
namespace interpreter_aux{

    typedef ref::array_aux::size_type arg_list_size_type;

    template<typename H, typename Tail, arg_list_size_type N, typename U>
    struct arg_list : list_option<H, Tail>/*<-*/
    {
        typedef list_option<H, Tail> list_option_;
        typedef typename v2::ref::nth_result_of::csv_array<
            N, U
        >::type arg_list_cont_type; //notice it's arg, not args

        arg_list(){}
        arg_list(list_option_ const& l, arg_list_cont_type const& a)
            : list_option_( l ), 
            arg_list_cont_( a )
        {}

        arg_list_cont_type const& arg_list_cont() const
        {
            return this->arg_list_cont_;
        }

        protected:
        arg_list_cont_type arg_list_cont_;

    }/*->*/;

    template<
    	typename Head = typename empty_list_option::head_type, 
        typename Tail = typename empty_list_option::tail_type
    >
    class arg_list_generator : public list_option<Head, Tail>/*<-*/
    {

        typedef ::boost::mpl::na na_;
		typedef list_option<Head, Tail> list_option_;

        public:

        arg_list_generator(){}
        explicit arg_list_generator(Tail const& t, Head const& h)
            : list_option_( t, h )
        {}

    	template<typename Option>
        struct modulo_result
        {
        	typedef arg_list_generator<Option, arg_list_generator> type;
        };
    
    	template<typename Option>
        typename modulo_result<Option>::type
    	operator%(Option option)const
        {
        	typedef typename modulo_result<Option>::type result_;
            return result_( *this, option );
        }

        template<arg_list_size_type N, typename U = na_> // size?
        struct result{
            typedef interpreter_aux::arg_list<Head, Tail, N, U> type;
        };
 
#if BOOST_ASSIGN_V2_ENABLE_CPP0X

	    protected:
    	template<typename T, typename...Args>
    	typename result<sizeof...(Args) + 1, T>::type
    	impl(T& t, Args&...args)const
    	{
        	typedef typename result<sizeof...(Args)+1, T>::type result_;
        	namespace ns = ref::assign_copy;
        	return result_(*this, ref::csv_array( t, args... ) );
    	}

	    public:

    	template<typename T, typename...Args>
    	typename boost::lazy_disable_if<
        	v2::type_traits::or_const<T, Args...>,
        	result<sizeof...(Args)+1, T>
    	>::type
    	operator()(T& t, Args&...args)const
    	{
        	return this->impl(t, args...);
    	}

    	template<typename T, typename...Args>
    	typename result<sizeof...(Args)+1, T const>::type
    	operator()(T const& t, Args const&...args)const
    	{
        	return this->impl(t, args...);
    	}

#else

        typename result<0>::type
        operator()()const
        {
            typedef typename result<0>::type result_;
            return result_(
                *this,
                ref::csv_array<na_>( _nil )
            );
        }

#define BOOST_ASSIGN_V2_MACRO1(N, U)\
    	return result_( \
        	*this, \
        	ref::csv_array<U>( BOOST_PP_ENUM_PARAMS(N, _) ) \
    	);\
/**/
#define BOOST_ASSIGN_V2_MACRO2(z, N, data)\
    	template<typename T>\
    	typename result<N, T>::type\
    	operator()( BOOST_PP_ENUM_PARAMS(N, T &_) )const \
    	{ \
        	typedef typename result<N, T>::type result_;\
        	BOOST_ASSIGN_V2_MACRO1( N, T )\
    	} \
    	template<typename T>\
    	typename result<N, T const>::type\
    	operator()( BOOST_PP_ENUM_PARAMS(N, T const &_) )const \
    	{ \
        	typedef typename result<N, T const>::type result_;\
        	BOOST_ASSIGN_V2_MACRO1( N, T const )\
    	} \
/**/

BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

    }/*->*/;

// TODO deal with maps

    template<
    	typename C, typename H, typename T, 
        arg_list_size_type N, typename U
    >
    C& operator|(
    	C& cont, 
        interpreter_aux::arg_list<H, T, N, U> const& arg_list
    )/*<-*/
    {
    	typedef typename v2::result_of::put<
        	C
        	, functor_aux::value<
        		typename container_aux::value<C>::type
        	>
        	, typename interpreter_aux::deduce_modifier_tag<C>::type
        	, typename interpreter_aux::deduce_data_tag<C>::type
		>::type put_;
        v2::ref::as_arg_list(
            arg_list.apply(  put_( cont ) ),
            arg_list.arg_list_cont()
        );
        return cont;

    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// interpreter_aux 
//<-
namespace{
//->

    interpreter_aux::arg_list_generator<> const _csv_put/*<-*/
        = interpreter_aux::arg_list_generator<>()/*->*/;

//<-
}
//->
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PIPE_CSV_PUT_HPP_ER_2010

