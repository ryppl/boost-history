/*=============================================================================
    Copyright (c) 2001-2009 Joel de Guzman
    Copyright (c) 2001-2009 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_WRAP_ACTION_APR_19_2008_0103PM)
#define BOOST_SPIRIT_WRAP_ACTION_APR_19_2008_0103PM

#include <boost/spirit/home/phoenix/core/actor.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/scope.hpp>
#include <boost/spirit/home/support/attributes.hpp>
#include <boost/spirit/home/lex/lexer/pass_flags.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit { namespace lex { namespace lexertl 
{ 
    namespace detail
    {
        template <typename FunctionType, typename Iterator, typename Context
          , typename IdType>
        struct wrap_action
        {
            // plain functions with 5 arguments and function objects are not 
            // touched at all
            template <typename F>
            static FunctionType call(F const& f)
            {
                return f;
            }

            // wrap phoenix actor
            struct phoenix_action
            {
                template <typename F, typename T1, typename T2, typename T3
                  , typename T4, typename T5>
                struct result { typedef void type; };

                template <typename Eval>
                void operator()(phoenix::actor<Eval> const& f, Iterator& start
                  , Iterator& end, BOOST_SCOPED_ENUM(pass_flags)& pass
                  , IdType& id, Context& ctx) const
                {
                    f (start, end, pass, id, ctx);
                }
            };

            template <typename Eval>
            static FunctionType call(phoenix::actor<Eval> const& f)
            {
                using phoenix::arg_names::_1;
                using phoenix::arg_names::_2;
                using phoenix::arg_names::_3;
                using phoenix::arg_names::_4;
                using phoenix::arg_names::_5;
                return phoenix::bind(phoenix_action(), phoenix::lambda[f], 
                    _1, _2, _3, _4, _5);
            }

            // semantic actions with 4 arguments
            template <typename F>
            static void arg4_action(F* f, Iterator& start, Iterator& end
              , BOOST_SCOPED_ENUM(pass_flags)& pass, IdType& id
              , Context const&)
            {
                f(start, end, pass, id);
            }

            template <typename F>
            static void arg4_action_bool(F* f, Iterator& start, Iterator& end
              , BOOST_SCOPED_ENUM(pass_flags)& pass, IdType& id, Context const&)
            {
                bool pass_bool = true;
                f (start, end, pass_bool, id);
                pass = pass_bool ? pass_flags::pass_normal : pass_flags::pass_fail;
            }

            template <typename A0, typename A1, typename A2, typename A3>
            static FunctionType call(void(*f)(A0, A1, A2, A3))
            {
                void (*pf)(void(*)(A0, A1, A2, A3)
                  , Iterator&, Iterator&, BOOST_SCOPED_ENUM(pass_flags)&
                  , IdType&, Context const&) = &wrap_action::arg4_action;

                using phoenix::arg_names::_1;
                using phoenix::arg_names::_2;
                using phoenix::arg_names::_3;
                using phoenix::arg_names::_4;
                return phoenix::bind(pf, f, _1, _2, _3, _4);
            }

            template <typename A0, typename A1, typename A3>
            static FunctionType call(void(*f)(A0, A1, bool&, A3))
            {
                void (*pf)(void(*)(A0, A1, bool&, A3), Iterator&, Iterator&
                  , BOOST_SCOPED_ENUM(pass_flags)&, IdType&, Context const&) 
                      = &wrap_action::arg4_action_bool;

                using phoenix::arg_names::_1;
                using phoenix::arg_names::_2;
                using phoenix::arg_names::_3;
                using phoenix::arg_names::_4;
                return phoenix::bind(pf, f, _1, _2, _3, _4);
            }

            // semantic actions with 3 arguments
            template <typename F>
            static void arg3_action(F* f, Iterator& start, Iterator& end
              , BOOST_SCOPED_ENUM(pass_flags)& pass, IdType
              , Context const&)
            {
                f(start, end, pass);
            }

            template <typename F>
            static void arg3_action_bool(F* f, Iterator& start, Iterator& end
              , BOOST_SCOPED_ENUM(pass_flags)& pass, IdType, Context const&)
            {
                bool pass_bool = true;
                f (start, end, pass_bool);
                pass = pass_bool ? pass_flags::pass_normal : pass_flags::pass_fail;
            }

            template <typename A0, typename A1, typename A2>
            static FunctionType call(void(*f)(A0, A1, A2))
            {
                void (*pf)(void(*)(A0, A1, A2), Iterator&, Iterator&
                  , BOOST_SCOPED_ENUM(pass_flags)&, IdType
                  , Context const&) = &wrap_action::arg3_action;

                using phoenix::arg_names::_1;
                using phoenix::arg_names::_2;
                using phoenix::arg_names::_3;
                return phoenix::bind(pf, f, _1, _2, _3);
            }

            template <typename A0, typename A1>
            static FunctionType call(void(*f)(A0, A1, bool&))
            {
                void (*pf)(void(*)(A0, A1, bool&), Iterator&, Iterator&
                  , BOOST_SCOPED_ENUM(pass_flags)&, IdType, Context const&) 
                      = &wrap_action::arg3_action_bool;

                using phoenix::arg_names::_1;
                using phoenix::arg_names::_2;
                using phoenix::arg_names::_3;
                return phoenix::bind(pf, f, _1, _2, _3);
            }

            // semantic actions with 2 arguments
            template <typename F>
            static void arg2_action(F* f, Iterator& start, Iterator& end
              , bool, IdType, Context const&)
            {
                f(start, end);
            }

            template <typename A0, typename A1>
            static FunctionType call(void(*f)(A0, A1))
            {
                void (*pf)(void(*)(A0, A1), Iterator&, Iterator&, bool
                  , IdType, Context const&) = &wrap_action::arg2_action;

                using phoenix::arg_names::_1;
                using phoenix::arg_names::_2;
                return phoenix::bind(pf, f, _1, _2);
            }

            // we assume that either both iterators are to be passed to the 
            // semantic action or none iterator at all (i.e. it's not possible 
            // to have a lexer semantic action function taking one arguments).

            // semantic actions with 0 argument
            template <typename F>
            static void arg0_action(F* f, Iterator&, Iterator&
              , bool, IdType, Context const&)
            {
                f();
            }

            static FunctionType call(void(*f)())
            {
                void (*pf)(void(*)(), Iterator&, Iterator&, bool
                  , IdType, Context const&) = &arg0_action;

                return phoenix::bind(pf, f);
            }
        };

        // specialization allowing to skip wrapping for lexer types not 
        // supporting semantic actions
        template <typename Iterator, typename Context, typename Idtype>
        struct wrap_action<unused_type, Iterator, Context, Idtype>
        {
            // plain functors are not touched at all
            template <typename F>
            static F const& call(F const& f)
            {
                return f;
            }
        };
    }

}}}}

#endif
