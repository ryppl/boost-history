//-----------------------------------------------------------------------------
// boost mpl/equal.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_EQUAL_HPP
#define BOOST_MPL_EQUAL_HPP

#include "boost/mpl/while_true.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/logical.hpp"
#include "boost/mpl/identity/select1st.hpp"
#include "boost/mpl/identity/select2nd.hpp"
#include "boost/mpl/utility/next.hpp"
//#include "boost/mpl/compose/f_gx.hpp"
//#include "boost/mpl/compose/f_gx_hx.hpp"
#include "boost/mpl/pair.hpp"
#include "boost/mpl/type_compare.hpp"
#include "boost/type_traits/same_traits.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<
      typename EndIterator1
    , typename EndIterator2
    >
struct equal_loop_predicate
{
    template<typename Statement> struct apply
    {
        typedef typename Statement::first iterator1;
        typedef typename Statement::second iterator2;

        typedef typename mpl::logical_and< 
              mpl::logical_not< boost::is_same<iterator1,EndIterator1> >
            , mpl::logical_not< boost::is_same<iterator2,EndIterator2> >
            , mpl::type_compare<iterator1,iterator2>
            >::type type;

        BOOST_STATIC_CONSTANT(bool, value = type::value);
    };
};

struct equal_loop_func
{
    template<typename Pair> struct apply
    {
        typedef typename mpl::next<typename Pair::first>::type first_;
        typedef typename mpl::next<typename Pair::second>::type second_;
        typedef mpl::pair<first_,second_> type;
    };
};

} // namespace detail

template<
      typename Sequence1
    , typename Sequence2
    >
struct equal
{
 private:
    typedef typename mpl::begin<Sequence1>::iterator first1_;
    typedef typename mpl::begin<Sequence2>::iterator first2_;
  	typedef typename mpl::end<Sequence1>::iterator last1_;
	  typedef typename mpl::end<Sequence2>::iterator last2_;
	
    typedef mpl::while_true<
          mpl::detail::equal_loop_predicate<last1_,last2_>
        , mpl::pair<first1_,first2_>
        , mpl::detail::equal_loop_func
// it was a nice exercise ;), but turned out to be too stressful for the compiler
//        , mpl::compose_f_gx_hx<
//                mpl::make_pair<mpl::placeholder,mpl::placeholder>
//              , mpl::compose_f_gx< mpl::next<mpl::placeholder>,mpl::select1st<mpl::_1> >
//              , mpl::compose_f_gx< mpl::next<mpl::placeholder>,mpl::select2nd<mpl::_1> >
//              >
        > loop_;
    
    typedef typename loop_::executed::state result_;
 
 public:
    BOOST_STATIC_CONSTANT(bool, value = 
        (::boost::mpl::logical_and< 
                boost::is_same<typename result_::first,last1_>
              , boost::is_same<typename result_::second,last2_>
              >::value)
        );
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_EQUAL_HPP
