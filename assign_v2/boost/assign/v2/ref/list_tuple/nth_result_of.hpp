//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_TUPLE_NTH_RESULT_OF_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_TUPLE_NTH_RESULT_OF_ER_2010_HPP
#include <boost/assign/v2/ref/list_tuple/container.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/unpack_args.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace nth_result_of{ 

    class list_tuple
    {
        typedef list_tuple_aux::root root_;
        typedef list_tuple_aux::container<0, root_> state_;

        struct forward_op{

            template<typename NewState, typename Vec>
            struct apply{
                typedef list_tuple_aux::result<NewState> meta_;
                typedef ::boost::mpl::unpack_args< meta_ > g_;
                typedef typename ::boost::mpl::apply1<
                    g_,
                    Vec
                >::type type;
            };

        };

        public:

        template<typename VecTypes>
        struct apply : ::boost::mpl::fold<
            VecTypes,
            state_,
            forward_op
        >{};

    };

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_LIST_TUPLE_NTH_RESULT_OF_ER_2010_HPP


