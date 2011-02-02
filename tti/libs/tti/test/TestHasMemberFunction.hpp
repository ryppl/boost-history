#if !defined(TEST_HAS_MEMBER_FUNCTION_HPP)
#define TEST_HAS_MEMBER_FUNCTION_HPP

#include "TestStructs.hpp"
#include <boost/tti/TTIntrospection.hpp>

TTI_HAS_MEMBER_FUNCTION(VoidFunction)
TTI_TRAIT_HAS_MEMBER_FUNCTION(FunctionReturningInt,IntFunction)
TTI_HAS_MEMBER_FUNCTION(aFunction)
TTI_TRAIT_HAS_MEMBER_FUNCTION(AnotherIntFunction,anotherFunction)
TTI_HAS_MEMBER_FUNCTION(sFunction)
TTI_HAS_MEMBER_FUNCTION(someFunctionMember)

#endif // TEST_HAS_MEMBER_FUNCTION_HPP
