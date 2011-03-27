# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2011.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_CONFIG_VARIADICS_HPP
# define BOOST_PREPROCESSOR_CONFIG_VARIADICS_HPP
#
# if defined(BOOST_PP_NO_VARIADICS)
/* This macro is purely for testing purposes so that we can 
   test Boost PP without variadic macro support even on 
   compilers that support variadic macros by defining
   BOOST_PP_NO_VARIADICS on the command line. We can remove 
   this after the intiial tests.
*/
# define BOOST_PP_VARIADICS 0
#
# else // !defined(BOOST_PP_NO_VARIADICS)
#
# if 1
/* This is the code we will use. It is essentially a duplication
   of the code in Boost Config. If we could ever use Boost Config
   in the future, we can change the above line to #if 0.
*/
#if defined(__GCCXML__)
// GCC-XML emulates other compilers, it has to appear first here!
# define BOOST_PP_VARIADICS 0
#
#elif defined __CUDACC__
//  NVIDIA CUDA C++ compiler for GPU
#
#elif defined __COMO__
//  Comeau C++
#if defined(__EDG_VERSION__) && (__EDG_VERSION__ < 400)
# define BOOST_PP_VARIADICS 0
#endif
#
#elif defined(__PATHSCALE__) && (__PATHCC__ >= 4)
// PathScale EKOPath compiler (has to come before clang and gcc)
#
#elif defined __clang__
//  Clang C++ emulates GCC, so it has to appear early.
#
#elif defined __DMC__
//  Digital Mars C++
# if (__DMC__ < 0x812)
# define BOOST_PP_VARIADICS 0
# endif
#
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
//  Intel
#if defined(__EDG_VERSION__) && (__EDG_VERSION__ < 400)
# define BOOST_PP_VARIADICS 0
#endif
#
# elif defined __GNUC__
//  GNU C++:
# if __GNUC__ < 3
# define BOOST_PP_VARIADICS 0
# endif
#
#elif defined __KCC
//  Kai C++
#if defined(__EDG_VERSION__) && (__EDG_VERSION__ < 400)
# define BOOST_PP_VARIADICS 0
#endif
#
#elif defined __sgi
//  SGI MIPSpro C++
#if defined(__EDG_VERSION__) && (__EDG_VERSION__ < 400)
# define BOOST_PP_VARIADICS 0
#endif
#
#elif defined __DECCXX
//  Compaq Tru64 Unix cxx
#if defined(__EDG_VERSION__) && (__EDG_VERSION__ < 400)
# define BOOST_PP_VARIADICS 0
#endif
#
#elif defined __ghs
//  Greenhills C++
#if defined(__EDG_VERSION__) && (__EDG_VERSION__ < 400)
# define BOOST_PP_VARIADICS 0
#endif
#
#elif defined __CODEGEARC__
//  CodeGear - must be checked for before Borland
#
#elif defined __BORLANDC__
//  Borland
# if __BORLANDC__ <= 0x0550
# define BOOST_PP_VARIADICS 0
# endif
#
#elif defined  __MWERKS__
//  Metrowerks CodeWarrior
# define BOOST_PP_VARIADICS 0
#
#elif defined  __SUNPRO_CC
//  Sun Workshop Compiler C++
# define BOOST_PP_VARIADICS 0
#
#elif defined __HP_aCC
//  HP aCC
#if !defined(__EDG__)
#if (__HP_aCC < 62500) || !defined(HP_CXX0x_SOURCE)
# define BOOST_PP_VARIADICS 0
#endif
#elif defined(__EDG_VERSION__) && (__EDG_VERSION__ < 400)
# define BOOST_PP_VARIADICS 0
#endif
#
#elif defined(__MRC__) || defined(__SC__)
//  MPW MrCpp or SCpp
# define BOOST_PP_VARIADICS 0
#
#elif defined(__IBMCPP__)
//  IBM Visual Age
#if ! __C99_MACRO_WITH_VA_ARGS
#  define BOOST_NO_VARIADIC_MACROS
#endif
#
#elif defined(__PGI)
//  Portland Group Inc.
# define BOOST_PP_VARIADICS 0
#
#elif defined _MSC_VER
//  Microsoft Visual C++
//
//  Must remain the last #elif since some other vendors (Metrowerks, for
//  example) also #define _MSC_VER
#if _MSC_VER < 1400
# define BOOST_PP_VARIADICS 0
#endif
#
#else
// Unrecognized compiler assumes no variadic macro support for safety.
# define BOOST_PP_VARIADICS 0
#endif
#
#if !defined(BOOST_PP_VARIADICS)
#define BOOST_PP_VARIADICS 1
#endif
#
# else // !1
/* This is the ideal code, but will not work until Boost Config 
   works with C compilers or C++ compilers in C mode. I will be 
   working on that to see if I can make it happen in the future.
*/
# include <boost/config.hpp>
#
# if defined(BOOST_NO_VARIADIC_MACROS)
# define BOOST_PP_VARIADICS 0
# else
# define BOOST_PP_VARIADICS 1
# endif // !BOOST_NO_VARIADIC_MACROS
#
# endif // 1
#
# endif // BOOST_PP_NO_VARIADICS
#
# endif // BOOST_PREPROCESSOR_CONFIG_VARIADICS_HPP
