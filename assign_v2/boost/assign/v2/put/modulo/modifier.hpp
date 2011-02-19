//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_MODIFIER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_MODIFIER_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/put/frame/modifier.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace result_of_modulo{

    template<typename D> struct modifier /* Specialize on D(erived) */{
        template<typename Tag> struct apply{};
    };

}// result_of_modulo
namespace put_modulo_aux{

    template<typename Tag> struct modifier
    {
        modifier(){}
        modifier(Tag const& t) : modifier_tag( t ){}
        Tag modifier_tag;
    };

    template<typename T, typename NewTag>
    typename ::boost::mpl::apply1<result_of_modulo::modifier<T>, NewTag>::type
    operator%(T const& t, put_modulo_aux::modifier<NewTag> const& h )
    {
        typedef result_of_modulo::modifier<T> meta_;

        typedef typename ::boost::mpl::apply1<meta_, NewTag>::type result_;
        typedef put_aux::modifier<NewTag> modifier_;
        return result_( t.container(), t.fun, modifier_( h.modifier_tag ) );
    }

    struct kwd_modifier
    {
        template<typename Tag>        
        modifier<Tag> operator=( Tag const& t)const
        {
            return modifier<Tag>( t );
        }
    };

}// put_modulo_aux
namespace{
    put_modulo_aux::kwd_modifier const _modifier 
        = put_modulo_aux::kwd_modifier();
}
}// v2
}// assign
}// boost

#endif
