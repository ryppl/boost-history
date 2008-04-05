//[listing1
#include <iostream>
#include <algorithm>

#include <boost/operators.hpp>

#include <boost/move.hpp>

using std::swap;

struct implementation : boost::equality_comparable<implementation>
{
    explicit implementation(int x = 0) : member(x) { }
    
    implementation(const implementation& x) : member(x.member)
    { std::cout << "copy remote part: " << member << std::endl; }
    
    implementation& operator=(const implementation& x)
    {
        member = x.member;
        std::cout << "assign remote part: " << member << std::endl;
        return *this;
    }
    
    friend bool operator==(const implementation& x, const implementation& y)
    { return x.member == y.member; }
    
    int member;
};

class movable : public boost::equality_comparable<movable>
{
 public:
// model concept Regular

    explicit movable(int x = 0) : member(new implementation(x)) { }
    ~movable() { delete member; }
    movable(const movable& x) : member(new implementation(*x.member)) { }
    // operator=() implemented below
    
    friend bool operator==(const movable& x, const movable &y)
    { return *x.member == *y.member; }
    
    friend void swap(movable& x, movable& y)
    { swap(x.member, y.member); }
    
// model concept Movable
    
    // move-ctor assumes ownership of remote part
    movable(boost::move_from<movable> x) : member(x.source.member)
    { x.source.member = 0; }
    
    // operator=() on a movable type takes parameter by value and consumes it
    movable& operator=(movable x)
    { swap(*this, x); return *this; }
    
 private:
    implementation* member;
};

int main()
{
    movable x(10);
    movable y = x;

    return 0;
}
//]
