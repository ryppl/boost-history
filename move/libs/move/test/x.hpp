// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef X_DWA2004410_HPP
# define X_DWA2004410_HPP

# include <boost/move/move.hpp>
# include <boost/assert.hpp>
# include "say.hpp"
# include "./generic_tests.hpp"

//
// A sample movable class.
//
class X
{
 public: // "Ordinary" stuff
    X() : resource(++cnt)
    {
        SAY("X() #" << resource);
    }

    X(X const& rhs) : resource(++cnt)
    {
        copy(rhs);
    }

    ~X()
    {
        release();
    }

    // Assign is declared in the move stuff
    
 public: // Move stuff
    // move constructor
    X(boost::move_from<X> rhs)
      : resource(rhs.source.resource)
    {
        BOOST_ASSERT(rhs.source.resource <= cnt); // check for garbage
        SAY("MOVE #" << resource);
        test::moved();
        rhs.source.resource = 0;
        BOOST_ASSERT(resource);
    }

    // move assignment
    X& operator=(X rhs)
    {
        BOOST_ASSERT(rhs.resource <= cnt); // check for garbage
        release();
        resource = rhs.resource;
        SAY("MOVE #" << resource);
        test::moved();
        rhs.resource = 0;
        BOOST_ASSERT(resource);
        return *this;
    }
    
    static int copies;  // count the number of copies
    
    operator boost::move_from<X>() { return *this; }

    int value() const { return resource; }
    
 private: // helper functions
    void release()
    {
        BOOST_ASSERT(resource <= cnt); // check for garbage
        if (!resource)
            SAY("destroy empty");
        else
            SAY("destroy #" << resource);
        resource = 0;
    }

    void copy(X const& rhs)
    {
        BOOST_ASSERT(rhs.resource <= cnt); // check for garbage
        SAY("copy #" << this->resource << " <- #" << rhs.resource);
        test::copied();
        ++copies;
    }
    
 private:   // Data members
    int resource;
    
    static int cnt;  // count the number of resources
};

int X::cnt;
int X::copies;

struct X_source {
    X operator()() { return X(); }
};

#endif // X_DWA2004410_HPP
