//object_tracked with DTOR check for double destruction (death)
#ifndef LIBS_MANAGED_PTR_TEST_CHK_DBL_DELETE_HPP_LJE20040129
#define LIBS_MANAGED_PTR_TEST_CHK_DBL_DELETE_HPP_LJE20040129
//Requires:
//  #define of BOOST_CHECK
#include "boost/utility/object_tracked.hpp"
struct chk_dbl_die
: public utility::object_tracked
{
    ~chk_dbl_die(void)
    {
        BOOST_CHECK(am_i_live());
    }
};
#endif
