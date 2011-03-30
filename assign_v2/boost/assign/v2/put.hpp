//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_ER_2010_HPP
#include <boost/assign/v2/interpreter/crtp.hpp>
#include <boost/assign/v2/interpreter/data_generator.hpp>
#include <boost/assign/v2/interpreter/modifier.hpp>
#include <boost/assign/v2/interpreter/replace.hpp>
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_put
namespace interpreter_aux{

    template<typename C, typename F, typename ModifierTag, typename DataTag>
    class put_interpreter :
//<-
        protected ref::wrapper< ref::assign_tag::copy, C >,
//->
        public interpreter_aux::interpreter_crtp< 
        	C, 
            F, 
            ModifierTag, DataTag, put_interpreter<C, F, ModifierTag, DataTag> 
        >
    {
//<-
        typedef interpreter_aux::interpreter_crtp< 
        	C, F, ModifierTag, DataTag, put_interpreter 
        > super2_t;
//->
        public:

        typedef /*<-*/ typename super2_t::result_type 
        	BOOST_ASSIGN_V2_IGNORE(/*->*/ unspecified /*<-*/)/*->*/
        result_type;
//<-
        protected:

        typedef interpreter_aux::interpreter_modifier<ModifierTag> modifier_;
        typedef ref::assign_tag::copy assign_tag_;
        typedef ref::wrapper<assign_tag_,C> super1_t;
//->
        public:

        put_interpreter()/*<-*/
            {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        explicit put_interpreter( C& cont )/*<-*/
            : super1_t( cont )
        {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
        explicit put_interpreter( C& cont, F const& f, modifier_ const& m )/*<-*/
            : super1_t( cont ), super2_t( f, m )
        {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        C& container()const/*<-*/{
            return static_cast<super1_t const&>(*this).get();
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    };

    template<typename C, typename F, typename ModifierTag, typename DataTag>
    struct replace_data_generator< 
    	interpreter_aux::put_interpreter<C, F, ModifierTag, DataTag> 
    >{
        template<typename F1>
        struct apply/*<-*/{ 
        	typedef interpreter_aux::put_interpreter<
            	C, F1, ModifierTag, DataTag
            > type; 
        }/*->*/;
    };

    template<typename C, typename F, typename ModifierTag, typename DataTag>
    struct replace_modifier_tag< 
    	interpreter_aux::put_interpreter<C, F, ModifierTag, DataTag> 
    >{
        template<typename ModifierTag1>
        struct apply/*<-*/{ 
        	typedef interpreter_aux::put_interpreter<
            	C, F, ModifierTag1, DataTag
            > type; 
        }/*->*/;
    };

    template<typename C, typename F, typename ModifierTag, typename DataTag>
    struct replace_data_tag< 
    	interpreter_aux::put_interpreter<C, F, ModifierTag, DataTag> 
    >{
        template<typename DataTag1>
        struct apply/*<-*/{ 
        	typedef interpreter_aux::put_interpreter<
            	C, F, ModifierTag, DataTag1
            > type; 
        }/*->*/;
    };

}// interpreter_aux
namespace result_of{

    template<
    	typename C/*<-*/
		, typename DataGenerator 
        	= typename interpreter_aux::deduce_data_generator<C>::type 
        , typename ModifierTag 
        	= typename interpreter_aux::deduce_modifier_tag<C>::type
        , typename DataTag 
        	= typename interpreter_aux::deduce_data_tag<C>::type/*->*/
    >
    struct put/*<-*/{
        typedef 
        	interpreter_aux::put_interpreter<
            	C, 
                DataGenerator, 
                ModifierTag, 
                DataTag
            > 
        type;
    }/*->*/;

}// result_of

    template<typename C>
    typename result_of::put<C>::type
    put( C& cont )/*<-*/
    {
        typedef typename result_of::put<C>::type result_;
        return result_( cont );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

/*<-*/
    template<
    	typename DataGenerator 
        ,typename ModifierTag 
        ,typename DataTag 
        ,typename C
    >
    typename result_of::put<C, DataGenerator, ModifierTag, DataTag>::type
    put( C& cont )
    {
    	typedef typename result_of::put<
        	C, DataGenerator, ModifierTag, DataTag
        >::type result_;
        return result_( cont );
    }

/*->*/
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_ER_2010_HPP
