#include "TestHasMember.hpp"
#include <boost/mpl/assert.hpp>

int main()
  {
  
  // Wrong function signature for sFunction
  
  BOOST_MPL_ASSERT(( tti::has_member_sFunction<AType::AnIntType (AnotherType::*)(short,long,double)> ));
  
  return 0;

  }
