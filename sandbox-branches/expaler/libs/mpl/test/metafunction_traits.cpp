// (C) Copyright Jonathan Turkanis 2003.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.

// Tests metafunction_traits with metafunction classes with up to five
// parameters and with the lambda expressions occuring in the MPL documentation.

#if defined(__BORLANDC__) && (__BORLANDC__ >= 0x600)
#define BOOST_STRICT_CONFIG
#endif

//--------------Headers for lambda expressions--------------------------------//

#include <boost/mpl/deref.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/type_traits.hpp>

//--------------Headers for metafunction classes------------------------------//

#include <boost/config.hpp>                 // BOOST_STATIC_CONSTANT.
#include <boost/mpl/always.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/same_as.hpp>

//--------------Headers for non-metafunctions---------------------------------//

#include <boost/mpl/protect.hpp>
#include <string>   // Sample non-metafunction template.
#include <boost/mpl/is_metafunction_or_lambda.hpp>

#define CHECK_LAMBDA_EXPRESSION(expr_)                                \
    { BOOST_STATIC_ASSERT(is_lambda_expression< expr_ >::value); }    \
    { BOOST_STATIC_ASSERT(!is_metafunction_class< expr_ >::value); }  \
    /**/

#define CHECK_METAFUNCTION(func_)                                    \
    { BOOST_STATIC_ASSERT(!is_lambda_expression< func_ >::value); }  \
    { BOOST_STATIC_ASSERT(is_metafunction_class< func_ >::value); }  \
    /**/

#define CHECK_NON_METAFUNCTION(t)                                 \
    { BOOST_STATIC_ASSERT(!is_lambda_expression< t >::value); }   \
    { BOOST_STATIC_ASSERT(!is_metafunction_class< t >::value); }  \
    /**/

//--------------Lambda expressions from the MPL documentation-----------------//

using namespace boost;
using namespace boost::mpl;

// From MPL Paper.
typedef remove_const<_1>                          expr1;

// From MPL Paper.
typedef less< sizeof_<_1>, int_<16> >             expr2;

// From iter_fold.
typedef if_<
            less< deref<_1>, deref<_2> >,
            _2,
            _1
        >                                         expr3;

// From iter_fold_backward.
typedef if_<
            less< deref<_2>, int_<0> >,
            push_front<_1,_2>,
            _1
        >                                         expr4;

// From fold.
typedef if_< is_float<_2>, next<_1>, _1 >  expr5;

// From copy.
typedef push_back<_, _>                           expr6;

// From copy_if.
typedef less< _1, int_<5> >                       expr7;

// From find_if.
typedef is_same<_1, unsigned>                     expr8;

// From count_if.
typedef is_float<_>                               expr9;
typedef is_same<_, char>                          expr10;
typedef is_same<_, void>                          expr11;

// From max_element.
typedef sizeof_<_1>                               expr12;

// From transform.
typedef add_pointer<_1>                           expr13;

// From remove_if.
typedef greater<_, int_<4> >                      expr14;

//--------------Metafunction classes from the MPL documentation---------------//

struct always_true {
    typedef true_ type;
};

template<long N>
struct le {
    template<typename M>
    struct apply {
        BOOST_STATIC_CONSTANT(bool, value = M::value < N);
        typedef bool_<value> type;
    };
};

typedef le<5> less_than_5;

struct less_than {
    template<typename N1, typename N2>
    struct apply {
        BOOST_STATIC_CONSTANT(bool, value = N1::value < N2::value);
        typedef bool_<value> type;
    };
};

//--------------Additional metafunction classes-------------------------------//

struct meta0 {
    typedef int type;
};

struct meta1 {
    template<typename T1>
    struct apply { typedef int type; };
};

struct meta2 {
    template<typename T1, typename T2>
    struct apply { typedef int type; };
};

struct meta3 {
    template<typename T1, typename T2, typename T3>
    struct apply { typedef int type; };
};

struct meta4 {
    template<typename T1, typename T2, typename T3, typename T4>
    struct apply { typedef int type; };
};

