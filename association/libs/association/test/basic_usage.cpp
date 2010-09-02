//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2010.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/association for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/association/association.hpp>
#include <iostream>

using namespace boost;

class CBoy;
class CGirl;

struct boy {};
struct girl {};

typedef association::bidir<
    tagged<CBoy,    boy>,
    tagged<CGirl,   girl> 
> friends;
    
class CBoy {
public:
    CBoy() {}
    void GiveGirlfriendFlowers();
    void RecvSlap() {
        std::cout << "boy: ouch!" << '\n';
        m_girlfriend.disconnect();
    }
    friends::end_point<girl>::type m_girlfriend;
};

class CGirl {
public:
    CGirl() {
    }
    void RecvFlowers() {
        std::cout << "girl: thank you for the flowers!" << '\n';
    }
    void SlapBoyfriend() {
        std::cout << "girl: take this!" << '\n';
        if(m_boyfriend.get())
            m_boyfriend->RecvSlap();
    }
    void make_boyfriend() {
        std::cout << "girl: i have a boyfriend!" << '\n';
    }
    void break_boyfriend() {
        std::cout << "girl: we broke up." << '\n';
    }
    friends::end_point<boy>::type m_boyfriend;
};

void CBoy::GiveGirlfriendFlowers() {
    //~ if(m_girlfriend.get())
        //~ m_girlfriend->RecvFlowers();
    
    if (friends::get<girl>(*this)) 
        friends::get<girl>(*this)->RecvFlowers();

}

BOOST_ASSOCIATION_DCL(friends, m_boyfriend, m_girlfriend);


int main()
{
    CBoy Henry;
    CGirl Sally;

    //~ Henry.m_girlfriend.connect(&Sally);
    friends::connect(&Henry,&Sally);   
    friends::disconnect<boy>(Henry);
    friends::connect(&Henry,&Sally);
    friends::disconnect<girl>(Sally);
    friends::connect(&Henry,&Sally);
    Henry.GiveGirlfriendFlowers();
    //~ Sally.SlapBoyfriend();
    friends::get<girl>(Henry)->SlapBoyfriend();

    return 0;
}
