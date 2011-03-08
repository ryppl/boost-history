// TODO remove file
/*
//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODULO_STD_FRAME_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODULO_STD_FRAME_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/put/frame/fwd.hpp>
#include <boost/assign/v2/put/frame/modifier.hpp>
#include <boost/assign/v2/put/modulo/modifier.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

    template<typename Tag>
    struct modulo_std
    {
        modulo_std(){}
        modulo_std(Tag const& t){}// : modifier_tag( t ){}
        Tag modifier_tag;
    };

    template<typename C, typename F, typename Tag, typename D, typename Tag1>
    typename ::boost::mpl::apply1<result_of_modulo::modifier<D>, Tag1>::type
    operator%(
        crtp<C, F, Tag, D> const& lhs,
        modulo_std<Tag1> const& rhs
    )
    {
        typedef result_of_modulo::modifier<D> meta_;

        typedef typename ::boost::mpl::apply1<meta_, Tag1>::type result_;
        typedef put_aux::modifier<Tag1> modifier_;
        return result_(
            lhs.container(),
            lhs.fun,
            modifier_() // rhs.modifier_tag )
        );
    }

    struct kwd_std
    {
        template<typename Tag>
        modulo_std<Tag> operator=( Tag const& t)const
        {
            return modulo_std<Tag>( t );
        }
    };

}// put_aux
namespace{
    put_aux::kwd_std const _std = put_aux::kwd_std();
}
}// v2
}// assign
}// boost

#endif
*/
