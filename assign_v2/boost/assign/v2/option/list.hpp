//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_OPTION_LIST_ER_2011_HPP
#define BOOST_ASSIGN_V2_OPTION_LIST_ER_2011_HPP
#include <boost/assign/v2/support/keyword.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/or.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

    template<typename Lhs, typename O>
    struct modulo_result
    {
        static Lhs lhs;
        static O option;

        #ifdef BOOST_MSVC
        BOOST_TYPEOF_NESTED_TYPEDEF_TPL(nested, lhs % option )
        typedef typename nested::type type;
        #else
        typedef BOOST_TYPEOF_TPL( lhs % option ) type;
        #endif
    };

    struct ignore_option{};

    template<typename Head>
    struct list_option_exit
        : boost::is_same<Head, kwd_nil_>
    {};

namespace result_of{

    template<
        typename Head,
        typename Tail,
        typename Lhs,
        bool exit = list_option_exit<Head>::value
    >
    struct apply_list_option
    {
        typedef typename apply_list_option<
            typename Tail::head_type, typename Tail::tail_type, Lhs
        >::type lhs_;

        typedef typename modulo_result<lhs_, Head>::type type;
    };

    template<
        typename Head,
        typename Tail,
        typename Lhs
    >
    struct apply_list_option<Head, Tail, Lhs, true>
    {
        typedef Lhs const& type;
    };

}// result_of

    template<
        typename Head = kwd_nil_,
        typename Tail = kwd_nil_,
        bool exit = list_option_exit<Head>::value
    >
    struct list_option;

    template<typename Head, typename Tail, bool exit>
    struct list_option_inherit
        : Tail
    {
        list_option_inherit(){}
        list_option_inherit( Tail tail )
            : Tail( tail )
        {}
    };

    template<typename Head, typename Tail, bool exit>
    struct list_option
        : list_option_inherit<Head, Tail, exit>
    {

        typedef list_option_inherit<Head, Tail, exit> super_t;
        typedef Head head_type;
        typedef Tail tail_type;

        list_option(){ /*exit = true*/ }
        list_option(Tail tail, Head h)
            : super_t( tail ), head_( h )
        {}

        template<typename O>
        struct result
        {
            typedef list_option<O, list_option> type;
        };

        template<typename O>
        typename result<O>::type
        operator%(O option)const
        {
            typedef typename result<O>::type result_;
            return result_( *this, option );
        }

        template<typename Lhs>
        typename boost::lazy_disable_if_c<
            exit,
            result_of::apply_list_option<Head, Tail, Lhs>
        >::type
        apply(Lhs const& lhs)const
        {
            return static_cast<
                Tail const&
            >( *this ).apply( lhs ) % this->head_;
        }

        template<typename Lhs>
        typename boost::lazy_enable_if_c<
            exit,
            result_of::apply_list_option<Head, Tail, Lhs>
        >::type
        apply(Lhs const& lhs)const{ return lhs; }

        private:
        Head head_;

    };

    typedef list_option<> empty_list_option;

    template<typename O>
    struct is_list_option
        : boost::is_base_of< empty_list_option, O>
    {};

    template<
        typename O    // O&&, O&
    >
    struct is_list_option_cpp0x : is_list_option<
        typename boost::remove_cv<
            typename boost::remove_reference<O>::type
        >::type
    >{};

    template<
        typename D, typename C, typename F, typename MTag, typename DTag,
        typename H, typename T
    >
    typename result_of::apply_list_option<H, T, D>::type
    operator%(
        interpreter_crtp<D, C, F, MTag, DTag> const& lhs,
        list_option<H, T> const& list
    )
    {
        return list.apply( static_cast<D const&>( lhs ) );
    }

namespace result_of{

    template<typename L, typename O1>
    struct list_option_modulo
        : L::template result<O1>
    {};


}// result_of
}// interpreter_aux

    typedef interpreter_aux::empty_list_option empty_list_option_;

namespace{

    const empty_list_option_ _option = empty_list_option_();

}
namespace interpreter_aux{
namespace result_of{

    template<typename O1, typename O2>
    struct option_modulo
        : result_of::list_option_modulo<
            typename result_of::list_option_modulo<
                empty_list_option,
                O1
            >::type,
            O2
        >
    {};

}// result_of

    template<typename O> struct option_crtp{};

    template<typename O>
    struct is_option_crtp/*<-*/
        : boost::is_base_of< option_crtp<O>, O>
    {}/*->*/;

    template<typename O1, typename O2>
    typename result_of::option_modulo<O1, O2>::type
    operator%(option_crtp<O1> const option1, O2 const& option2)
    {
        O1 const& ref = static_cast<O1 const&>( option1 );
        return _option % ref % option2;
    }

    template<typename O>
    struct is_option/*<-*/
        : ::boost::mpl::or_<
            is_list_option<O>,
            is_option_crtp<O>
        >
    {};

}// interpreter_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_OPTION_LIST_ER_2011_HPP
