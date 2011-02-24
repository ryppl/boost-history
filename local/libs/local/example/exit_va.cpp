
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Use local scope exit (with const, non-const, and `this` binding).
// Adapted from Boost.ScopeExit documentation.

#include <boost/local/exit.hpp>
#include <boost/foreach.hpp>
#include <boost/typeof/std/vector.hpp> // Typeof emulation mode.
#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP() // Typeof emulation mode.
#include <vector>
#include <ostream>
#include <iostream>
#include <cassert>

class world;

class person {
    friend class world;
public:
    typedef unsigned int id_t;
    typedef unsigned int evolution_t;

    person(): id_(0), evolution_(0) {}

    friend std::ostream& operator<<(std::ostream& o, person const& p)
        { return o << "person: " << p.id_ << ", " << p.evolution_; }

private:
    id_t id_;
    evolution_t evolution_;
};

BOOST_TYPEOF_REGISTER_TYPE(person)

class world {
public:
    typedef unsigned int id_t;

    world(): next_id_(1) {}

    void add_person(person const& a_person);

    friend std::ostream& operator<<(std::ostream& o, world const& w) {
        o << "world: " << w.next_id_ << ", {";
        BOOST_FOREACH(person const& p, w.persons_) {
            o << ' ' << p << ',';
        }
        return o << "}";
    }
private:
    id_t next_id_;
    std::vector<person> persons_;
};

BOOST_TYPEOF_REGISTER_TYPE(world)

void world::add_person(person const& a_person) {
    persons_.push_back(a_person);

    // This block must be no-throw.
    person& p = persons_.back();
    person::evolution_t checkpoint = p.evolution_;

    BOOST_LOCAL_EXIT(const bind checkpoint, const bind& p, bind this) {
        if (checkpoint == p.evolution_) this_->persons_.pop_back();
        std::cout << "1st local exit" << std::endl;
        return; // Exit local scope (not enclosing function).
        assert(false);
    } BOOST_LOCAL_EXIT_END

    // ...

    checkpoint = ++p.evolution_;

    // Assign new id to the person.
    world::id_t const prev_id = p.id_;
    p.id_ = next_id_++;
    BOOST_LOCAL_EXIT(const bind checkpoint, const bind prev_id,
            bind& p, bind& next_id_) {
        if (checkpoint == p.evolution_) {
            next_id_ = p.id_;
            p.id_ = prev_id;
        }
        std::cout << "2nd local exit" << std::endl;
    } BOOST_LOCAL_EXIT_END

    // ...

    checkpoint = ++p.evolution_;
}

int main() {
    person adamo, eva;
    std::cout << adamo << std::endl;
    std::cout << eva << std::endl;

    world w;
    w.add_person(adamo);
    w.add_person(eva);
    std::cout << w << std::endl;
}