struct meta5 {
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    struct apply { typedef int type; };
};

struct meta_with_defaults1 {
    template<typename T1 = int>
    struct apply { typedef int type; };
};

struct meta_with_defaults2 {
    template<typename T1, typename T2 = int>
    struct apply { typedef int type; };
};

struct meta_with_defaults3 {
    template<typename T1, typename T2 = int, typename T3 = int>
    struct apply { typedef int type; };
};

struct meta_with_defaults4 {
    template<typename T1, typename T2, typename T3 = int, typename T4 = int>
    struct apply { typedef int type; };
};

struct meta_with_defaults5 {
    template< typename T1, typename T2, typename T3,
              typename T4 = int, typename T5 = int >
    struct apply { typedef int type; };
};

struct meta_returning_pointer {
    typedef int* type;
};

struct meta_returning_reference {
    typedef int& type;
};

struct meta_returning_array {
    typedef int type[1];
};

struct meta_returning_function {
    typedef void (type)();
};

typedef always<int> always_int;

typedef same_as<int> same_as_int;

//--------------Protected lambda expression-----------------------------------//

typedef remove_const< protect<expr3> > protected_expr;

int main()
{
        /* From the MPL Docs */

    CHECK_LAMBDA_EXPRESSION(expr1);
    CHECK_LAMBDA_EXPRESSION(expr2);
    CHECK_LAMBDA_EXPRESSION(expr3);
    CHECK_LAMBDA_EXPRESSION(expr4);
    CHECK_LAMBDA_EXPRESSION(expr5);
    CHECK_LAMBDA_EXPRESSION(expr6);
    CHECK_LAMBDA_EXPRESSION(expr7);
    CHECK_LAMBDA_EXPRESSION(expr8);
    CHECK_LAMBDA_EXPRESSION(expr9);
    CHECK_LAMBDA_EXPRESSION(expr10);
    CHECK_LAMBDA_EXPRESSION(expr11);
    CHECK_LAMBDA_EXPRESSION(expr12);
    CHECK_LAMBDA_EXPRESSION(expr13);
    CHECK_LAMBDA_EXPRESSION(expr14);

        /* From the MPL Docs */

    CHECK_METAFUNCTION(always_true);
    CHECK_METAFUNCTION(less_than_5);
    CHECK_METAFUNCTION(less_than);

        /* Additional metafunction classes */

    CHECK_METAFUNCTION(always_int);
    CHECK_METAFUNCTION(same_as_int);
    CHECK_METAFUNCTION(meta1);
    CHECK_METAFUNCTION(meta2);
    CHECK_METAFUNCTION(meta3);
    CHECK_METAFUNCTION(meta4);
    CHECK_METAFUNCTION(meta5);
    CHECK_METAFUNCTION(meta_with_defaults1);
    CHECK_METAFUNCTION(meta_with_defaults2);
    CHECK_METAFUNCTION(meta_with_defaults3);
    CHECK_METAFUNCTION(meta_with_defaults4);
    CHECK_METAFUNCTION(meta_with_defaults5);
    CHECK_METAFUNCTION(meta_returning_reference);
    CHECK_METAFUNCTION(meta_returning_array);
    CHECK_METAFUNCTION(meta_returning_function);

        /* Protected lambda expression */

    BOOST_STATIC_ASSERT(!is_lambda_expression<protected_expr>::value);

        /* Non-metatypes */

    CHECK_NON_METAFUNCTION(std::string);
    CHECK_NON_METAFUNCTION(int);
    CHECK_NON_METAFUNCTION(int*);
    CHECK_NON_METAFUNCTION(int&);
    CHECK_NON_METAFUNCTION(int[1]);
    CHECK_NON_METAFUNCTION(int(*)[1]);
    CHECK_NON_METAFUNCTION(int(&)[1]);
    CHECK_NON_METAFUNCTION(void(*)());
    CHECK_NON_METAFUNCTION(void(&)());
    CHECK_NON_METAFUNCTION(void());

    return 0;
}
