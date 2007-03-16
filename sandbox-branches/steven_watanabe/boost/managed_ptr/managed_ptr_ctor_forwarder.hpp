//macros to generate CTOR forwarding methods
#ifndef BOOST_MANAGED_PTR_PP_CTOR_FORWARDER_HPP_LJE20040327
#define BOOST_MANAGED_PTR_PP_CTOR_FORWARDER_HPP_LJE20040327
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/fold_right.hpp>
#include <boost/preprocessor/seq/rest_n.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>

#define MANAGED_PTR_CTOR_FORWARDER_MAX_ARGS 3
#define FORWARDER_TYPE_PARAM(z, n, _) \
        typename boost::mpl::at_c<VecOfTypes, n>::type BOOST_PP_CAT(a, n) \
        /**/
//Macro:
//  FORWARDER_TUPLE
//Purpose:
//  Generate a 1 element tuple containing a pair.
#define FORWARDER_TUPLE(s, arg_tuple, name) \
        (name arg_tuple) \
        /**/
#define FORWARDER_TRGT(z, arity, pass_signature, target_seq) \
        : \
        BOOST_PP_TUPLE_REM_CTOR( \
                1, \
                BOOST_PP_SEQ_FOLD_RIGHT( \
                        FORWARDER_TUPLE, \
                        ( BOOST_PP_IIF(pass_signature, signature BOOST_PP_ENUM_TRAILING_PARAMS_Z, \
                          BOOST_PP_ENUM_PARAMS_Z)(z, arity, a) \
                        ), \
                        target_seq \
                ) \
        ) { } \
        /**/
//Macro:
//  MANAGED_PTR_CTOR_FORWARDER_SRC_TRGT(z, arity, pass_signature, source, target_seq)
//
//  Where:
//  {  
//
//     z is the "next available repetition depth" as defined for the BOOST_PP_REPEAT
//       macro argument.
//
//     arity >= 0 and is the number of arguments passed to targetn (see target_seq)
//
//     pass_signature := 0 or 1 and is flag indicating whether targetn takes a signature.
//
//     source = a class name, this is the class which passes the arguments on.
//
//     target_seq :=
//       (target0)(target1)...(targetn)
//       where:
//       { n >= 1
//       }
//
//  }
//  
//Purpose:
//  Generate templated constructors with arity+1 arguments for source.
//  The first argument to CTOR specifies the types of the remaining arguments.
//  This 1st arg can be a boost::mpl::vector<T1,...Tarity> const&.
//
//Requires:
//    BOOST_PP_SEQ_SIZE(target_seq) >= 1
//
//Example1:
//  For arity=2, pass_signature=0, source=src, target_seq=(tgt0)(tgt1)(tgt2)
//  generated ctor is:
//
//    template<typename VecOfTypes>
//    src(VecOfTypes const& signature
//      , boost::mpl::at_c<VecOfTypes,0>::type a0
//      , boost::mpl::at_c<VecOfTypes,1>::type a1
//      )
//      :tgt0(tgt1(tgt2(a0,a1))
//    {}
//
//  If pass_signature=1, then the tgt line would be:
//
//      :tgt0(tgt1(tgt2(signature,a0,a1)))
//
#define FORWARDER_SRC_TRGT(z, arity, pass_signature, source, target_seq) \
        template<typename VecOfTypes> source( \
                VecOfTypes const& signature BOOST_PP_COMMA_IF(arity) \
                BOOST_PP_ENUM_ ## z(arity, FORWARDER_TYPE_PARAM, ~) \
        ) \
        FORWARDER_TRGT(z,  arity, pass_signature, target_seq) \
        /**/
#define FORWARDER_OVERLOAD_REMAP(z, arity, seq) \
        FORWARDER_SRC_TRGT(z, BOOST_PP_INC(arity), BOOST_PP_SEQ_ELEM(0, seq), \
          BOOST_PP_SEQ_ELEM(1, seq), BOOST_PP_SEQ_REST_N(2, seq) \
        ) \
        /**/
//Macro:
//  MANAGED_PTR_CTOR_FORWARDER_OVERLOAD(max_arity, source, target_seq, pass_signature)
//Purpose:
//  Invoke
//    MANAGED_PTR_CTOR_FORWARDER_SRC_TRGT(z, arity, name_seq, pass_signature) for arity=0,max_arity
//Acknowledgements:
//  Special thanks to Paul Mensonides who generously provided all this code
//  in response to:
//
//    http://article.gmane.org/gmane.comp.lib.boost.devel/35348
//
#define MANAGED_PTR_CTOR_FORWARDER_OVERLOAD(max_arity, source, target_seq, pass_signature ) \
        template<typename ZeroArgs>source( \
                boost::mpl::vector<ZeroArgs> const& signature \
        ) \
        FORWARDER_TRGT(2,  0, pass_signature, target_seq) \
        BOOST_PP_REPEAT(max_arity, FORWARDER_OVERLOAD_REMAP, (pass_signature)(source)target_seq ) \
        /**/
#endif
    
