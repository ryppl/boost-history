//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_UTILITY_CSV_RESULT_ER_2010_HPP
#define BOOST_ASSIGN_V2_UTILITY_CSV_RESULT_ER_2010_HPP
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace csv_aux{

    template<typename F, typename T>
    struct unary_result
    {
        static F f;
        static T t;
        typedef BOOST_TYPEOF_TPL( f( t ) ) type;
    };

    template<typename F>
    struct result
    {

        typedef F state_;

        template<typename Vec>
        struct apply : ::boost::mpl::fold<
            Vec,
            state_,
            unary_result< ::boost::mpl::_1, ::boost::mpl::_2>
        >{};

    };

}// csv_aux
namespace result_of{

    template<typename F, typename V>
    struct csv : ::boost::mpl::apply1<
    	csv_aux::result<F>,
        V
    >{};

}// nth_result_of
}// v2
}// assign
}// boost

#endif
