/*=============================================================================
    Copyright (c) 2005-2006 João Abecasis
    Copyright (c) 2006-2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#if !defined(BOOST_FUSION_FUNCTIONAL_INVOCATION_INVOKE_HPP_INCLUDED)
#if !defined(BOOST_PP_IS_ITERATING)

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_shifted.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/blank.hpp>

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_convertible.hpp>

#include <boost/function_types/is_function.hpp>
#include <boost/function_types/is_callable_builtin.hpp>
#include <boost/function_types/is_member_function_pointer.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>

#include <boost/utility/result_of.hpp>

#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/functional/invocation/limits.hpp>
#include <boost/fusion/functional/invocation/detail/that_ptr.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Function, class Sequence> struct invoke;
    }

    template <typename Function, class Sequence>
    inline typename result_of::invoke<Function, Sequence>::type
    invoke(Function, Sequence &);

    template <typename Function, class Sequence>
    inline typename result_of::invoke<Function, Sequence const>::type
    invoke(Function, Sequence const &);

    //----- ---- --- -- - -  -   -

    namespace detail
    {
        namespace ft = function_types;

        template< 
            typename Function, class Sequence, 
            int N = result_of::size<Sequence>::value,
            bool CBI = ft::is_callable_builtin<Function>::value,
            bool MFP = ft::is_member_function_pointer<Function>::value, 
            bool RandomAccess = boost::is_convertible< 
                    typename traits::category_of<Sequence>::type,
                    fusion::random_access_traversal_tag 
                >::value 
            >
        struct invoke_impl
        {
            typedef boost::blank result;
        };

        template <class Sequence, int N> struct invoke_param_types;

        template <typename Func, class N, bool CBI = true, typename Sig = void>
        // contains type member with the result, empty on error
        struct invoke_result
            : mpl::if_< 
                  mpl::or_< 
                      mpl::equal_to< ft::function_arity<Func>, N >,
                      mpl::and_< ft::is_callable_builtin<Func, ft::variadic>,
                          mpl::less< ft::function_arity<Func>, N > >
                  >, ft::result_type<Func>, boost::blank
              >::type
        { }; 
        template <typename Func, class N, typename Sig>
        struct invoke_result<Func,N,false,Sig>
            : boost::result_of<Sig>
        { };

        // Transform for F so that boost::result_of< F(...) > works
        template <typename F> struct invoke_result_of_prep
            : mpl::if_< ft::is_function<F>, boost::add_reference<F>, 
                boost::remove_cv<F> 
              >::type
        { };

        #define  BOOST_PP_FILENAME_1 <boost/fusion/functional/invocation/invoke.hpp>
        #define  BOOST_PP_ITERATION_LIMITS (0, BOOST_FUSION_INVOKE_MAX_ARITY)
        #include BOOST_PP_ITERATE()

        template <typename T, class C, class Sequence>
        struct invoke_data_member
        {
        private:

            typedef typename result_of::front<Sequence>::type that;

            typedef mpl::or_< is_convertible<that,C*>,
                              is_convertible<that,C&>,
                              non_const_pointee<that> > non_const_cond;

            typedef typename mpl::eval_if< non_const_cond,
                mpl::identity<C>, add_const<C> >::type qualified_class;

            typedef typename mpl::eval_if< non_const_cond,
                mpl::identity<T>, add_const<T> >::type qualified_type;

        public:

            template <typename _ = void>
            struct result
                : boost::add_reference<qualified_type>
            { }; 

            static inline typename result<>::type call(T C::* f, Sequence & s)
            {
                typename result_of::front<Sequence>::type c = fusion::front(s);
                return that_ptr<qualified_class>::get(c)->*f;
            }
        };

        template <typename T, class C, class Sequence>
        struct invoke_impl<T C::*, Sequence, 1, true, false, false>
            : detail::invoke_data_member<T, C, Sequence>
        { };

        template <typename T, class C, class Sequence>
        struct invoke_impl<T C::*, Sequence, 1, true, false, true>
            : detail::invoke_data_member<T, C, Sequence>
        { };

    }

    namespace result_of
    {
        template <typename Function, class Sequence> struct invoke
            : detail::invoke_impl< 
                typename boost::remove_reference<Function>::type, Sequence
              >::template result<>
        { }; 
    }

    template <typename Function, class Sequence>
    inline typename result_of::invoke<Function,Sequence>::type
    invoke(Function f, Sequence & s)
    {
        return detail::invoke_impl<
                typename boost::remove_reference<Function>::type,Sequence
            >::call(f,s);
    }

    template <typename Function, class Sequence>
    inline typename result_of::invoke<Function,Sequence const>::type
    invoke(Function f, Sequence const & s)
    {
        return detail::invoke_impl<
                typename boost::remove_reference<Function>::type,Sequence const
            >::call(f,s);
    }

}}

#define BOOST_FUSION_FUNCTIONAL_INVOCATION_INVOKE_HPP_INCLUDED
#else // defined(BOOST_PP_IS_ITERATING)
///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#define N BOOST_PP_ITERATION()

        template <typename Function, class Sequence, bool CBI>
        struct invoke_impl<Function,Sequence,N,CBI,false,true>
        {
        private:
            typedef typename invoke_result_of_prep<Function>::type func;
        public:

            template <typename _ = void>
            struct result
                : invoke_result< Function, mpl::size_t<N>, CBI,
#define M(z,j,data) typename result_of::at_c<Sequence,j>::type
                    func(BOOST_PP_ENUM(N,M,~)) >
#undef M
            { }; 

            template <typename F>
            static inline typename result<F>::type
            call(F & f, Sequence & s)
            {
#define M(z,j,data) fusion::at_c<j>(s)
                return f( BOOST_PP_ENUM(N,M,~) );
            }
        };

#if N > 0
        template <typename Function, class Sequence>
        struct invoke_impl<Function,Sequence,N,true,true,true>
        {
        public:

            template <typename _ = void>
            struct result
                : invoke_result< Function, mpl::size_t<N> >
            { }; 

            template <typename F>
            static inline typename result<F>::type
            call(F & f, Sequence & s)
            {
                return (that_ptr<typename mpl::front<
                                ft::parameter_types<Function> >::type
                    >::get(fusion::at_c<0>(s))->*f)(BOOST_PP_ENUM_SHIFTED(N,M,~));
            }
        };
#endif

#undef M

#define M(z,j,data)                                                             \
            typename seq::I##j i##j =                                          \
                fusion::next(BOOST_PP_CAT(i,BOOST_PP_DEC(j)));

        template <typename Function, class Sequence, bool CBI>
        struct invoke_impl<Function,Sequence,N,CBI,false,false>
        {
        private:
            typedef typename invoke_result_of_prep<Function>::type func;
            typedef invoke_param_types<Sequence,N> seq;
        public:

            template <typename _ = void>
            struct result
                : invoke_result< Function, mpl::size_t<N>, CBI, 
                    func(BOOST_PP_ENUM_PARAMS(N,typename seq::T)) >
            { }; 

            template <typename F>
            static inline typename result<F>::type
            call(F & f, Sequence & s)
            {
#if N > 0
                typename seq::I0 i0 = fusion::begin(s);
                BOOST_PP_REPEAT_FROM_TO(1,N,M,~)
#endif
                return f( BOOST_PP_ENUM_PARAMS(N,*i) );
            }
        };

#if N > 0
        template <typename Function, class Sequence>
        struct invoke_impl<Function,Sequence,N,true,true,false>
        {
        private:
            typedef invoke_param_types<Sequence,N> seq;
        public:

            template <typename _ = void>
            struct result
                : invoke_result< Function, mpl::size_t<N> >
            { }; 

            template <typename F>
            static inline typename result<F>::type
            call(F & f, Sequence & s)
            {
                typename seq::I0 i0 = fusion::begin(s);
                BOOST_PP_REPEAT_FROM_TO(1,N,M,~)

                return (that_ptr< typename mpl::front< 
                                      ft::parameter_types<Function> >::type
                    >::get(*i0)->*f)(BOOST_PP_ENUM_SHIFTED_PARAMS(N,*i));
            }
        };
#endif

#undef M

        template <class Sequence> struct invoke_param_types<Sequence,N>
        {
#if N > 0
            typedef typename result_of::begin<Sequence>::type I0;
            typedef typename result_of::deref<I0>::type T0;

#define M(z,i,data)                                                             \
            typedef typename result_of::next<                                  \
                BOOST_PP_CAT(I,BOOST_PP_DEC(i))>::type I##i;                   \
            typedef typename result_of::deref<I##i>::type T##i;

            BOOST_PP_REPEAT_FROM_TO(1,N,M,~)
#undef M
#endif
        };


#undef N
#endif // defined(BOOST_PP_IS_ITERATING)
#endif 

