//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_INTERPRETER_DATA_ER_2011_HPP
#define BOOST_ASSIGN_V2_INTERPRETER_DATA_ER_2011_HPP
#include <boost/assign/v2/support/traits/container.hpp>
#include <boost/assign/v2/support/switch.hpp>
#include <boost/assign/v2/support/functor/pair.hpp>
#include <boost/assign/v2/support/functor/value.hpp>
#include <boost/assign/v2/interpreter/fwd.hpp>
#include <boost/assign/v2/option/data.hpp>
    
namespace boost{
namespace assign{
namespace v2{
//[syntax_interpreter_data
namespace switch_tag{
    struct data_generator{};
}// switch_tag

/*<-*/
#define BOOST_ASSIGN_V2_SWITCH_TAG data_generator
BOOST_ASSIGN_V2_SWITCH_CASE(0, container_aux::is_multi_array, element_)
BOOST_ASSIGN_V2_SWITCH_CASE(1, container_aux::is_map, map_)
BOOST_ASSIGN_V2_SWITCH_CASE_DEFAULT(2, value_)
#undef BOOST_ASSIGN_V2_SWITCH_TAG
/*->*/

namespace interpreter_aux{

    template<    
        typename C    // Value- or pointer-container
    >
    struct deduce_data_generator/*<-*/
        : data_generator<
            C, 
            typename switch_aux::result< 
                switch_tag::data_generator,
                C
            >::type
        >
    {}/*->*/;

}// interpreter_aux
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_INTERPRETER_DATA_ER_2011_HPP
