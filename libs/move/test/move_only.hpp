// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef MOVE_ONLY_DWA2004410_HPP
# define MOVE_ONLY_DWA2004410_HPP

# include <boost/move.hpp>
# include <boost/noncopyable.hpp>
# include "say.hpp"

//
// A class that can be moved, but not copied.
//
struct move_only : boost::movable<move_only>
{
    move_only() {}
    
    move_only(boost::move_from<move_only>)
    {
        SAY("move construct move_only");
    }
    
    move_only& operator=(boost::move_from<move_only>)
    {
        SAY("move assign move_only");
        return *this;
    }
};


#endif // MOVE_ONLY_DWA2004410_HPP
