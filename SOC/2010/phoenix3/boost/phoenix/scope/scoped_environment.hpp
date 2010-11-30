/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_SCOPE_SCOPED_ENVIRONMENT_HPP
#define PHOENIX_SCOPE_SCOPED_ENVIRONMENT_HPP

#include <boost/mpl/int.hpp>
#include <boost/fusion/sequence/sequence_facade.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/phoenix/core/limits.hpp>

namespace boost { namespace phoenix
{
    template<typename Env, typename OuterEnv, typename Locals>
    struct scoped_environment
        : fusion::sequence_facade<
            scoped_environment<Env, OuterEnv, Locals>
          , fusion::random_access_traversal_tag
        >
    {
        typedef Env env_type;
        typedef OuterEnv outer_env_type;
        typedef Locals locals_type;

        scoped_environment(Env& env, OuterEnv& outer_env, Locals const& locals)
            : env(env)
            , outer_env(outer_env)
            , locals(locals)
        {}

        Env& env;
        OuterEnv& outer_env;
        Locals locals;
    
        #define PHOENIX_ADAPT_SCOPED_ENVIRONMENT(INTRINSIC)                     \
        template <typename Seq>                                                 \
        struct INTRINSIC                                                        \
        {                                                                       \
            typedef typename Seq::env_type env_type;                            \
            typedef typename fusion::result_of::INTRINSIC<env_type>::type type; \
                                                                                \
            static type call(Seq & seq)                                         \
            {                                                                   \
                return fusion::INTRINSIC(seq);                                  \
            }                                                                   \
        }                                                                       \
        /**/
        PHOENIX_ADAPT_SCOPED_ENVIRONMENT(begin);
        PHOENIX_ADAPT_SCOPED_ENVIRONMENT(end);
        PHOENIX_ADAPT_SCOPED_ENVIRONMENT(size);
        #undef PHOENIX_ADAPT_SCOPED_ENVIRONMENT
    
        template <typename Seq, typename N>
        struct value_at
        {
            typedef typename Seq::env_type env_type;
            typedef typename fusion::result_of::value_at<env_type, N>::type type;
        };
        
        template <typename Seq, typename N>
        struct at
        {
            typedef typename Seq::env_type env_type;
            typedef typename fusion::result_of::at<env_type, N>::type type;

            static type call(Seq & seq)
            {
                return fusion::at<N>(seq);
            }
        };
    };

    template <typename Env, typename Dummy = void>
    struct is_scoped_environment : mpl::false_ {};

    template <typename Env, typename OuterEnv, typename Locals>
    struct is_scoped_environment<scoped_environment<Env, OuterEnv, Locals> >
        : mpl::true_
    {};

    template <typename Env, typename OuterEnv, typename Locals>
    struct is_scoped_environment<scoped_environment<Env, OuterEnv, Locals> const>
        : mpl::true_
    {};
}}

#endif
