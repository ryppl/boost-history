/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
    Copyright (c) 2007 Stjepan Rajko
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#if !defined(BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_INHERITED_HPP_INCLUDED)
#if !defined(BOOST_PP_IS_ITERATING)

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/mpl/and.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/container/vector/vector.hpp>
#include <boost/fusion/sequence/conversion/as_vector.hpp>
#include <boost/fusion/algorithm/transformation/pop_back.hpp>

#include <boost/fusion/functional/adapter/limits.hpp>
#include <boost/fusion/functional/adapter/detail/has_type.hpp>
#include <boost/fusion/functional/adapter/detail/nullary_call_base.hpp>

#include <boost/mpl/size.hpp>


namespace boost { namespace fusion
{

    template <class Function, class CSequence, class Sequence, typename Enable=void> class unfused_inherited;

    //----- ---- --- -- - -  -   -

    #define  BOOST_PP_ITERATION_PARAMS_1 (3, (0,BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY, <boost/signal_network/detail/unfused_inherited.hpp>))
    #include BOOST_PP_ITERATE() 
}}

#define BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_INHERITED_HPP_INCLUDED
#else // defined(BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////

# if BOOST_PP_ITERATION_DEPTH()== 2

#define CONSTRUCTOR_ARGS BOOST_PP_FRAME_ITERATION(1)
#define OPERATOR_ARGS BOOST_PP_FRAME_ITERATION(2)

        template <class Function, class CSequence, class Sequence>
        class unfused_inherited<Function, CSequence, Sequence,
            typename boost::enable_if<
            boost::mpl::and_<
                boost::mpl::equal_to<
                    boost::fusion::result_of::size<CSequence>,
                    boost::mpl::int_<CONSTRUCTOR_ARGS> >,
                boost::mpl::equal_to<
                    boost::fusion::result_of::size<Sequence>,
                    boost::mpl::int_<OPERATOR_ARGS> >
            >  >::type >
    : public Function
    {
        typedef typename result_of::as_vector<CSequence>::type carg_vector_t;
        typedef typename result_of::as_vector<Sequence>::type arg_vector_t;

    public:
        using Function::operator();

#define M(z,i,s) \
    typename result_of::value_at_c<s,i>::type a##i
#define ADAPTED_ARGS(z,i,s) \
    typename detail::call_param<typename result_of::value_at_c<s,i>::type>::type a##i

        unfused_inherited(BOOST_PP_ENUM(CONSTRUCTOR_ARGS,ADAPTED_ARGS,carg_vector_t))
            : Function(BOOST_PP_ENUM_PARAMS(CONSTRUCTOR_ARGS,a))
        { }

#if OPERATOR_ARGS>0
        inline typename Function::template result<arg_vector_t>::type
        operator()(BOOST_PP_ENUM(OPERATOR_ARGS,M,arg_vector_t)) const
        {
            arg_vector_t arg(BOOST_PP_ENUM_PARAMS(OPERATOR_ARGS,a));
            return Function::operator()(arg);
        }

#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400)
        void//inline typename Function::template result<arg_vector_t>::type 
        operator()(BOOST_PP_ENUM(OPERATOR_ARGS,M,arg_vector_t)) 
        {
            arg_vector_t arg(BOOST_PP_ENUM_PARAMS(OPERATOR_ARGS,a));
            return Function::operator()(arg);
        }
#endif

#else
        inline typename Function::template result<arg_vector_t>::type
        operator()() const
        {
            arg_vector_t arg;
            return Function::operator()(arg);
        }

#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400)
        inline typename Function::template result<arg_vector_t>::type 
        operator()() 
        {
            arg_vector_t arg;
            return Function::operator()(arg);
        }
#endif

#endif

#undef M
#undef ADAPTED_ARGS

    };

#undef CONSTRUCTOR_ARGS
#undef OPERATOR_ARGS

#else

#define  BOOST_PP_ITERATION_PARAMS_2 (3, (0,BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY,<boost/signal_network/detail/unfused_inherited.hpp>))
#include BOOST_PP_ITERATE()

#endif

#endif // defined(BOOST_PP_IS_ITERATING)
#endif

