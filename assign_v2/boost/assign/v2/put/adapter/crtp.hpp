//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_ADAPTER_CRTP_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_ADAPTER_CRTP_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/pp/forward.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/detail/traits/container/is_ptr_container.hpp>
#include <boost/assign/v2/put/adapter/as_arg_list.hpp>
#include <boost/assign/v2/put/adapter/fwd.hpp>
#include <boost/assign/v2/put/adapter/modifier.hpp>
#include <boost/concept_check.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#else
#include <boost/assign/v2/detail/config/limit_arity.hpp>
#include <boost/assign/v2/detail/config/limit_csv_arity.hpp>
#include <boost/assign/v2/detail/functor/crtp_unary_and_up.hpp>
#include <boost/preprocessor/arithmetic.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition.hpp>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

    template<typename F>
    struct fun_holder{

        typedef F fun_type;

        fun_holder(){}
        fun_holder(fun_type const& f) : fun( f ){}

        fun_type fun;

    };

    template<typename Tag>
    struct modifier_holder{

        typedef Tag modifier_tag;
        typedef put_aux::adapter_modifier<Tag> modifier_type;

        modifier_holder(){}
        modifier_holder(modifier_type const& m) : modifier( m ){}

        modifier_type modifier;

    };

    template<typename D>
    struct wrapper
    {

        wrapper(D const& d):d_( d ){}
            
        operator D const&()const{ return this->d_; }
                    
        typedef wrapper const& result_type;            
        
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
        template<typename T>
        result_type operator()( T&& t )const
        {
            this->d_(
                this->fun( std::forward<Args>(args)... )
            );
            return (*this);
        }
#else
        
        template<typename T>
        result_type operator()(T& t)const
        {
            this->d_( t ); return (*this);
        }
        
        template<typename T>
        result_type operator()(T const & t)const
        {
            this->d_( t ); return (*this);
        }
#endif
                 
        private:
        D const& d_;
        
    };
}//put_aux
//[syntax_put_adapter_crtp
namespace put_aux{

    template<typename C, typename D>
    struct ConceptAdapter1{

        BOOST_CONCEPT_USAGE(ConceptAdapter1)
        {
            D derived( cont );
            C& ref = derived.container();
        }

        private:
        static C& cont;

    };

    template<typename C, typename F, typename Tag, typename D>
    struct ConceptAdapter2 : ConceptAdapter1<C, D>{

        typedef put_aux::adapter_modifier<Tag> modifier_;

        BOOST_CONCEPT_USAGE(ConceptAdapter2)
        {
            D derived( cont, f, m );
        }

        private:
        static C& cont;
        static F const &f;
        static modifier_ m;

    };

    template<typename /*<<Container>>*/C, typename /*<<Functor>>*/F, typename /*<<Modifier tag>>*/Tag, typename /*<<Derived type>>*/D>
    class adapter_crtp 
//<-
        : public fun_holder<F>
        , public modifier_holder<Tag>
#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
        , public functor_aux::crtp_unary_and_up<
            adapter_crtp<C, F, Tag, D>,
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

        typedef fun_holder<F> fun_holder_;
        typedef modifier_holder<Tag> modifier_holder_;
//->

        public:

        typedef /*<-*/ typename modifier_holder_::modifier_type BOOST_ASSIGN_V2_IGNORE(/*->*/ adapter_modifier<Tag> /*<-*/)/*->*/
        modifier_type;

        adapter_crtp(){}
        explicit adapter_crtp( F const& f )/*<-*/ : fun_holder_( f ){}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        explicit adapter_crtp( F const& f, modifier_type const& m )/*<-*/
            : fun_holder_( f ), modifier_holder_( m ){}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        typedef D const& result_type;

        template<typename R>
        result_type 
        operator()( as_arg_list_adapter<R> range )const/*<-*/
        {
             return ::boost::for_each( range(), wrapper<D>( this->derived() ) ); 
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->

        template<typename...Args>
        result_type operator()( Args&&...args )const/*<-*/
        {
            return  this->modify(
                /*<< Instance of F >>*/ this->fun( std::forward<Args>(args)... )
            );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
#else
        protected:
        typedef functor_aux::crtp_unary_and_up<
            adapter_crtp,
            ::boost::mpl::always<result_type>
        > super_t;

        public:
        using super_t::operator();

        result_type operator()()const
        {
            return this->modify( this->fun() );
        }

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
    BOOST_PP_INC(BOOST_ASSIGN_V2_LIMIT_ARITY),
    BOOST_ASSIGN_V2_MACRO,
    ~
)
#undef BOOST_ASSIGN_V2_MACRO
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

        // must be mutable
        C& container()const{ return this->derived().container(); }

        protected:

        template<typename T>
        struct ptr_enabler : boost::enable_if<
            container_traits::is_ptr_container<C>
        >{};

        template<typename T>
        result_type modify(T* t, typename ptr_enabler<T>::type* = 0)const
        {
            typedef put_concept::Modifier<Tag, C, T*> concept_;
            BOOST_CONCEPT_ASSERT(( concept_ ));
            this->modifier.impl( this->derived().container(), t );
            return this->derived();
        }

        template<typename T>
        void check_modifier( BOOST_ASSIGN_V2_FORWARD_PARAM(T, t) )const
        {
            typedef put_concept::Modifier<Tag, C,
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
            check_modifier( t );
            /*<< Instance of put_aux::adapter_modifier<Tag> >>*/this->modifier.impl(
                /*<< Reference to C >>*/this->derived().container(),
                std::forward<T>( t )
            );
            return this->derived();
        }
#else
        template<typename T>
        result_type modify(T& t)const
        {
            check_modifier( t );
            this->modifier.impl( this->derived().container(), t );
            return this->derived();
        }

        template<typename T>
        result_type modify(T const& t)const
        {
            check_modifier( t );
            this->modifier.impl( this->derived().container(), t );
            return this->derived();
        }

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

//->

    };

}// put_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_ADAPTER_CRTP_ER_2010_HPP
