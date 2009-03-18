/////////////////////////////////////////////////////////////////////////////
// creation.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
// Disclaimer :
//
// This code is a minor modification to Boost.Accumulator and serves a
// particular purpose not readily provided by the original library.
// Contact the Boost library for copyright implications.
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SHARED_FEATURES_CREATION_HPP_ER_2009
#define BOOST_SHARED_FEATURES_CREATION_HPP_ER_2009
#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/value_of.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/cons.hpp>
#include <boost/fusion/include/any.hpp>
#include <boost/fusion/include/find_if.hpp>
#include <boost/shared_features/mpl_features.hpp>
namespace boost{namespace shared_features{

namespace detail{

template<
    typename First,
    typename Last,
    bool is_empty = fusion::result_of::equal_to<First,Last>::value
> struct build_acc_list;

template<typename First,typename Last>
struct build_acc_list<First,Last,true>
{
        typedef fusion::nil type;
        template<typename Args>
        static fusion::nil
        call(Args const & a, First const & f, Last const & l)
        {
                return fusion::nil();
        }
};

template<typename First,typename Last>
struct build_acc_list<First,Last,false>
{
        typedef build_acc_list<
            typename fusion::result_of::next<First>::type,
            Last
        > next_build_acc_list;

        typedef  typename fusion::result_of::value_of<First>::type
                                                                first_type;

        typedef fusion::cons<
            first_type,
            typename next_build_acc_list::type
        > type;

        template<typename Args>
        static type
        call(Args const & args, First const & f, Last const & l)
        {
                first_type first(args);
                return type(
                    args,
                    next_build_acc_list::call(args,fusion::next(f),l)
                );
        }
};
    namespace meta{
        template<typename Sequence>
        struct make_acc_list : build_acc_list<
            typename fusion::result_of::begin<Sequence>::type,
            typename fusion::result_of::end<Sequence>::type>
        {};
    }

    template<typename Sequence,typename Args>
    typename meta::make_acc_list<Sequence>::type
    make_acc_list(Sequence const &seq, Args const& args)
    {

        return meta::make_acc_list<Sequence>::call(args,fusion::begin(seq),
            fusion::end(seq));
    }



}//detail
}//shared_features
}//boost


#endif
