//  Boost compiler configuration selection header file  ------------------------------//

//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

// locate which compiler we are using and define BOOST_COMPILER_CONFIG as needed: 

#if defined __GNUC__
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/gcc.hpp>
#elif defined __ICC
//  Intel on Linux ---------------------------------------------------------
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/intel_icc.hpp>
#elif defined __KCC
//  Kai C++ ----------------------------------------------------------------
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/kai.hpp>
#elif defined __sgi
//  SGI MIPSpro C++ --------------------------------------------------------
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/sgi.hpp>
#elif defined __DECCXX
//  Compaq Tru64 Unix cxx ---------------------------------------------------
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/true64.hpp>
#elif defined __ghs
//  Greenhills C++ -----------------------------------------------------------//
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/greenhills.hpp>
#elif defined __BORLANDC__
//  Borland ------------------------------------------------------------------//
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/borland.hpp>
#elif defined __ICL
//  Intel on Windows --------------------------------------------------------//
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/intel_icl.hpp>
#elif defined  __MWERKS__
//  Metrowerks CodeWarrior  --------------------------------------------------//
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/metrowerks.hpp>
#elif defined  __SUNPRO_CC
//  Sun Workshop Compiler C++ ------------------------------------------------
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/sun.hpp>
#elif defined __HP_aCC
//  HP aCC -------------------------------------------------------------------
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/hp_acc.hpp>
#elif defined(__MRC__) || defined(__SC__)
//  MPW MrCpp or SCpp --------------------------------------------------------
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/mpw.hpp>
#elif defined _MSC_VER
//  Microsoft Visual C++ (excluding Intel/EDG front end)  --------------------
//
//  Must remain the last #elif since some other vendors (Metrowerks, for
//  example) also #define _MSC_VER
#   define BOOST_COMPILER_CONFIG <boost/config/compiler/visualc.hpp>
#endif // Microsoft (excluding Intel/EDG frontend)
