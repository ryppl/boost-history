//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CONVERSION_HELPER_ER_2010_HPP
#define BOOST_ASSIGN_V2_CONVERSION_HELPER_ER_2010_HPP
#include <boost/mpl/bool.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace conversion_aux{

    // This is in replacement of switch_aux::helper since here we need
    // two arguments.

    template<typename T, typename U>
    struct default_f : ::boost::mpl::true_{};

    template<typename Tag,
        template<typename, typename> class F = conversion_aux::default_f>
    struct helper
    {
        typedef Tag tag;
        template<typename T>  // T must derive from mpl::pair<>
        struct apply
            : F<typename T::first, typename T::second>
        {
        };
    };

}// conversion_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_CONVERSION_HELPER_ER_2010_HPP
