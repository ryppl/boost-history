#include <memory>
#include <string>
#include <functional>
#include <algorithm>
#include <iostream>

//#if 0
#include "map.hpp"

class Person
{
public:
    Person(int ID, std::string const& Name) : ID_(ID), Name_(Name) { }

    int ID() const { return ID_; }
    std::string const& Name() const { return Name_; }
private:
    int ID_;
    std::string Name_;
};

struct PersonKey : boost::CustomKeyOfValue<int, Person>
{
    int operator()(Person const& p) const { return p.ID(); }
};

struct Print : std::unary_function<Person const&, void>
{
    void operator()(Person const& p) const
    { std::cout << "ID: " << p.ID() << "; Name: " << p.Name() << std::endl; }
};

typedef boost::map<int, Person, boost::UniqueKey, PersonKey> tree_type;

typedef boost::map<int, Person, boost::UniqueKey, PersonKey,
    boost::IndexedNode> tree_type2;
//#endif

template <typename T>
struct Key
{
    int operator()(std::string const&) { return 1; }
};

template <typename R>
class F
{
public:
    typedef boost::map<int, R, boost::UniqueKey, Key<R>,
        boost::NonIndexedNode> map_type;
};

int main()
{
    F<std::string> f;
//#if 0
    //-------------------------------------------------------------------------
    tree_type t;

    t.insert(Person(5, "John Doe"));
    std::cout << "size: " << t.size()
              << ", valid: " << (t.valid() ? "true" : "false") << std::endl;
    if (t.find(5) == t.end())
    {
        std::cout << "find() failed!" << std::endl;
    }
    std::for_each(t.begin(), t.end(), Print());
    std::cout << std::endl;

    t.insert(Person(2, "Jane Doe"));
    std::cout << "size: " << t.size()
              << ", valid: " << (t.valid() ? "true" : "false") << std::endl;
    if (t.find(2) == t.end())
    {
        std::cout << "find() failed!" << std::endl;
    }
    std::for_each(t.begin(), t.end(), Print());
    std::cout << std::endl;

    t.insert(Person(8, "Bill Gates"));
    std::cout << "size: " << t.size()
              << ", valid: " << (t.valid() ? "true" : "false") << std::endl;
    if (t.find(8) == t.end())
    {
        std::cout << "find() failed!" << std::endl;
    }
    std::for_each(t.begin(), t.end(), Print());
    std::cout << std::endl;

    t.insert(Person(3, "George Bush"));
    std::cout << "size: " << t.size()
              << ", valid: " << (t.valid() ? "true" : "false") << std::endl;
    if (t.find(3) == t.end())
    {
        std::cout << "find() failed!" << std::endl;
    }
    std::for_each(t.begin(), t.end(), Print());
    std::cout << std::endl;

    t.insert(t.find(3), Person(4, "Babs"));
    std::cout << "size: " << t.size()
              << ", valid: " << (t.valid() ? "true" : "false") << std::endl;
    if (t.find(4) == t.end())
    {
        std::cout << "find() failed!" << std::endl;
    }
    std::for_each(t.begin(), t.end(), Print());
    std::cout << std::endl;
    //-------------------------------------------------------------------------
    tree_type2 t2;

    t2.insert(Person(25, "Doe Joe"));
    std::cout << "size: " << t2.size()
              << ", valid: " << (t2.valid() ? "true" : "false") << std::endl;
    if (t2.find(25) == t2.end())
    {
        std::cout << "find() failed!" << std::endl;
    }
    std::for_each(t2.begin(), t2.end(), Print());
    std::cout << std::endl;

    t2.insert(Person(92, "Jane Wei"));
    std::cout << "size: " << t2.size()
              << ", valid: " << (t2.valid() ? "true" : "false") << std::endl;
    if (t2.find(92) == t2.end())
    {
        std::cout << "find() failed!" << std::endl;
    }
    std::for_each(t2.begin(), t2.end(), Print());
    std::cout << std::endl;

    t2.insert(Person(38, "Billy Bathgate"));
    std::cout << "size: " << t2.size()
              << ", valid: " << (t2.valid() ? "true" : "false") << std::endl;
    if (t2.find(38) == t2.end())
    {
        std::cout << "find() failed!" << std::endl;
    }
    std::for_each(t2.begin(), t2.end(), Print());
    std::cout << std::endl;

    t2.insert(Person(13, "Curious George Bush"));
    std::cout << "size: " << t2.size()
              << ", valid: " << (t2.valid() ? "true" : "false") << std::endl;
    if (t2.find(13) == t2.end())
    {
        std::cout << "find() failed!" << std::endl;
    }
    std::for_each(t2.begin(), t2.end(), Print());
    std::cout << std::endl;

    t2.insert(t2.find(38), Person(40, "Nads"));
    std::cout << "size: " << t2.size()
              << ", valid: " << (t2.valid() ? "true" : "false") << std::endl;
    if (t2.find(40) == t2.end())
    {
        std::cout << "find() failed!" << std::endl;
    }
    std::for_each(t2.begin(), t2.end(), Print());
    std::cout << std::endl;

    tree_type2::iterator i;
    i = t2.at(3);
    if (i == t2.end())
    {
        std::cout << "at() failed!" << std::endl;
    }
    else
    {
        Print()(*i);
    }

    std::cout << "pos(25): " << t2.pos(25) << std::endl;
    std::cout << "pos(13): " << t2.pos(13) << std::endl;
    std::cout << "pos(38): " << t2.pos(38) << std::endl;
    std::cout << "pos(40): " << t2.pos(40) << std::endl;
    std::cout << "pos(92): " << t2.pos(92) << std::endl;
    std::cout << std::endl;

    t2.erase(40);
    std::for_each(t2.begin(), t2.end(), Print());
    std::cout << "size: " << t2.size()
              << ", valid: " << (t2.valid() ? "true" : "false") << std::endl;
    std::cout << std::endl;

    t2.erase(t2.find(38));
    std::for_each(t2.begin(), t2.end(), Print());
    std::cout << "size: " << t2.size()
              << ", valid: " << (t2.valid() ? "true" : "false") << std::endl;
    std::cout << std::endl;
//#endif

#ifdef __WIN32__
    std::string s;
    std::getline(std::cin, s);
#endif
}

