//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_RHS_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_RHS_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector/vector0.hpp>

#include <boost/assign/v2/ref/list_tuple.hpp>
#include <boost/assign/v2/put/pipe/modulo_traits.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#include <boost/assign/v2/temporary/variadic_vector.hpp>
#else
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/assign/v2/detail/functor/crtp_unary_and_up.hpp>
#endif

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace put_pipe_aux{

    typedef ::boost::mpl::vector0<> empty_args_list_;

#if! BOOST_ASSIGN_V2_ENABLE_CPP0X
    typedef ref::list_tuple_aux::na_type na_type;
#endif

    template<typename ParList = empty_par_list_, typename ArgsList = empty_args_list_, bool enable_pars = ::boost::mpl::size<ArgsList>::value == 0>
    class rhs;

    template<typename ParList, typename ArgsList, bool enable_pars>
    struct rhs_result
    {

        template<typename V>
        struct next_helper
        {
        	typedef typename ::boost::mpl::push_back<
            	ArgsList, V
            >::type next_args_list_;
        	typedef rhs<ParList,  next_args_list_,  enable_pars> type;
        };

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

        template<typename...Args>
        struct apply : next_helper<
            typename ::boost::mpl::detail::variadic_vector<
                Args...
            >::type
        >
        {};

#else

        template<typename V>
        struct apply : next_helper<V>{};

#endif

    };


    template<typename ParList, typename ArgsList, bool enable_pars>
    class rhs
#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
        :    public functor_aux::crtp_unary_and_up<
                rhs<ParList, ArgsList, enable_pars>,
                rhs_result<ParList, ArgsList, enable_pars>
            >
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
    {
        typedef boost::use_default list_tag_;
        typedef modulo_traits<ParList> modulo_traits_;
        typedef ref::nth_result_of::list_tuple meta_;

        public:

        typedef typename modulo_traits_::size par_list_size;
        typedef typename modulo_traits_::cont_ par_list_cont_type;

        BOOST_STATIC_CONSTANT(
            std::size_t, // size_type?
            args_list_size = ::boost::mpl::size<ArgsList>::value
        ); 
    
        typedef typename ::boost::mpl::apply1<
        	meta_, ArgsList
        >::type args_list_cont_type;

        rhs(){}
        explicit rhs(par_list_cont_type const& a, args_list_cont_type const& b) 
        	: par_list_cont_( a ), args_list_cont_( b ){}

        // operator%

        template<typename P>
        struct modulo_result
        {
            typedef typename modulo_traits_::template 
            	next_par_list<P>::type par_list_;
            typedef rhs<par_list_, ArgsList> type;
        };

        template<typename P>
        typename boost::lazy_enable_if_c<
            enable_pars,
            modulo_result<P>
        >::type
        operator%(P const& p)const
        {
            typedef typename modulo_result<P>::type result_;
            return result_(
                this->par_list_cont()( p ),
                this->args_list_cont()
            );
        }

        // operator()

        template<
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
            typename...Args
#else
            typename VArgs
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
        >
        struct result : rhs_result<
            ParList,
            ArgsList,
            enable_pars
        >::template apply<
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
            Args...
#else
            VArgs
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
        >{};

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

        public:

        template<typename ...Args>
        typename result<Args...>::type
        operator()(Args&&...args )const
        {
            typedef typename result<Args...>::type result_;
            return result_(
                this->par_list_cont(),
                this->args_list_cont()(
                    std::forward<Args>( args )...
                )
            );
        }

#else
        protected:

        typedef functor_aux::crtp_unary_and_up<
            rhs,
            rhs_result<ParList, ArgsList, enable_pars>
        > super_t;

        typedef ::boost::mpl::vector0<> v0_;

        public:

        using super_t::operator();

        typename result<v0_>::type
        operator()()const
        {
            typedef typename result<v0_>::type result_;
            return result_(
                this->par_list_cont(),
                this->args_list_cont()()
            );
        }

#define BOOST_ASSIGN_V2_MACRO1( z, n, data )\
 ( BOOST_PP_CAT(_,n) )\
/**/
#define BOOST_ASSIGN_V2_MACRO2(z, N1, data)\
    template<BOOST_PP_ENUM_PARAMS(N1, typename U)>\
    typename result<\
        ::boost::mpl::vector<\
            BOOST_PP_ENUM_PARAMS(N1, U)\
        >\
    >::type\
    impl( BOOST_PP_ENUM_BINARY_PARAMS(N1, U, &_) )const{\
        typedef ::boost::mpl::vector<\
            BOOST_PP_ENUM_PARAMS(N1, U)\
        > v_;\
        typedef typename result<v_>::type result_;\
        return result_(\
            this->par_list_cont(),\
            this->args_list_cont()( BOOST_PP_ENUM_PARAMS(N1, _) )\
        );\
    }\
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    BOOST_ASSIGN_V2_MACRO2,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO1
#undef BOOST_ASSIGN_V2_MACRO2

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

        par_list_cont_type const& par_list_cont()const
        {
            return this->par_list_cont_;
        }
        args_list_cont_type const& args_list_cont()const
        {
            return this->args_list_cont_;
        }

        protected:

        par_list_cont_type par_list_cont_;
        args_list_cont_type args_list_cont_;

    };

    // For testing purposes
    template<int i, int j, typename T, typename P, typename U>
    void check_rhs(T const& rhs, P const& pred, U const& u)
    {
        #if BOOST_ASSIGN_V2_ENABLE_CPP0X
        using namespace boost::assign::v2::ref; // tuple (cpp0x)
        #else
        using namespace boost; // tuple<> (cpp03)
        #endif
        
        //std::cout << "(i,j)->" << get<j>(
        //        rhs.args_list().get( boost::mpl::int_<i>() )
        //    ) << std::endl;
        
        pred(
            get<j>(
                rhs.args_list_cont().get( boost::mpl::int_<i>() )
            ),
            u
        );
    }


}// put_pipe_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_CONTAINER_ER_2010_HPP
