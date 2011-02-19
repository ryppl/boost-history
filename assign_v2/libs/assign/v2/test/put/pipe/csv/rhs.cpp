//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <boost/preprocessor/expr_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/check/equal_val.hpp> 
#include <boost/assign/v2/detail/check/equal_ref.hpp>
#include <boost/assign/v2/put/pipe/csv.hpp>

#include <libs/assign/v2/test/put/pipe/csv/rhs.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_csv{
namespace xxx_rhs{

    template<int i, typename T, typename P, typename U>
    void xxx_assert(T& rhs, P const& pred, U const& u)
    {
        // TODO name lookup?
        using namespace boost;
        using namespace boost::assign::v2;
        
        pred( rhs.seq_args()[ i ].get(), u );
    }

    void test(){
    
        namespace as2 = boost::assign::v2;
        typedef boost::mpl::int_<0> zero_;
        typedef int const cint_;
        cint_ x = -1, y = 0, z = 1; 
        int x1 = x + 5, y1 = y + 5, z1 = z + 5;

        namespace chk = as2::check_aux;
        typedef chk::equal_ref r_;
        typedef chk::equal_val v_;

        {
            BOOST_AUTO( rhs, ( as2::_csv_put( x1, y1, z1 ) ) );
            xxx_assert<0>(rhs, r_(), x1);
            xxx_assert<1>(rhs, r_(), y1);
            xxx_assert<2>(rhs, r_(), z1);
        }
        {
            BOOST_AUTO( rhs, ( as2::_csv_put( x1, y1, z ) ) );
            xxx_assert<0>(rhs, r_(), x1);
            xxx_assert<1>(rhs, r_(), y1);
            xxx_assert<2>(rhs, r_(), z);
        }
#define Z -10
        {
            BOOST_AUTO( rhs, ( as2::_csv_put( x1, y, Z ) ) );
            xxx_assert<0>(rhs, r_(), x1);
            xxx_assert<1>(rhs, r_(), y);
            xxx_assert<2>(rhs, v_(), Z);
        }

#undef Z

    }

}// xxx_rhs
}// xxx_csv
}// xxx_pipe
}// xxx_put
}// xxx_test_assign

