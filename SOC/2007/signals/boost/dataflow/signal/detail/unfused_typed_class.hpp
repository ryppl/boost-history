/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
    Copyright (c) 2007 Stjepan Rajko
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#if !defined(BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_TYPED_CLASS_HPP_INCLUDED)
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
#include <boost/fusion/container/vector/vector.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/algorithm/transformation/pop_back.hpp>

#include <boost/fusion/functional/adapter/limits.hpp>
#include <boost/fusion/functional/adapter/detail/has_type.hpp>
#include <boost/fusion/functional/adapter/detail/nullary_call_base.hpp>

#include <boost/mpl/size.hpp>


namespace boost { namespace fusion
{

    template <class Function, class Sequence, typename Enable=void> class unfused_typed_class;

    //----- ---- --- -- - -  -   -

    #define  BOOST_PP_ITERATION_PARAMS_1 (3, (0,BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY, <boost/dataflow/signal/detail/unfused_typed_class.hpp>))
    #include BOOST_PP_ITERATE()
}}

#define BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_TYPED_CLASS_HPP_INCLUDED
#else // defined(BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////

#define OPERATOR_ARGS BOOST_PP_ITERATION()
#define CONSTRUCTOR_ARGS BOOST_PP_ITERATION()

        template <class Function, class Sequence>
        class unfused_typed_class<Function, Sequence,
            typename boost::enable_if<
                typename boost::mpl::equal_to<
                    typename boost::fusion::result_of::size<Sequence>::type,
                    typename boost::mpl::int_<OPERATOR_ARGS>::type >::type           
            >::type>
    : public Function
    {
        typedef typename result_of::as_vector<Sequence>::type carg_vector_t;
        typedef typename result_of::as_vector<Sequence>::type arg_vector_t;

    public:

        using Function::operator();

        inline explicit unfused_typed_class()
        { }

#define M(z,i,s)                                                                \
    typename result_of::value_at_c<s,i>::type a##i

#if CONSTRUCTOR_ARGS>0

        inline explicit unfused_typed_class(BOOST_PP_ENUM(CONSTRUCTOR_ARGS,M,carg_vector_t))
            : Function(carg_vector_t(BOOST_PP_ENUM_PARAMS(CONSTRUCTOR_ARGS,a)))
        { }

#endif

#if OPERATOR_ARGS>0
        inline typename Function::template result<arg_vector_t>::type
        operator()(BOOST_PP_ENUM(OPERATOR_ARGS,M,arg_vector_t)) const
        {
            arg_vector_t arg(BOOST_PP_ENUM_PARAMS(OPERATOR_ARGS,a));
            return Function::operator()(arg);
        }

#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400)
        inline typename Function::template result<arg_vector_t>::type 
        operator()(BOOST_PP_ENUM(OPERATOR_ARGS,M,arg_vector_t)) 
        {
            arg_vector_t arg(BOOST_PP_ENUM_PARAMS(OPERATOR_ARGS,a));
            return Function::operator()(arg);
        }
#endif

#endif

#undef M

    }; 
#undef CONSTRUCTOR_ARGS
#undef OPERATOR_ARGS

#endif // defined(BOOST_PP_IS_ITERATING)
#endif

