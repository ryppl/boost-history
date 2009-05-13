// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//!!!These are my local testfiles - hardly any comments, "#pragma once" in headers and #include "" etc.!!!

#include "vector.hpp"
#include "convenience.hpp"
#include "concepts.hpp"

#include <cassert>

class copyable
{
public:
    copyable()
    {
    }
    copyable(const copyable&)
    {
    }
    copyable& operator=(const copyable&)
    {
        return *this;
    }
};

class moveable
{
private:
    moveable(const moveable&);

public:
    moveable()
    {
    }
    moveable(moveable&&)
    {
    }
    moveable& operator=(moveable&&)
    {
        return *this;
    }
};

int main()
{
    namespace gsoc=boost::fusion::gsoc;

    {
        typedef gsoc::vector<> at;

        {
            using namespace gsoc::result_of;
            static_assert(equal_to<advance_c<begin<at>::type,0>::type,end<at>::type>::type::value,"");
            static_assert(!distance<begin<at>::type,end<at>::type>::type::value,"");
            static_assert(!size<at>::type::value,"");
            static_assert(empty<at>::type::value,"");
        }

        {
            at a;

            using namespace gsoc;
            assert(equal_to(advance_c<0>(begin(a)),end(a)));
            assert(!distance(begin(a),end(a)));
            assert(!size(a));
            assert(empty(a));
        }
    }

    {
        typedef gsoc::vector<long long,float,char> bt;

        {
            using namespace gsoc::result_of;
            static_assert(equal_to<advance_c<begin<bt>::type,3>::type,end<bt>::type>::type::value,"");
            static_assert(distance<begin<bt>::type,end<bt>::type>::type::value==3,"");
            static_assert(distance<next<begin<bt>::type>::type,prior<end<bt>::type>::type>::type::value==1,"");
            static_assert(size<bt>::type::value==3,"");
            static_assert(!empty<bt>::type::value,"");

            static_assert(std::is_same<value_of<begin<bt>::type>::type,long long>::value,"");
        }

        {
            bt b(0xDEADBEEF,1337.0f,42);

            using namespace gsoc;
            assert(equal_to(advance_c<3>(begin(b)),end(b)));
            assert(distance(begin(b),end(b))==3);
            assert(distance(next(begin(b)),prior(end(b)))==1);
            assert(size(b)==3);
            assert(!empty(b));

            assert(deref(begin(b))==0xDEADBEEF);
            at_c<0>(b)=0xBEEFDEEF;
            assert(at_c<0>(b)==0xBEEFDEEF);
        }

        {
            bt b(0xDEADBEEF,1337.0f,42);

            using namespace gsoc;
            assert(deref(begin(b))==0xDEADBEEF);
            at_c<0>(b)=0xBEEFDEEF;
            assert(at_c<0>(b)==0xBEEFDEEF);
        }
    }

    {
        using namespace gsoc;
        //equal_to(begin(vector<long long>()),begin(vector<int>()));
        assert(equal_to(begin(vector<long long>()),begin(vector<long long>()))==true);
    }

    {
        using namespace gsoc::result_of;

        typedef const gsoc::vector<float> f;
        static_assert(std::is_same<deref<begin<f>::type>::type,const float&>::value,"");
        static_assert(std::is_same<deref<advance_c<begin<f>::type,0>::type>::type,const float&>::value,"");

    }

    {
        gsoc::vector<moveable,moveable> c;
        gsoc::vector<moveable,moveable> d;
        c=std::move(d);
    }

    {
        gsoc::vector<copyable,copyable> c;
        gsoc::vector<copyable,copyable> d;
        c=d;
        c=std::move(d);
    }

    {
        gsoc::vector<moveable,copyable> c;
        gsoc::vector<moveable,copyable> d;
        c=std::move(d);
    }

    return 0;
}
