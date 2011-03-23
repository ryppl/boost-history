//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PIPE_OPTION_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_V2_PIPE_OPTION_TRAITS_ER_2010_HPP
#include <boost/assign/v2/ref/aux_/list/as_modulo_list.hpp>
#include <boost/assign/v2/ref/aux_/list/list.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector/vector0.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace interpreter_aux{

    typedef ::boost::mpl::vector0<> empty_option_list_; 
    typedef ref::list_aux::size_type option_size_type;

    template<typename OptionList>
    struct option_traits
    {
        typedef ref::nth_result_of::list<use_default> meta_;
        typedef typename ::boost::mpl::apply1<meta_, OptionList>::type cont_;
        typedef typename ::boost::mpl::size<OptionList>::type size;

        template<typename O>
        struct next_option_list : ::boost::mpl::push_back<
            OptionList, 
            O const&
        >{};
    };

}// interpreter_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PIPE_OPTION_TRAITS_ER_2010_HPP
