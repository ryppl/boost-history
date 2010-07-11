/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/mpl/modulus.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/noncopyable.hpp>

template<typename N, typename WithNoncopyable>
struct generate_types
{
    typedef
        mpl::vector<
            char
          , int
          , float
          , double
          , std::string
          , mpl::vector<>
        >
    base_sequence;

    typedef typename
        mpl::eval_if<
            WithNoncopyable
          , mpl::push_back<base_sequence, boost::noncopyable>
          , mpl::identity<base_sequence>
        >::type
    sequence;

    typedef typename
        mpl::transform<
            mpl::range_c<typename N::value_type, 0, N::value>
          , mpl::at<
                sequence
              , mpl::modulus<mpl::_1, typename mpl::size<sequence>::type>
            >
          , mpl::back_inserter<mpl::vector<> >
        >::type
    type;
};
