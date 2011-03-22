//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_KEY_ER_2010_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_KEY_ER_2010_HPP
#include <boost/assign/v2/interpreter/data.hpp>
#include <boost/assign/v2/optional/data.hpp>
#include <boost/mpl/apply.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_optional_key
namespace aux{

    template<typename C>
    struct container_key{ typedef typename C::key_type type; };

    template<typename /*<<Value or pointer-container>>*/C>
    struct /*<<Metafunction>>*/deduce_key_generator/*<-*/
        :  boost::mpl::eval_if<
            ptr_container_aux::is_ptr_container<C>,
            deduce_ptr_generator<C, container_key>,
            deduce_value_generator<C, container_key>
        >
    {}/*->*/;

    struct optional_key{};


namespace result_of{

    template<typename C, typename D>
    struct optional_key : ::boost::mpl::apply1<
        aux::result_of::optional_data_generator<D>,
        typename deduce_key_generator<C>::type
    >{};

}//result_of

    template<typename C, typename F, typename Tag, typename D>
    typename result_of::optional_key<C, D>::type
    operator%/*<<Overrides data generator with a constructor for C::key_type>>*/(
        interpreter_crtp<C, F, Tag, D> const& lhs,
        optional_key rhs
    )/*<-*/
    {
        typedef typename deduce_key_generator<C>::type gen_;
        return lhs % ( _data = gen_() );
    }/*<-*/BOOST_ASSIGN_V2_IGNORE(/*<-*/;/*->*/)/*->*/


}// aux
//<-
namespace{
//->
    aux::optional_key const _key/*<-*/= aux::optional_key()/*->*/;
//]
}
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_KEY_ER_2010_HPP
