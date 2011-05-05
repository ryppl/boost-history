//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_ITERATOR_ER_2011_HPP
#define BOOST_ASSIGN_V2_CHAIN_ITERATOR_ER_2011_HPP
#include <boost/assign/v2/ref/aux_/convert_traits/reference.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/range/detail/demote_iterator_traversal_tag.hpp>
#include <boost/range/detail/join_iterator.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace chain_aux{

    template<typename It1, typename It2>
    struct iterator
    {
        typedef typename boost::iterator_reference<It1>::type ref1_;
        typedef typename boost::iterator_reference<It2>::type ref2_;
        typedef ::boost::mpl::quote2<
            ref::convert_traits::reference
        > convert_;

        typedef typename ::boost::mpl::apply2<
            convert_,
            ref1_,
            ref2_
        >::type reference;

        typedef typename boost::remove_reference<reference>::type value_type;
        typedef boost::range_detail::join_iterator<
            It1
            ,It2
            ,value_type
            ,reference
            // , boost::single_pass_traversal_tag
        > type;

    };

}// chain_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_CHAIN_ITERATOR_ER_2011_HPP
