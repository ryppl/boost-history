//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_FRAME_MODIFIER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_FRAME_MODIFIER_ER_2010_HPP
#include <boost/concept_check.hpp>

namespace boost{
namespace assign{
namespace v2{
//[frame_modifier
namespace /*<< Template arguments to put_aux::modifier<> are declared within >>*/modifier_tag{}
namespace put_aux{

    template<typename Tag>
    struct /*<<Specialize on Tag>>*/ modifier{};

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
        static put_aux::modifier<Tag>& m;
        static C& cont;
        static T t;
    };

}
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_FRAME_MODIFIER_ER_2010_HPP
