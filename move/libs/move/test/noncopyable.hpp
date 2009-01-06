// Copyright David Abrahams 2004. Distributed under the Boost
// Copyright Daniel James 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef Y_DWA2004410_HPP
# define Y_DWA2004410_HPP

# include <boost/move/move.hpp>
# include <boost/assert.hpp>
# include "say.hpp"
# include "./generic_tests.hpp"

//
// A sample movable class.
//
class Y
{
    Y(Y& rhs);
 public: // "Ordinary" stuff
    Y() : resource(++cnt)
    {
        SAY("Y() #" << resource);
    }

    ~Y()
    {
        release();
    }

 public: // Move stuff
    // move constructor
    Y(boost::move_from<Y> rhs)
      : resource(rhs.source.resource)
    {
        BOOST_ASSERT(rhs.source.resource <= cnt); // check for garbage
        SAY("MOVE #" << resource);
        test::moved();
        rhs.source.resource = 0;
        BOOST_ASSERT(resource);
    }

    // move assignment
    Y& operator=(Y rhs)
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

    operator boost::move_from<Y>()
    {
        return boost::move_from<Y>(*this);
    }

    int value() const { return resource; }
 
    static int copies;  // count the number of copies
 
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

    void copy(Y const& rhs)
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

int Y::cnt;
int Y::copies;

#endif // Y_DWA2004410_HPP
