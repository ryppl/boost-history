//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_VALUE_ADAPTER_MODIFIER_ER_2010_HPP
#define BOOST_ASSIGN_V2_VALUE_ADAPTER_MODIFIER_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/concept_check.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_put_adapter_modifier
namespace /*<< Template arguments to `value_aux::adapter_modifier<>` have to be declared within this `namespace`>>*/modifier_tag{}
namespace value_aux{

    template<typename Tag>
    struct /*<<Specialize on Tag>>*/ adapter_modifier{};

}
namespace put_concept{

    template<typename Tag, /*<<Container>>*/ typename C, /*<<Input>>*/ typename T>
    struct Modifier
    {

        BOOST_CONCEPT_USAGE(Modifier)
        {
            m.impl( cont, t ); 
        }

        private:
        static value_aux::adapter_modifier<Tag>& m;
        static C& cont;
        static T t;
    };

}
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_VALUE_ADAPTER_MODIFIER_ER_2010_HPP
