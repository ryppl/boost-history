//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_FWD_ER_2011_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_FWD_ER_2011_HPP
#include <boost/assign/v2/support/traits/container.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace data_tag{

    // Storage
    struct storage{};
    struct storage_ptr : storage{};
    struct storage_value : storage{};

    // Assign
    struct assign{};
    struct assign_map : assign{};

    template<typename Storage, typename Assign> struct plus : Storage{};

    // Shortcuts
    typedef plus<storage_ptr, assign> ptr;
    typedef plus<storage_ptr, assign_map> ptr_map;
    typedef plus<storage_value, assign> value;
    typedef plus<storage_value, assign_map> value_map;

}// data_tag
namespace interpreter_aux{

    template<
        typename C // Value- or pointer-container
    >
    struct deduce_data_generator;

    template<typename C> struct deduce_data_tag;

    template<typename D, typename C, typename F, typename MTag, typename DTag>
    class interpreter_crtp;

    template<typename Tag> struct interpreter_modifier;

    template<typename Derived> struct replace_data_generator;
    template<typename Derived> struct replace_modifier_tag;
    template<typename Derived> struct replace_data_tag;

    template<typename R>
    struct as_arg_list_adapter;

}// interpreter_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_FWD_ER_2011_HPP

