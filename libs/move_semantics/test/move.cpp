//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright David Abrahams, Vicente Botet, Ion Gaztanaga 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/move for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/move_semantics/move.hpp>
#include "../example/movable.hpp"

movable function(movable m)
{
   return movable(boost::move(m));
}

movable functionr(BOOST_RV_REF(movable) m)
{
   return movable(boost::move(m));
}

movable function2(movable m)
{
   return boost::move(m);
}

BOOST_RV_REF(movable) function2r(BOOST_RV_REF(movable) m)
{
   return boost::move(m);
}

movable move_return_function2 ()
{
    return movable();
}

movable move_return_function ()
{
    movable m;
    return (boost::move(m));
}
int main()
{
   {
      movable m;
      movable m2(boost::move(m));
      movable m3(function(movable(boost::move(m2))));
      movable m4(function(boost::move(m3)));
	}
   {
      movable m;
      movable m2(boost::move(m));
      movable m3(functionr(movable(boost::move(m2))));
      movable m4(functionr(boost::move(m3))); 
	}
   {
      movable m;
      movable m2(boost::move(m));
      movable m3(function2(movable(boost::move(m2))));
      movable m4(function2(boost::move(m3)));
	}
   {
      movable m;
      movable m2(boost::move(m));
      movable m3(function2r(movable(boost::move(m2))));
      movable m4(function2r(boost::move(m3)));
	}
   {
      movable m;
      movable m2(boost::move(m));
      movable m3(move_return_function());
	}
   {
      movable m;
      movable m2(boost::move(m));
      movable m3(move_return_function2());
	}
   return 0;
}
