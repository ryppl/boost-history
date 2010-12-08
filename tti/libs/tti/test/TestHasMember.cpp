#include "TestHasMember.hpp"
#include <boost/detail/lightweight_test.hpp>

int main()
  {
  
  BOOST_TEST(tti::has_member_AnInt<int AType::*>::value);
  BOOST_TEST(tti::has_member_AnInt<long AnotherType::*>::value);
  BOOST_TEST(tti::has_member_VoidFunction<void (AType::*)()>::value);
  BOOST_TEST(tti::has_member_IntFunction<int (AType::*)()>::value);
  BOOST_TEST(tti::has_member_IntFunction<double (AnotherType::*)(int)>::value);
  BOOST_TEST(tti::has_member_aFunction<AType (AnotherType::*)(int)>::value);
  BOOST_TEST(tti::has_member_anotherFunction<int (AnotherType::*)(AType)>::value);
  BOOST_TEST(tti::has_member_sFunction<AType::AnIntType (AnotherType::*)(int,long,double)>::value);
  BOOST_TEST(tti::has_member_aMember<bool AnotherType::*>::value);
  BOOST_TEST(!tti::has_member_aMember<int AnotherType::*>::value);
  BOOST_TEST(tti::has_member_cMem<bool AnotherType::*>::value);
  BOOST_TEST(!tti::has_member_someDataMember<short AType::*>::value);
  BOOST_TEST(!tti::has_member_someFunctionMember<AType (AnotherType::*)(long,int)>::value);
  
  return boost::report_errors();

  }
