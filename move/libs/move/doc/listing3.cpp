//[listing3
//...

struct sink
{
    explicit sink(movable x) : member(boost::move(x)) { }
    
    movable member;
};

int main()
{
    movable x = f(10, 5);
    sink y(x);          // must copy.
    sink z(f(20, 2));   // no copy.

    return 0;
}
//]
