// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_RESULT_OF_HPP
#define BOOST_FUSION_SUPPORT_RESULT_OF_HPP

#include <boost/utility/result_of.hpp>

//cschmidt: fusion might call result_of on class types without an operator().
//Therefore we emulate the old behavior if (and only if) the boost
//implementation falls back to decltype by default.

#ifndef BOOST_NO_DECLTYPE
#   include <boost/fusion/support/ref.hpp>

#   include <boost/mpl/has_xxx.hpp>
#   include <boost/mpl/eval_if.hpp>

#   include <boost/type_traits/is_function.hpp>
#endif

namespace boost { namespace fusion { namespace support
{
#ifdef BOOST_NO_DECLTYPE
    using boost::result_of;
#else
    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)

        template<typename F>
        struct get_result_type
        {
            typedef typename F::result_type type;
        };

        template<typename F, typename Sig>
        struct get_result
          : F::template result<Sig>
        {};

        //TODO cschmidt: non-native variadic templates version
        template<typename IdentityF, typename... Args>
        struct result_of_class_type
        {
            typedef typename
                mpl::eval_if<
                    detail::has_result_type<IdentityF>
                  , detail::get_result_type<IdentityF>
                    //TODO cschmidt: fallback to boost::result_of (decltype) if
                    //'F::template result' does not exist.
                    //Is this even possible?
                  , detail::get_result<IdentityF,IdentityF(Args...)>
                >::type
            type;
        };
    }

    template<typename Sig>
    struct result_of
      : boost::result_of<Sig>
    {
    };

    template<typename F, typename... Args>
    struct result_of<F(Args...)>
    {
        //TODO cschmidt: workaround until is_function supports rvalues
        typedef typename fusion::detail::identity<F>::type f;

        typedef typename
            mpl::eval_if<
                is_function<f>
              , boost::result_of<F(Args...)>
              , detail::result_of_class_type<f,Args...>
            >::type
        type;
    };
#endif

}}}

#endif
