/*=============================================================================
    Copyright (c) 2005-2006 João Abecasis
    Copyright (c) 2006-2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#if !defined(BOOST_FUSION_FUNCTIONAL_INVOCATION_INVOKE_FUNCTION_OBJECT_HPP_INCLUDED)
#if !defined(BOOST_PP_IS_ITERATING)

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#include <boost/blank.hpp>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/functional/invocation/limits.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <class Function, class Sequence> struct invoke_function_object;
    }

    template <class Function, class Sequence>
    inline typename result_of::invoke_function_object<Function, Sequence>::type
    invoke_function_object(Function, Sequence &);

    template <class Function, class Sequence>
    inline typename result_of::invoke_function_object<Function, Sequence const
        >::type invoke_function_object(Function, Sequence const &);

    //----- ---- --- -- - -  -   -

    namespace detail
    {
        template< 
            class Function, class Sequence, 
            int N = result_of::size<Sequence>::value,
            bool RandomAccess = boost::is_convertible< 
                    typename traits::category_of<Sequence>::type,
                    fusion::random_access_traversal_tag 
                >::value 
            >
        struct invoke_function_object_impl
        {
            typedef boost::blank result;
        };

        template <class Sequence, int N> 
        struct invoke_function_object_param_types;

        #define  BOOST_PP_FILENAME_1 \
            <boost/fusion/functional/invocation/invoke_function_object.hpp>
        #define  BOOST_PP_ITERATION_LIMITS \
            (0, BOOST_FUSION_INVOKE_FUNCTION_OBJECT_MAX_ARITY)
        #include BOOST_PP_ITERATE()
    }

    namespace result_of
    {
        template <class Function, class Sequence> struct invoke_function_object
            : detail::invoke_function_object_impl< 
                typename boost::remove_reference<Function>::type, Sequence
              >::template result<>
        { }; 
    }

    template <class Function, class Sequence>
    inline typename result_of::invoke_function_object<Function,Sequence>::type
    invoke_function_object(Function f, Sequence & s)
    {
        return detail::invoke_function_object_impl<
                typename boost::remove_reference<Function>::type,Sequence
            >::call(f,s);
    }

    template <class Function, class Sequence>
    inline typename result_of::invoke_function_object<Function,Sequence const>::type
    invoke_function_object(Function f, Sequence const & s)
    {
        return detail::invoke_function_object_impl<
                typename boost::remove_reference<Function>::type,Sequence const
            >::call(f,s);
    }

}}

#define BOOST_FUSION_FUNCTIONAL_INVOCATION_INVOKE_FUNCTION_OBJECT_HPP_INCLUDED
#else // defined(BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#define N BOOST_PP_ITERATION()

        template <class Function, class Sequence>
        struct invoke_function_object_impl<Function,Sequence,N,true>
        {
        public:

            template <typename _ = void>
            struct result
                : Function::template result <
#define M(z,j,data)                                                             \
    typename boost::remove_reference<                                          \
        typename result_of::value_at_c<Sequence,j>::type >::type 
                    BOOST_PP_ENUM(N,M,~) >
#undef M
            { }; 

            template <class F>
            static inline typename result<F>::type
            call(F & f, Sequence & s)
            {
#define M(z,j,data) fusion::at_c<j>(s)
                return f( BOOST_PP_ENUM(N,M,~) );
#undef M
            }
        };

        template <class Function, class Sequence>
        struct invoke_function_object_impl<Function,Sequence,N,false>
        {
        private:
            typedef invoke_function_object_param_types<Sequence,N> seq;
        public:

            template <typename _ = void>
            struct result
                : Function::template result < 
                    BOOST_PP_ENUM_PARAMS(N,typename seq::T) >
            { }; 

            template <class F>
            static inline typename result<F>::type
            call(F & f, Sequence & s)
            {
#if N > 0
                typename seq::I0 i0 = fusion::begin(s);
#define M(z,j,data)                                                             \
            typename seq::I##j i##j =                                          \
                fusion::next(BOOST_PP_CAT(i,BOOST_PP_DEC(j)));
                BOOST_PP_REPEAT_FROM_TO(1,N,M,~)
#undef M
#endif
                return f( BOOST_PP_ENUM_PARAMS(N,*i) );
            }
        };

        template <class Sequence> 
        struct invoke_function_object_param_types<Sequence,N>
        {
#if N > 0
            typedef typename result_of::begin<Sequence>::type I0;
            typedef typename boost::remove_reference< 
                typename result_of::value_of<I0>::type >::type T0;

#define M(z,i,data)                                                             \
            typedef typename result_of::next<                                  \
                BOOST_PP_CAT(I,BOOST_PP_DEC(i))>::type I##i;                   \
            typedef typename boost::remove_reference<                          \
                typename result_of::value_of<I##i>::type >::type T##i;

            BOOST_PP_REPEAT_FROM_TO(1,N,M,~)
#undef M
#endif
        };

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)
#endif 

