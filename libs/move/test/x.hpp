// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef X_DWA2004410_HPP
# define X_DWA2004410_HPP

# include <boost/move.hpp>
# include <boost/assert.hpp>
# include "say.hpp"

//
// A sample movable class.
//
class X : public boost::movable<X>
{
 public: // "Ordinary" stuff
    X() : resource(++cnt)
    {
        SAY("X() #" << resource);
    }

    // non-const lvalue - copy ctor
    BOOST_LVALUE_COPY_CTOR(
        X, (rhs)(: resource(++cnt)),
    {
        SAY("copy #" << resource << " <- #" << rhs.resource);
    })

    ~X()
    {
        release();
    }

# ifdef BOOST_NO_IMPLICIT_MOVE_ASSIGN_FOR_COPYABLE_TYPES
    X& operator=(X rhs)
    {
        return *this = boost::move_from<X>(rhs);
    }
# else
    BOOST_LVALUE_ASSIGN(
        X, (rhs), 
    {
        release();
        resource = ++cnt;
        SAY("copy #" << resource << " <- #" << rhs.resource);
        return *this;
    })
# endif 
    
 public: // Move stuff
    // non-const rvalue - move ctor
    X(boost::move_from<X> rhs)
      : resource(rhs->resource)
    {
        SAY("MOVE #" << resource);
        rhs->resource = 0;
        BOOST_ASSERT(resource);
    }

    // non-const rvalue - move assignment
    X& operator=(boost::move_from<X> rhs)
    {
        release();
        resource = rhs->resource;
        SAY("MOVE #" << resource);
        rhs->resource = 0;
        BOOST_ASSERT(resource);
        return *this;
    }
    
 private: // helper functions
    void release()
    {
        if (!resource)
            SAY("destroy empty");
        else
            SAY("destroy #" << resource);
        resource = 0;
    }

 private:   // Data members
    int resource;
    
    static int cnt;  // count the number of resources
};

int X::cnt;

#endif // X_DWA2004410_HPP
