
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

/** @file
 * @brief Configuration macros allow to change the behaviour of this library
 *  at compile-time.
 */

#ifndef BOOST_LOCAL_CONFIG_HPP_
#define BOOST_LOCAL_CONFIG_HPP_

/**
 * @def BOOST_LOCAL_CONFIG_COMPLIANT
 * @brief Force to use ISO C++ standard features only.
 *
 * If programmers leave this configuration macro undefined, its default
 * value is to be left not defined.
 *
 * If this macro is defined, variadic macros and empty macro parameters are not
 * used by this library. Using variadic macros and empty macro parameters
 * allows this library to provide the <em>variadic macro</em> and <em>empty
 * macro</em> syntaxes which some programmers might find more readable than the
 * <em>sequencing macro</em> syntax (see
 * @RefMacro{BOOST_LOCAL_FUNCTION_PARAMS}, @RefMacro{BOOST_LOCAL_BLOCK}, and
 * @RefMacro{BOOST_LOCAL_EXIT}). If this configuration macro is defined then
 * only the sequencing macro syntax is allowed (regardless of whether the
 * compiler supports variadic and empty macros or not).
 *
 * @Warning The variadic and empty macro syntaxes are not supported by all C++
 *  compilers so they should be used with care to avoid portability issues
 *  (and this configuration macro can be defined to disable them).
 *
 * Variadic macros, are supported by most recent compilers (like MSVC and
 * GCC) and they were first introduced by the C99 standard but they are not
 * part of the official ISO C++ standard. Empty macro parameters are also
 * supported by the C99 standards but they are not supported by all modern
 * compilers (for example, they are not supported by MSVC).
 * 
 * 
 * @See @RefSect{Tutorial} section, @RefSect2{Getting, Started} section.
 */
#ifndef BOOST_LOCAL_CONFIG_COMPLIANT
#undef BOOST_LOCAL_CONFIG_COMPLIANT
#else
#define BOOST_LOCAL_CONFIG_COMPLIANT
#endif

/**
 * @def BOOST_LOCAL_CONFIG_FUNCTION_ARITY_MAX
 * @brief Maximum number of function parameters supported for the local
 *  functions.
 *
 * If programmers leave this configuration macro undefined, its default
 * value is <c>5</c>.
 * 
 * This only refers to the number of local function parameters and not to the
 * number of bound variables in scope (the limit on the number of bound
 * variables is instead the maximum size allowed for a Boost.Preprocessor
 * sequences).
 *
 * @Warning Increasing this number will increase compilation time.
 *
 * @See @RefSect2{Getting, Starting} section.
 */
#ifndef BOOST_LOCAL_CONFIG_FUNCTION_ARITY_MAX
#define BOOST_LOCAL_CONFIG_FUNCTION_ARITY_MAX 5
#endif

/**
 * @def BOOST_LOCAL_CONFIG_THIS_PARAM_NAME
 * @brief The name of the special symbol used to access the bound object
 * <c>this</c>.
 *
 * If programmers leave this configuration macro undefined, the default
 * symbol used is <c>this_</c>.
 *
 * @Warning Programmers should not #define this macro unless it is absolutely
 *  necessary (e.g., to avoid name clashes with another library which cannot be
 *  changed). Changing the symbol <c>this_</c> effectively changes the public
 *  API of this library.
 *
 * @See @RefSect2{Getting, Started} section.
 */
#ifndef BOOST_LOCAL_CONFIG_THIS_PARAM_NAME
#define BOOST_LOCAL_CONFIG_THIS_PARAM_NAME this_
#endif

#endif // #include guard

