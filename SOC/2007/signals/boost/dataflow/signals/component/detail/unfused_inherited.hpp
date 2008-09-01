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
#include <boost/preprocessor/punctuation/paren_if.hpp>

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/mpl/and.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/container/vector/vector.hpp>
#include <boost/fusion/include/as_vector.hpp>

#include <boost/fusion/functional/adapter/limits.hpp>

#include <boost/mpl/size.hpp>
#include <boost/utility/result_of.hpp>

namespace boost { namespace fusion
{

    template <class Function, class Sequence, typename Enable=void> class unfused_inherited;

    //----- ---- --- -- - -  -   -

    #define  BOOST_PP_ITERATION_PARAMS_1 (3, (0,BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY, <boost/dataflow/signals/component/detail/unfused_inherited.hpp>))
    #include BOOST_PP_ITERATE() 
}}

#define BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_INHERITED_HPP_INCLUDED
#else // defined(BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////

#define OPERATOR_ARGS BOOST_PP_ITERATION()

    template <class Function, class Sequence>
    class unfused_inherited<Function, Sequence,
        typename boost::enable_if<
            boost::mpl::equal_to<
                boost::fusion::result_of::size<Sequence>,
                boost::mpl::int_<OPERATOR_ARGS> >
        >::type >
    : public Function
    {
    protected:
        typedef Function base_type;
        typedef typename result_of::as_vector<Sequence>::type arg_vector_t;

    public:
        using Function::operator();

#define M(z,i,s) \
        typename result_of::value_at_c<s,i>::type a##i
#define MT(z,i,s) \
        typename result_of::value_at_c<s,i>::type

        unfused_inherited()
        { }

        template<typename T1>
        unfused_inherited(const T1 &t1)
            : Function(t1)
        { }
        
        template<typename T1>
            unfused_inherited(T1 &t1)
            : Function(t1)
        { }

        template<typename T1, typename T2>
            unfused_inherited(const T1 &t1, const T2 &t2)
            : Function(t1, t2)
        { }

        template<typename T1, typename T2>
            unfused_inherited(T1 &t1, T2 &t2)
            : Function(t1, t2)
        { }
        
        template<typename F>
        struct result
            : public Function::template result<F> {};
        
        template<typename F>
        struct result<F(BOOST_PP_ENUM(OPERATOR_ARGS,MT,arg_vector_t))>
        {
            typedef typename boost::result_of<Function(const arg_vector_t &)>::type type;
        };

        inline typename boost::result_of<Function(const arg_vector_t &)>::type
        operator()(BOOST_PP_ENUM(OPERATOR_ARGS,M,arg_vector_t)) const
        {
            arg_vector_t arg
#if OPERATOR_ARGS>0
            (BOOST_PP_ENUM_PARAMS(OPERATOR_ARGS,a))
#endif
                ;
            return Function::operator()(arg);
        }

#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400)
        inline typename boost::result_of<Function(const arg_vector_t &)>::type
        operator()(BOOST_PP_ENUM(OPERATOR_ARGS,M,arg_vector_t)) 
        {
            arg_vector_t arg
#if OPERATOR_ARGS>0
            (BOOST_PP_ENUM_PARAMS(OPERATOR_ARGS,a))
#endif
                ;
            return Function::operator()(arg);
        }
#endif

#undef M
#undef MT

    };

#undef OPERATOR_ARGS

#endif // defined(BOOST_PP_IS_ITERATING)
#endif

