//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** @file    _order.hpp
 * 
 * Some iterative algorithm versions that are identical for all *order cursors 
 * (as we are just calling the appropriate traversal function that are 
 * defined in the respective *order.hpp files).
 */

// NO guards, as this is context-included!

template <class Cursor, class Op>
Op for_each(root_tracking_cursor<Cursor> s, Op f)
{
    root_tracking_cursor<Cursor> t = last(s);
    s = first(s);
    while (s!=t) {
        f(*s);
        forward(s);
    }
    return f;
}


template <class Cursor, class Op>
Op for_each(Cursor s, Op f)
{
    return for_each(root_tracking_cursor<Cursor>(s), f);
}