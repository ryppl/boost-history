////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/test/put/deque/csv.h>
#include <libs/assign/v2/test/put/deque/functor.h>
#include <libs/assign/v2/test/put/deque/modulo.h>
#include <libs/assign/v2/test/put/deque/std.h>
#include <libs/assign/v2/test/put/deque.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_deque{

	void test()
    {
		xxx_csv::test();
		xxx_functor::test();
		xxx_modulo::test();
		xxx_std::test();
    }

}// xxx_deque
}// xxx_put
}// test_assign_v2
