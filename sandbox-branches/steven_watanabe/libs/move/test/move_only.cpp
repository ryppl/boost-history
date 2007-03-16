// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "say.hpp"
#include "move_only.hpp"
#include <boost/move.hpp>

move_only
test_returnable1()
{
    SAY("returning move_only lvalue");

    move_only x;

    return boost::move(x);
}

move_only
test_returnable2()
{
    SAY("returning move_only rvalue");
    return move_only();
}

int main()
{
    SAY("--- implicitly movable noncopyable type tests ---");
    SAY("--- direct initialize from function result ---");
    move_only quack(test_returnable1());
    
    SAY("--- copy initialize from function result ---");
    move_only squirm = test_returnable2();

    SAY("--- assign ---");
    squirm = test_returnable2();
    
    return 0;
}
