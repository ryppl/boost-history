//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_CRTP_ER_2011_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_CRTP_ER_2011_HPP
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/assign/v2/interpreter/modifier.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/pp/forward.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/support/traits/container.hpp>
#include <boost/assign/v2/support/traits/type/param.hpp>
#include <boost/concept_check.hpp>
#include <boost/function.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/reference.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/support/mpl/variadic_args_to_indices.hpp>
#include <utility>
#else
#include <boost/assign/v2/support/config/limit_functor_arity.hpp>
#include <boost/assign/v2/support/functor/crtp_unary_and_up.hpp>
#include <boost/mpl/int.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

#include <boost/tuple/tuple.hpp> // Needed by for_each...

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

    template<typename F>
    struct data_gen_holder{

        typedef F fun_type;

        data_gen_holder(){}
        data_gen_holder(fun_type const& f) : fun( f ){}

        fun_type fun;

    };

    template<typename Tag>
    struct modifier_holder{

        typedef Tag modifier_tag;
        typedef interpreter_aux::interpreter_modifier<Tag> modifier_type;

        modifier_holder(){}
        modifier_holder(modifier_type const& m) : modifier( m ){}

        modifier_type modifier;

    };

}// interpreter_aux
//[syntax_interpreter_crtp
namespace interpreter_aux{
/*<-*/
    template<typename C, typename D>
    struct ConceptDerivedInterpreter1{

        BOOST_CONCEPT_USAGE(ConceptDerivedInterpreter1)
        {
            D derived( cont );
            C& ref = derived.container();
        }

        private:
        static C& cont;

    };

    template<typename C, typename F, typename MTag, typename D>
    struct ConceptDerivedInterpreter2 : ConceptDerivedInterpreter1<C, D>{

        typedef interpreter_aux::interpreter_modifier<MTag> modifier_;

        BOOST_CONCEPT_USAGE(ConceptDerivedInterpreter2)
        {
            D derived( cont, f, m );
        }

        private:
        static C& cont;
        static F const &f;
        static modifier_ m;

    };
/*->*/

