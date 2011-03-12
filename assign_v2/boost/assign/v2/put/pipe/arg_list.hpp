//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_ARG_LIST_HPP_ER_2010
#define BOOST_ASSIGN_V2_PUT_PIPE_ARG_LIST_HPP_ER_2010
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/ref/array/csv_array.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/put/pipe/modulo_traits.hpp>
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
namespace put_pipe_aux{

    typedef ref::array_aux::size_type arg_list_size_type;

    template<typename Pars, arg_list_size_type N, typename U>
    struct arg_list
    {
        typedef boost::use_default tag2_;
        typedef modulo_traits<Pars> modulo_traits_;
        typedef typename modulo_traits_::cont_ par_list_cont_type;
        typedef typename v2::ref::nth_result_of::csv_array<
            N,
            U
        >::type arg_list_cont_type; //notice it's arg, not args

        arg_list(){}
        arg_list(par_list_cont_type const& a, arg_list_cont_type const& b)
            : par_list_cont_( a ), arg_list_cont_( b ){}

        par_list_cont_type const& par_list_cont()const 
        {
            return this->par_list_cont_;
        }

        arg_list_cont_type& arg_list_cont() // TODO needed (non-const)?
        {
            return this->arg_list_cont_;
        }

        arg_list_cont_type const& arg_list_cont() const
        {
            return this->arg_list_cont_;
        }

        protected:
        par_list_cont_type par_list_cont_;
        arg_list_cont_type arg_list_cont_;

    };

    template<typename ParList = ::boost::mpl::vector0<> >
    class arg_list_generator
    {

        typedef ::boost::mpl::na na_;
        typedef modulo_traits<ParList> modulo_traits_;

        public:

        typedef typename modulo_traits_::size par_list_size;
        typedef typename modulo_traits_::cont_ par_list_cont_type;

        arg_list_generator(){}
        explicit arg_list_generator(par_list_cont_type const& p)
            : par_list_cont_( p ){}

        template<typename P>
        struct modulo_result
        {
            typedef typename modulo_traits_:: template next_par_list<
                P
            >::type par_list_;
            typedef arg_list_generator<par_list_> type;
        };

        template<typename P>
        typename modulo_result<P>::type
        operator%(P const& p)const
        {
            typedef typename modulo_result<P>::type result_;
            return result_( this->par_list_cont()( p ) );
        }

        template<std::size_t N, typename U = na_> // size?
        struct result{
            typedef put_pipe_aux::arg_list<ParList, N, U> type;
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
            this->par_list_cont(),
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
        this->par_list_cont(), \
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

        par_list_cont_type const& par_list_cont()const
        {
            return this->par_list_cont_;
        }

        protected:
        par_list_cont_type par_list_cont_;

    };

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_ARG_LIST_HPP_ER_2010

