
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef DICTINARY_HPP_
#define DICTINARY_HPP_

#include <contract.hpp>
#include <map>

/** Simple dictionary. */
template<typename Key, typename Value>
class dictionary {

    CONTRACT_CLASS( (dictionary)
    (invariant) ({
        CONTRACT_ASSERT( count() >= 0 );
    }) )

public:
    // Creation //

    /** Create empty dictionary. */
    dictionary(void)
    CONTRACT_CONSTRUCTOR(
            (public) (dictionary)( (void) )
    (postcondition) ({
        CONTRACT_ASSERT( 0 == count() );
    })
    (body) ({
    }) )

    /** Destroy dictionary. */
    virtual ~dictionary(void)
    CONTRACT_DESTRUCTOR(
            (public) (dictionary)( (void) )
    (body) ({
    }) )

    // Basic Queries //

    /** Number of key entries. */
    int count(void) const
    CONTRACT_FUNCTION(
            (public) (int) (count)( (void) ) (const)
    (postcondition) (result) ({
        CONTRACT_ASSERT( result >= 0 );
    })
    (body) ({
        return items_.size();
    }) )

    /** If there is an entry for specified key. */
    bool has(const Key& key) const
    CONTRACT_FUNCTION(
            (public) (bool) (has)( (const Key&)(key) ) (const)
    (postcondition) (result) ({
        if (count() == 0) CONTRACT_ASSERT( result == false );
    })
    (body) ({
        return items_.find(key) != items_.end();

    }) )

    /** Value for given key. */
    const Value& value_for(const Key& key) const
    CONTRACT_FUNCTION(
            (public) (const Value&) (value_for)( (const Key&)(key) )
            (const)
    (precondition) ({
        CONTRACT_ASSERT( has(key) );
    })
    (body) ({
        return items_.find(key)->second;
    }) )

    // Commands //

    /** Put value for given key. */
    void put(const Key& key, const Value& value)
    CONTRACT_FUNCTION(
            (public) (void) (put)( (const Key&)(key)
                    (const Value&)(value) ) (copyable)
    (postcondition) ({
        CONTRACT_ASSERT( count() == 
                (CONTRACT_OLDOF(this)->count() + 1) );
        CONTRACT_ASSERT( has(key) );
        CONTRACT_ASSERT( value_for(key) == value );
    })
    (body) ({
        items_.insert(std::pair<Key, Value>(key, value));
    }) )

    /** Remove value for given key. */
    void remove(const Key& key)
    CONTRACT_FUNCTION(
            (public) (void) (remove)( (const Key&)(key) ) (copyable)
    (precondition) ({
        CONTRACT_ASSERT( has(key) );
    })
    (postcondition) ({
        CONTRACT_ASSERT( count() ==
                (CONTRACT_OLDOF(this)->count() - 1) );
        CONTRACT_ASSERT( !has(key) );
    })
    (body) ({
        items_.erase(key);
    }) )

private:
    std::map<Key, Value> items_;
};

#endif // #inlcude guard

