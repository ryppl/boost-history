//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_MODIFIER_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_MODIFIER_MAKE_ER_2010_HPP
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/assign/v2/detail/keyword/ignore.hpp>
#include <boost/assign/v2/put/frame/fwd.hpp>
#include <boost/assign/v2/put/frame/modifier.hpp>
#include <boost/assign/v2/put/modifier/modulo/meta.hpp>

namespace boost{
namespace assign{
namespace v2{
//[modulo_modifier
namespace put_aux{

    template<typename /*<<A class of modifiers>>*/ Keyword, typename /*<<A part of the modifier's implementation>>*/Arg = keyword_aux::ignore>
    struct modulo_modifier
    {
        
        modulo_modifier(){}
        modulo_modifier(Arg arg):arg_( arg ){}
        
        Arg const& arg()const{ return this->arg_; }

        template<typename Arg1>
        modulo_modifier<Keyword, Arg1>
        operator=(const Arg1& arg1)const{
            return modulo_modifier<Keyword, Arg1>( arg1 );
        }
        
        protected:
        Arg arg_;
    };
}//put_aux
//]
namespace put_aux{

    template<typename C, typename F, typename Tag, typename D, 
        typename Keyword, typename Arg>
    typename ::boost::mpl::apply2<
        result_of::modulo_modifier<D>, 
        Keyword, Arg
    >::type
    operator%(
        crtp<C, F, Tag, D> const& lhs,
        modulo_modifier<Keyword, Arg> const& rhs
    )
    {
        typedef put_aux::meta_modifier_tag<Keyword, Arg> meta_;
        typedef typename ::boost::mpl::apply1<meta_, D>::type modifier_tag;
        typedef put_aux::modifier<modifier_tag> modifier_;
    
        typedef typename ::boost::mpl::apply2<
            result_of::modulo_modifier<D>, 
            Keyword, Arg
        >::type result_;
    
        return result_(
            lhs.container(),
            lhs.fun,
            modifier_( lhs.modifier, rhs.arg() )
        );
    }

}// put_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_MODULO_MODIFIER_MAKE_ER_2010_HPP
