//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_COMPOSE_CSV_COMPOSER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_COMPOSE_CSV_COMPOSER_ER_2010_HPP
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/assign/v2/ref/anon/csv/assign_copy.hpp>
#include <boost/assign/v2/ref/fusion/nth_result_of.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/put/compose/csv/held.hpp>

namespace boost{
	struct use_default;
namespace assign{ 
namespace v2{
namespace put_compose_csv_aux{

    template<typename Pars = boost::mpl::vector0<> >
	class composer
    {

    	typedef boost::use_default tag2_;
    	typedef v2::ref::assign_copy::nth_result_of::fusion<tag2_> meta1_;

		public:
        
		BOOST_STATIC_CONSTANT(
        	std::size_t, 
            static_parameters_size = boost::mpl::size<Pars>::value
        );

        typedef typename boost::mpl::apply1<meta1_, Pars>::type pars_cont_type;

		composer(){}
		explicit composer(pars_cont_type const& p) : pars_cont( p ){}

        template<typename T>
        struct modulo_result
        {
        	typedef typename boost::mpl::push_back<
    			Pars,
                T const
            >::type new_;
            typedef composer<new_> type;
        };
        
        template<typename T>
        typename modulo_result<T>::type
        operator%(T const& t)const
        {
        	typedef typename modulo_result<T>::type result_;
            return result_( this->pars_cont( t ) );
        }
		
    	template<std::size_t N, typename U = boost::mpl::na>
        struct result{
        	typedef put_compose_csv_aux::held<Pars, N, U> type;
        };

		typename result<0>::type
    	operator()()const 
	    { 
			typedef typename result<0>::type result_;
    		namespace ns = ref::assign_copy; 
    		return result_( 
        		*this, 
        		ns::csv_anon( _nil ) 
    		);
    	} 

#define MACRO1(N, U)\
    namespace ns = ref::assign_copy;\
    return result_( \
        this->pars_cont, \
        ns::csv_anon<U>( BOOST_PP_ENUM_PARAMS(N, _) ) \
    );\
/**/

#define MACRO2(z, N, data)\
	template<typename T>\
	typename result<N, T>::type\
    operator()( BOOST_PP_ENUM_PARAMS(N, T &_) )const \
    { \
    	typedef typename result<N, T>::type result_;\
        MACRO1( N, T )\
    } \
	template<typename T>\
	typename result<N, T const>::type\
    operator()( BOOST_PP_ENUM_PARAMS(N, T const &_) )const \
    { \
    	typedef typename result<N, T const>::type result_;\
        MACRO1( N, T const )\
    } \
/**/

BOOST_PP_REPEAT_FROM_TO(
	1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_CSV_ARITY),
    MACRO2,
    ~
)
#undef MACRO1
#undef MACRO2

		protected:
		pars_cont_type pars_cont;
        
    };

}// put_compose_csv_aux
namespace adaptor{
namespace{
	put_compose_csv_aux::composer<> const _csv_put 
    	= put_compose_csv_aux::composer<>();
}
}// adaptor
}// v2
}// assign
}// boost

#endif
