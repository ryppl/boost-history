//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DEQUE_DEQUE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_DEQUE_DEQUE_ER_2010_HPP
#include <boost/assign/v2/detail/config/enable_cpp0x.hpp>
#include <boost/assign/v2/detail/keyword/nil.hpp>
#include <boost/assign/v2/put/fun/deduce.hpp>
#include <boost/assign/v2/put/modifier/deduce.hpp>
//#include <boost/assign/v2/put/deque/fwd.hpp>
#include <boost/assign/v2/put/deque/adapter.hpp>
#include <boost/type_traits/remove_cv.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <utility>
#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{
namespace result_of{

    template<typename T>
    struct deque
    {
        typedef typename boost::remove_cv<T>::type t_;
        typedef typename put_aux::deque_impl<t_>::type impl_;
        typedef typename put_aux::deduce_fun<impl_>::type f_;
        typedef typename put_aux::deduce_modifier_tag<impl_>::type modifier_tag_;
        typedef put_aux::deque_adapter<t_,f_,modifier_tag_> type;
    };

}// result_of

    template<typename T>
    typename result_of::deque<T>::type
    deque( keyword_aux::nil )
    {
        return typename result_of::deque<T>::type();
    }

}// put_aux
namespace result_of{

    template<typename T>
    struct deque 
        : put_aux::result_of::deque<T>
    {};

}// result_of
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
namespace put_aux{

    template<typename T, typename...Args>
    typename result_of::deque<T>::type
    deque(Args&&...args)
    {
        return deque<T>( v2::_nil )( std::forward<Args>(args)... );
    }

}// put_aux

using put_aux::deque;

#endif // BOOST_ASSIGN_V2_ENABLE_CPP0X
}// v2
}// assign
}// boost

#if !BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <boost/assign/v2/put/deque/cpp03/deque.hpp>
#endif

#endif // BOOST_ASSIGN_V2_PUT_DEQUE_DEQUE_ER_2010_HPP
