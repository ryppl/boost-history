#pragma warning(disable:4675)

#include <boost/s11n/binary_iarchive.hpp>
#include <boost/s11n/binary_oarchive.hpp>
#include <boost/s11n/ptree_iarchive.hpp>
#include <boost/s11n/ptree_oarchive.hpp>
#include <boost/s11n/nvp.hpp>
#include <boost/s11n/vector.hpp>
#include <boost/s11n/map.hpp>
#include <boost/s11n/string.hpp>
#include <boost/s11n/shared_ptr.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>

namespace std
{

    template<class T>
    std::ostream &operator <<(std::ostream &stream, const std::vector<T> &v)
    {
        for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
        {
            if (it != v.begin()) 
                stream << ' ';
            stream << *it;
        }
        return stream;
    }

    template<class T>
    std::istream &operator >>(std::istream &stream, std::vector<T> &v)
    {
        v.clear();
        while (1)
        {
            T t;
            ws(stream);
            if (stream.eof())
                break;
            stream >> t;
            if (stream.fail() || stream.bad())
                break;
            else
                v.push_back(t);
        }
        return stream;
    }

}

struct test
{
    char a;
    unsigned int b;
    std::vector<int> c;
    bool d;
    long e;
    std::map<int, double> f;
private:    
    friend class boost::s11n::access;
    template<class Archive> void serialize(Archive &ar)
    {
        ar & boost::s11n::make_nvp("a", a);
        ar & boost::s11n::make_nvp("b", b);
        ar & boost::s11n::make_concise_nvp("c", c);
        ar & boost::s11n::make_nvp("d", d);
        ar & boost::s11n::make_nvp("e", e);
        ar & boost::s11n::make_nvp("f", f);
    }
};

struct test2
{
    int a;
    unsigned int b;
    std::vector<int> c;
    bool d;
    boost::int64_t e;
};

template<class Archive> void serialize(Archive &ar, test2 &t)
{
    ar & boost::s11n::make_nvp("a.q.w", t.a);
    ar & boost::s11n::make_nvp("b", t.b);
    ar & boost::s11n::make_nvp("c", t.c);
    ar & boost::s11n::make_nvp("d", t.d);
    ar & boost::s11n::make_nvp("e", t.e);
}

struct Base
{
    int b;
    Base(): b(std::rand() % 100) { }
    virtual ~Base() { }
private:
    friend class boost::s11n::access;
    template<class Archive> void serialize(Archive &ar)
    {
        ar & boost::s11n::make_nvp("b", b);
    }
};

struct Base1: virtual public Base
{
    int b1;
    Base1(): b1(std::rand() % 100) { }
    virtual ~Base1() { }
private:
    friend class boost::s11n::access;
    template<class Archive> void serialize(Archive &ar)
    {
        ar & boost::s11n::make_nvp("Base", static_cast<Base &>(*this));
        ar & boost::s11n::make_nvp("b1", b1);
    }
};

struct Base2: virtual public Base
{
    int b2;
    Base2(): b2(std::rand() % 100) { }
    virtual ~Base2() { }
private:
    friend class boost::s11n::access;
    template<class Archive> void serialize(Archive &ar)
    {
        ar & boost::s11n::make_nvp("Base", static_cast<Base &>(*this));
        ar & boost::s11n::make_nvp("b2", b2);
    }
};

struct Derived: public Base1, public Base2
{
    int d;
    Derived(): d(std::rand() % 100) { }
private:    
    friend class boost::s11n::access;
    template<class Archive> void serialize(Archive &ar)
    {
        ar & boost::s11n::make_nvp("Base1", static_cast<Base1 &>(*this));
        ar & boost::s11n::make_nvp("Base2", static_cast<Base2 &>(*this));
        ar & boost::s11n::make_nvp("d", d);
    }
};

template<class Archive> void save(Archive &oa)
{
    int i = 3;
    oa & boost::s11n::make_nvp("i", i);

    std::vector<int> v;
    std::vector<int> *vptr = &v;
    v.push_back(7);
    v.push_back(8);
    v.push_back(9);
    oa & boost::s11n::make_nvp("v", v);
    oa & boost::s11n::make_nvp("vptr", vptr);
    
    test t;
    t.a = 15;
    t.b = 16;
    t.c = v;
    t.d = false;
    t.e = 17;
    t.f[3] = 7.56;
    t.f[7] = 1.67;
    test2 t2;
    t2.a = 115;
    t2.b = 116;
    t2.c = v;
    t2.d = false;
    t2.e = 117;
    oa & boost::s11n::make_nvp("t", t);
    oa & boost::s11n::make_nvp("t2", t2);

    Derived *ptrd = new Derived;
    Base *ptr = ptrd;
    oa & boost::s11n::make_nvp("ptr", ptr);

    boost::shared_ptr<Base> shptr(ptr);
    oa & boost::s11n::make_nvp("shptr", shptr);
    
    test t3;
    t3.a = 15;
    t3.b = 16;
    t3.d = false;
    t3.e = 17;
    oa & boost::s11n::make_nvp("t3", t3);

}

template<class Archive> void load(Archive &ia)
{
    int i;
    ia & boost::s11n::make_nvp("i", i);

    std::vector<int> v;
    std::vector<int> *vptr;
    ia & boost::s11n::make_nvp("v", v);
    ia & boost::s11n::make_nvp("vptr", vptr);

    test t;
    test2 t2;
    ia & boost::s11n::make_nvp("t", t);
    ia & boost::s11n::make_nvp("t2", t2);

    Base *ptr;
    ia & boost::s11n::make_nvp("ptr", ptr);
    Derived *ptrd = dynamic_cast<Derived *>(ptr);

    boost::shared_ptr<Base> shptr;
    ia & boost::s11n::make_nvp("shptr", shptr);

    test t3;
    ia & boost::s11n::make_nvp("t3", t3);
}

int main()
{

    boost::s11n::binary_oarchive::register_class<Base, Derived>("Derived");
    boost::s11n::binary_iarchive::register_class<Base, Derived>("Derived");
    boost::s11n::ptree_oarchive::register_class<Base, Derived>("Derived");
    boost::s11n::ptree_iarchive::register_class<Base, Derived>("Derived");

    {
        typedef std::map<std::string, boost::s11n::binary_iarchive::registry_entry<Base> > registry;
        registry &r = boost::s11n::binary_iarchive::registry<Base>();
        for (registry::const_iterator it = r.begin(); it != r.end(); ++it)
        {
            std::cout << it->first << " " << it->second.load_func << "\n";
        }
    }

    std::string str;
    boost::property_tree::iptree pt;

    {
        std::ostringstream stream;
        boost::s11n::ptree_oarchive oa1(pt);
        boost::s11n::binary_oarchive oa2(stream);
        save(oa1);
        save(oa2);
        str = stream.str();
    }

    write_info(std::cout, pt);
    std::cout << "\n" << str;

    {
        std::istringstream stream(str);
        stream.unsetf(std::ios_base::skipws);
        boost::s11n::ptree_iarchive ia1(pt);
        boost::s11n::binary_iarchive ia2(stream);
        load(ia1);
        load(ia2);
    }

}
