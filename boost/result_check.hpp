// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RESULT_CHECK_040913_HPP
#define BOOST_RESULT_CHECK_040913_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/always.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>

// The deduction isn't perfect:
//    T x();        // will deduce to T
//    T& x();       // will deduce to T&
//    T const& x(); // will deduce to T const&
//    T const x();  // will deduce to T const&

namespace boost {

    namespace result_check_ {

        struct no_overload_tag {};
        struct void_result_base {};
        struct void_result_tag : void_result_base {};

        template<class T> struct rvalue {};
        template<class T> struct lvalue {};

        template<class T> rvalue<T> operator,(T const&, void_result_base);
        template<class T> lvalue<T> operator,(T&, void_result_tag);

        typedef char (&yes_tag)[1];
        typedef char (&no_tag)[2];

        yes_tag to_yesno(mpl::true_);
        no_tag to_yesno(mpl::false_);

        template<class P, class T>
        typename mpl::apply1<P, T&>::type check(lvalue<T>, int, P*);

        template<class P, class T>
        typename mpl::apply1<P, T>::type check(rvalue<T>, int, P*);

        template<class P>
        typename mpl::apply1<P, void>::type check(void_result_tag, long, P*);

        template<class P>
        mpl::false_ check(rvalue<no_overload_tag>, long, P*);

        struct any
        {
            template<class T> any(T const&);
        };

        typedef mpl::always<mpl::true_> default_predicate;

    } // namespace result_check_

#define BOOST_RESULT_CHECK_LOCAL_ARG(z, n, text)                         \
    static typename boost::add_reference<T##n>::type                     \
        BOOST_PP_CAT(_, BOOST_PP_INC(n));

#define BOOST_RESULT_CHECK(arity, name, function, operation)             \
    namespace result_check_ {                                            \
                                                                         \
        using namespace boost::result_check_;                            \
                                                                         \
        no_overload_tag function(                                        \
            BOOST_PP_ENUM_PARAMS(arity, any BOOST_PP_INTERCEPT));        \
                                                                         \
        template<                                                        \
            BOOST_PP_ENUM_PARAMS(arity, class T)                         \
          , class Predicate = default_predicate                          \
        >                                                                \
        struct check_##name                                              \
        {                                                                \
            BOOST_PP_REPEAT(arity, BOOST_RESULT_CHECK_LOCAL_ARG, _)      \
                                                                         \
            BOOST_STATIC_CONSTANT(bool,                                  \
                value = (                                                \
                    sizeof(                                              \
                        to_yesno(                                        \
                            check(                                       \
                                ((operation), void_result_tag())         \
                              , 0L                                       \
                              , (Predicate*)0                            \
                            )                                            \
                        )                                                \
                    ) == sizeof(yes_tag)                                 \
                )                                                        \
            );                                                           \
                                                                         \
            typedef boost::mpl::bool_<value> type;                       \
        };                                                               \
    }                                                                    \
    using result_check_::check_##name;

} // namespace boost

#endif // BOOST_RESULT_CHECK_040913_HPP

