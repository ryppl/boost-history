/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_FUNCTION_FUNCTION_HPP
#define PHOENIX_FUNCTION_FUNCTION_HPP

#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/phoenix/core/domain.hpp>
#include <boost/phoenix/core/limits.hpp>
#include <boost/phoenix/support/iterate.hpp>
#include <boost/proto/make_expr.hpp>
#include <boost/proto/tags.hpp>
#include <boost/utility/result_of.hpp>

namespace boost { namespace phoenix
{
    ////////////////////////////////////////////////////////////////////////////
    // Functions
    ////////////////////////////////////////////////////////////////////////////
    
    
    namespace rule
    {
        struct function
            : proto::nary_expr<proto::tag::function, proto::vararg<meta_grammar> >
        {};
    }

    template <typename Dummy>
    struct meta_grammar::case_<proto::tag::function, Dummy>
        : proto::when<rule::function, proto::external_transform>
    {};

    namespace result_of
    {
        template <typename F,
            PHOENIX_typename_A_void(PHOENIX_ACTOR_LIMIT),
            typename Dummy = void>
        struct function;

        template <typename F>
        struct function<F>
          : proto::result_of::make_expr<
                proto::tag::function
              , phoenix_domain
              , F>
        {};

        template <typename F, typename A0>
        struct function<F, A0>
            : proto::result_of::make_expr<
                proto::tag::function
              , phoenix_domain
              , F, A0>
        {};

        template <typename F, typename A0, typename A1>
        struct function<F, A0, A1>
            : proto::result_of::make_expr<
                proto::tag::function
              , phoenix_domain
              , F, A0, A1>
        {};

        template <typename F, typename A0, typename A1, typename A2>
        struct function<F, A0, A1, A2>
            : proto::result_of::make_expr<
                proto::tag::function
              , phoenix_domain
              , F, A0, A1, A2>
        {};

        // Bring in the rest
        #include <boost/phoenix/function/detail/function_result_of.hpp>
    }

    // functor which returns our lazy function call extension
    template<typename F>
    struct function
    {
        function() {}

        function(F f)
          : f(f)
        {}

        template <typename Sig>
        struct result;

        typename result_of::function<F>::type const
        operator()() const
        {
            return proto::make_expr<proto::tag::function, phoenix_domain>(f);
        }
        
        template <typename This, typename A0>
        struct result<This(A0 const&)>
            : result_of::function<F, A0>
        {};

        template <typename A0>
        typename result_of::function<F, A0>::type const
        operator()(A0 const& a0) const
        {
            return proto::make_expr<
                proto::tag::function, phoenix_domain>(f, a0);
        }

        template <typename A0, typename A1>
        typename result_of::function<F, A0, A1>::type const
        operator()(A0 const& a0, A1 const& a1) const
        {
            return proto::make_expr<
                proto::tag::function, phoenix_domain>(f, a0, a1);
        }

        template <typename A0, typename A1, typename A2>
        typename result_of::function<F, A0, A1, A2>::type const
        operator()(A0 const& a0, A1 const& a1, A2 const& a2) const
        {
            return proto::make_expr<
                proto::tag::function, phoenix_domain>(f, a0, a1, a2);
        }
        
        // Bring in the rest
        #include <boost/phoenix/function/detail/function_operator.hpp>

        F f;
    };

}

    template<typename F>
    struct result_of<phoenix::function<F>()>
      : phoenix::result_of::function<F>
    {};

}

#endif

