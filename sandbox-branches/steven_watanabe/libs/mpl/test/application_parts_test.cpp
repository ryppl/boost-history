#include "boost/mpl/application_parts.hpp"
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

using namespace boost;

template<class,class>struct f2{};

void test0(void)
{
   typedef unsigned expected_t0;
   typedef double expected_t1;
   typedef f2<mpl::arg<1>,mpl::arg<2> > expected_lambda;
   typedef f2<expected_t0,expected_t1> expected_application;
   typedef mpl::application_parts<expected_application>::type actual_parts;
   typedef mpl::vector<expected_lambda,expected_t0,expected_t1> expected_parts;
   BOOST_MPL_ASSERT((is_same<actual_parts,expected_parts>));
}

template<typename>struct f1{};

void test1(void)
{
   typedef mpl::arg<1> expected_t0;
   typedef f1<mpl::arg<2> > expected_t1;
   typedef f2<mpl::arg<1>,mpl::arg<2> > expected_lambda;
   typedef f2<expected_t0,expected_t1> expected_application;
   typedef mpl::application_parts<expected_application>::type actual_parts;
   typedef mpl::vector<expected_lambda,expected_t0,expected_t1> expected_parts;
   BOOST_MPL_ASSERT((is_same<actual_parts,expected_parts>));
}

#include <boost/mpl/at.hpp>
#include <boost/mpl/apply.hpp>

void test_arg_order(void)
//Tests ability to discern order of arg<1>,arg<2> args to template instantiation
//with placeholder arguments.
{
   //forward order, i.e. 1st arg contains arg<1>, 2nd arg contains arg<2>
   typedef f1<mpl::arg<1> > good_fwd_arg1;
   typedef    mpl::arg<2>   good_fwd_arg2;
   typedef f2<good_fwd_arg1,good_fwd_arg2> fwd_application;
   typedef mpl::application_parts<fwd_application>::type trial_fwd_parts;
   typedef mpl::at_c<trial_fwd_parts,1>::type trial_fwd_arg1;
   typedef mpl::apply2<trial_fwd_arg1,mpl::arg<1>,mpl::arg<1> >::type apply_fwd_arg1;
   BOOST_MPL_ASSERT((is_same<good_fwd_arg1,apply_fwd_arg1>));
   //reverse order, i.e. 1st arg contains arg<2>, 2nd arg contains arg<1>
   typedef f1<mpl::arg<2> > good_rev_arg1;
   typedef    mpl::arg<1>   good_rev_arg2;
   typedef f2<good_rev_arg1,good_rev_arg2> rev_application;
   typedef mpl::application_parts<rev_application>::type trial_rev_parts;
   typedef mpl::at_c<trial_rev_parts,1>::type trial_rev_arg1;
   typedef mpl::apply2<trial_rev_arg1,mpl::arg<1>,mpl::arg<1> >::type apply_rev_arg1;
   BOOST_MPL_ASSERT_NOT((is_same<good_rev_arg1,apply_rev_arg1>));
}

#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>

void test_unpack_args(void)
//Tests relationship to unpack_args.
{
   typedef unsigned expected_t0;
   typedef double expected_t1;
   typedef f2<expected_t0,expected_t1> expected_application;
   typedef mpl::application_parts<expected_application>::type actual_parts;
   typedef mpl::front<actual_parts>::type actual_lambda;
   typedef mpl::pop_front<actual_parts>::type actual_vec_args;
   typedef mpl::unpack_args<actual_lambda> actual_metaclass;
   typedef mpl::apply<actual_metaclass,actual_vec_args>::type actual_application;
   BOOST_MPL_ASSERT((is_same<actual_application,expected_application>));
}
