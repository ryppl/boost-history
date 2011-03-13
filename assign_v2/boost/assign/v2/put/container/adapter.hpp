//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CONTAINER_ADAPTER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_CONTAINER_ADAPTER_ER_2010_HPP
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/put/container/fwd.hpp> // consistency
#include <boost/assign/v2/put/adapter/crtp.hpp>
#include <boost/assign/v2/put/adapter/modifier.hpp>
#include <boost/assign/v2/put/adapter/replace_parameter.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

    template<typename C, typename F, typename Tag>
    class container_adapter
        : protected ref::wrapper< ref::assign_tag::copy, C >
        , public put_aux::crtp< C, F, Tag, container_adapter<C, F, Tag> >
    {
        typedef put_aux::crtp< C, F, Tag, container_adapter > super2_t;

        public:

        typedef typename super2_t::result_type result_type;

        protected:

        typedef put_aux::adapter_modifier<Tag> modifier_;
        typedef ref::assign_tag::copy assign_tag_;
        typedef ref::wrapper<assign_tag_,C> super1_t;

        public:

        container_adapter(){}
        explicit container_adapter( C& cont ) : super1_t( cont ) {}
        explicit container_adapter( C& cont, F const& f, modifier_ const& m )
            : super1_t( cont ), super2_t( f, m )
            {
                // This constructor is required in conjunction with modulo
            }

        C& container()const{
            return static_cast<super1_t const&>(*this).get();
        }

    };

    template<typename C, typename F, typename Tag>
    struct replace_fun< put_aux::container_adapter<C, F, Tag> >
    {
        template<typename F1>
        struct apply{ typedef put_aux::container_adapter<C, F1, Tag> type; };
    };

    template<typename C, typename F, typename Tag>
    struct replace_modifier_tag< put_aux::container_adapter<C, F, Tag> >
    {
        template<typename Tag1>
        struct apply{ typedef put_aux::container_adapter<C, F, Tag1> type; };
    };

}// put_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_CONTAINER_ADAPTER_ER_2010_HPP
