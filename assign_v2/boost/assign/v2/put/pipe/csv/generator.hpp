//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_CSV_GENERATOR_HPP_ER_2010
#define BOOST_ASSIGN_V2_PUT_PIPE_CSV_GENERATOR_HPP_ER_2010
#include <boost/mpl/apply.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector/vector0.hpp>

#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

#include <boost/assign/v2/put/pipe/csv/rhs.hpp>
#include <boost/assign/v2/put/pipe/modulo/traits.hpp>

#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X

// do nothing

#else

#include <boost/mpl/aux_/na.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#endif

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace put_pipe_aux{

    template<typename Pars = ::boost::mpl::vector0<> >
    class csv_generator
    {

        typedef ::boost::mpl::na na_;
        typedef modulo_traits<Pars> modulo_traits_;

        public:

        typedef typename modulo_traits_::size pars_size;
        typedef typename modulo_traits_::cont_ pars_cont_type;

        csv_generator(){}
        explicit csv_generator(pars_cont_type const& p)
            : pars_cont( p ){}

        template<typename T>
        struct modulo_result
        {
            typedef typename modulo_traits_:: template new_pars<T>::type new_;
            typedef csv_generator<new_> type;
        };

        template<typename T>
        typename modulo_result<T>::type
        operator%(T const& t)const
        {
            typedef typename modulo_result<T>::type result_;
            return result_( this->pars_cont( t ) );
        }

        template<std::size_t N, typename U = na_>
        struct result{
            typedef put_pipe_aux::csv_rhs<Pars, N, U> type;
        };

        typename result<0>::type
        operator()()const
        {
            typedef typename result<0>::type result_;
            return result_(
                *this,
                ref::csv_array<na_>( _nil )
            );
        }

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

    protected:
    template<typename T, typename...Args>
    typename result<sizeof...(Args)+1, T>::type
    impl(T& t, Args&...args)const
    {
        typedef typename result<sizeof...(Args)+1, T>::type result_;
        namespace ns = ref::assign_copy;
        return result_(
            this->pars_cont,
            ref::csv_array( t, args... )
        );
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

#define BOOST_ASSIGN_V2_MACRO1(N, U)\
    return result_( \
        this->pars_cont, \
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

#endif

        protected:
        pars_cont_type pars_cont;

    };

}// put_pipe_aux

    put_pipe_aux::csv_generator<> const _csv_put
        = put_pipe_aux::csv_generator<>();

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_CSV_GENERATOR_HPP_ER_2010
