//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_MODULO_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_MODULO_TRAITS_ER_2010_HPP
#include <boost/assign/v2/ref/list/as_modulo_list.hpp>
#include <boost/assign/v2/ref/list/nth_result_of.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector/vector0.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace put_pipe_aux{

    typedef ::boost::mpl::vector0<> empty_par_list_; 

    typedef ref::list_aux::size_type modulo_size_type;

    template<typename ParList>
    struct modulo_traits
    {
        typedef ref::nth_result_of::list<use_default> meta_;
        typedef typename ::boost::mpl::apply1<meta_, ParList>::type cont_;
        typedef typename ::boost::mpl::size<ParList>::type size;

        template<typename P>
        struct next_par_list : ::boost::mpl::push_back<
        	ParList, 
            P const&
        >{};

    };

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_MODULO_TRAITS_ER_2010_HPP
