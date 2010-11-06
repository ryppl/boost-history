/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

//[ no_auto_function_multi_cast
#include <utility> // For std::forward

// The terminating case that returns the argument itself when no explicit
// target type is provided
template< class SourceType >
SourceType&& multi_cast( SourceType&& source )
{
  return source;
}

// The primary function template. It works by using a static_cast on the result
// of another multi_cast call with the final target type removed
template< class    TargetType
        , class... IntermediateTypes
        , class    SourceType
        >
TargetType multi_cast( SourceType&& source )
{
  return static_cast< TargetType >
         ( multi_cast< IntermediateTypes... >
           ( std::forward< SourceType >( source ) )
         );
}
//`The function templates themselves are fairly simple and the lack of automatic
//`return type deduction is not a huge problem since the return types are easily
//`specified based on the template parameters, but there is at least one
//`unfortunate drawback of this implementation. If a programmer attempts to use
//`this template with a type or types which would not form a valid `static_cast`
//`operation, the implementation will blow up in his or her face, with one or
//`more errors somewhere nested inside of templates that they themselves did not
//`write. Let's see exactly what types of error messages one might expect from
//`misuse.

// Base types
struct left {};
struct right {};

// A derived type that inherits from both "left" and "right"
struct child : left, right {};

// A type unrelated to the above three types
struct unrelated_type {};

int main()
{
  // Create an instance of the derived type
  child child_;

  // Refer to the instance through its base type "left"
  left& left_ = child_;

  // The following line successfully cross-casts from "left_" to a reference to
  // the object through the type "right"
  right& right_ = multi_cast< right&, child& >( left_ );

  // *Kaboom* This line will cause the compiler to generate some very scary
  // looking messages in the build log.
  right& error_ = multi_cast< right&, unrelated_type&, child& >( left_ );
}
//]
