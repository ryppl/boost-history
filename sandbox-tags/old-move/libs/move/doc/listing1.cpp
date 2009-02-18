//[movable_example
#include <iostream>
#include <algorithm>

#include <boost/operators.hpp>

#include <boost/move/move.hpp>

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

void movable_example()
{
    movable x(10);
    movable y = x;
}
//]

//[move_only_example
class move_only : public boost::equality_comparable<move_only>
{
    move_only(move_only&);

 public:

    explicit move_only(int x = 0) : member(new implementation(x)) { }
    ~move_only() { delete member; }

    // Move Constructor
    move_only(boost::move_from<move_only> rhs) : member(rhs.source.member)
    { rhs.source.member = 0; }
 
    // operator=() on a move_only type takes parameter by value and consumes it
    // note that this type can't be copied, so operator= can only be called
    // for a moved value.
    move_only& operator=(move_only x)
    { swap(*this, x); return *this; }

    operator boost::move_from<move_only>()
    { return boost::move_from<move_only>(*this); }
    
    friend bool operator==(const move_only& x, const move_only &y)
    { return *x.member == *y.member; }
    
    friend void swap(move_only& x, move_only& y)
    { swap(x.member, y.member); }
    
 private:
    implementation* member;
};

void move_only_example()
{
    move_only x(20);
    move_only y = boost::move(x);
}
//]

//[return_example
//...
move_only f(int x, int y)
{ return move_only(x * y); }

void return_example()
{
    move_only x = f(10, 5);
    move_only y;
    y = f(4,3);
}
//]

//[sink_example
//...
movable g(int x, int y)
{ return movable(x * y); }

struct sink
{
    explicit sink(movable x) : member(boost::move(x)) { }
    
    movable member;
};

void example4()
{
    movable x = g(10, 5);
    sink y(x);          // must copy.
    sink z(g(20, 2));   // no copy.
}
//]

int main() {
    std::cout<<"Movable Example:\n\n";
    movable_example();
    std::cout<<"\nMove Only Example:\n\n";
    move_only_example();
    std::cout<<"\nReturn Example:\n\n";
    return_example();
    std::cout<<"\nSink Example:\n\n";
    sink_example();
}