    template<
        typename D              // Derived
        , typename C            // Container
        , typename F            // Data generator
        , typename MTag         // Modifier tag
        , typename DTag         // Data tag
    >
    class interpreter_crtp
//<-
        : public data_gen_holder<F>
        , public modifier_holder<MTag>
#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
        , public functor_aux::crtp_unary_and_up<
            interpreter_crtp<D, C, F, MTag, DTag>,
            ::boost::mpl::always< D const& >
        >
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
    {
//<-
        public:
        D & derived(){ return static_cast<D&>(*this); }
        D const& derived()const{ return static_cast<D const&>(*this); }

        protected:

        typedef data_gen_holder<F> data_gen_holder_;
        typedef modifier_holder<MTag> modifier_holder_;
//->

        public:

        typedef /*<-*/ typename modifier_holder_::modifier_type
            BOOST_ASSIGN_V2_IGNORE(/*->*/ interpreter_modifier<Tag> /*<-*/)/*->*/
        modifier_type;

        interpreter_crtp(){}
        explicit interpreter_crtp( F const& f )/*<-*/
            : data_gen_holder_( f )
        {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        interpreter_crtp( F const& f, modifier_type const& m )/*<-*/
            : data_gen_holder_( f ), modifier_holder_( m )
        {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        typedef D const& result_type;
//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

        template<typename...Args>
        result_type operator()( Args&&...args )const/*<-*/
        {
            return  this->modify(
                /*Instance of F*/ this->fun( std::forward<Args>(args)... )
            );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        template<typename R>
        result_type for_each( R&& range )const/*<-*/
        {
            return this->for_each_impl( std::forward<R>( range ) );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        template<
            int I,
            typename R  // Range of tuples, each of size I
        >
        result_type for_each( R&& range )const/*<-*/
        {
            return this->for_each_impl<I>( std::forward<R>( range ) );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
#else
        protected:
        typedef functor_aux::crtp_unary_and_up<
            interpreter_crtp,
            ::boost::mpl::always<result_type>
        > super_t;

        public:
        using super_t::operator();

        result_type operator()()const
        {
            return this->modify( this->fun() );
        }

        template<typename R>
        result_type
        for_each( R& range )const/*<-*/
        {
            return this->for_each_impl( range );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        template<typename R>
        result_type
        for_each( R const& range )const/*<-*/
        {
            return this->for_each_impl( range );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        template<int I, typename R>
        result_type
        for_each( R& range )const/*<-*/
        {
            return this->for_each_impl<I>( range );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        template<int I, typename R>
        result_type
        for_each( R const& range )const/*<-*/
        {
            return this->for_each_impl<I>( range );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/


#define BOOST_ASSIGN_V2_MACRO(z, N, data) \
    template<BOOST_PP_ENUM_PARAMS(N, typename T)> \
    result_type \
    impl( BOOST_PP_ENUM_BINARY_PARAMS(N, T, & _) )const \
    { \
        return this->modify( this->fun(BOOST_PP_ENUM_PARAMS(N,_)) ); \
    } \
/**/
BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

        // must be mutable
        C& container()const{ return this->derived().container(); }

        protected:

        template<typename T>
        static void check_modifier()
        {
            typedef ConceptModifier<MTag, DTag, C,
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
                T&&
#else
                T&
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
            > concept_;
            BOOST_CONCEPT_ASSERT(( concept_ ));
        }

#if BOOST_ASSIGN_V2_ENABLE_CPP0X

        template<typename T>
        result_type modify(T&& t)const
        {
            check_modifier<T>();

            this->modifier.impl(
                this->derived().container(),
                std::forward<T>( t ),
                DTag()
            );
            return this->derived();
        }

        // for_each

        template<typename Indices>
        struct unpacker
        {

            unpacker(D const& d)
                : derived( d )
            {}

            template<typename T>
            void operator()(T const& tuple)const
            {
                this->impl(
                    tuple,
                    Indices()
                );
            }

            private:

            template<typename T, int...Values>
            void impl(
                T const& tuple,
                indices<int, Values...>
            )const
            {
                this->derived(
                    get<Values>( tuple )...
                );
            }
            D const& derived;
        };

        template<int I, typename R>
        result_type
        for_each_impl( BOOST_ASSIGN_V2_FORWARD_PARAM(R, range) )const
        {
            typedef typename head_indices<int, I>::type indices_;
            boost::for_each(
                BOOST_ASSIGN_V2_FORWARD_ARG(R, range),
                unpacker<indices_>( static_cast<D const&>(*this ) )
            );
            return this->derived();
        }

#else
        template<typename T>
        result_type modify(T& t)const
        {
            check_modifier<T>();
            this->modifier.impl( this->derived().container(), t, DTag() );
            return this->derived();
        }

        template<typename T>
        result_type modify(T const& t)const
        {
            check_modifier<T const>();
            this->modifier.impl( this->derived().container(), t, DTag() );
            return this->derived();
        }

        template<int I>
        struct unpacker
        {

            unpacker(D const& d)
                : derived( d )
            {}

            template<typename T>
            void operator()(T const& tuple)const
            {
                typedef ::boost::mpl::int_<I> int_;
                this->impl( int_(), tuple );
            }

            private:

#define BOOST_ASSIGN_V2_get(z, i, data)\
    get<i>( tuple )\
/**/
#define BOOST_ASSIGN_V2_MACRO(z, N, data)\
            template<typename T>\
            void impl(::boost::mpl::int_<N>, T const& tuple)const\
            {\
                this->derived(\
                    BOOST_PP_ENUM(N, BOOST_ASSIGN_V2_get, ~)\
                );\
            }\
/**/
BOOST_PP_REPEAT(
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_FUNCTOR_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    ~
)
#undef BOOST_ASSIGN_V2_GET
#undef BOOST_ASSIGN_V2_MACRO

            D const& derived;

        };

        template<int I, typename R>
        result_type
        for_each_impl( BOOST_ASSIGN_V2_FORWARD_PARAM(R, range) )const
        {
            boost::for_each(
                BOOST_ASSIGN_V2_FORWARD_ARG(R, range),
                unpacker<I>( static_cast<D const&>(*this ) )
            );
            return this->derived();
        }

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

        template<typename R>
        result_type
        for_each_impl( BOOST_ASSIGN_V2_FORWARD_PARAM(R, range) )const
        {
            typedef typename boost::range_reference<
                typename boost::remove_reference<
                    typename type_traits::param<R>::type
                >::type
            >::type t_;
            boost::function<result_type ( t_ )> f = boost::cref( *this );
            boost::for_each( BOOST_ASSIGN_V2_FORWARD_ARG(R, range), f );
            return this->derived();
        }

//->
    };

}// interpreter_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_CRTP_ER_2011_HPP
