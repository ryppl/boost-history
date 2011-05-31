
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef STACK_HPP_
#define STACK_HPP_

#include <contract.hpp>
#include <vector>

/** Simple stack. */
template<typename T>
class stack {

    CONTRACT_CLASS( (stack)
    (invariant) ({
        CONTRACT_ASSERT( count() >= 0 );
    }) )

public:
    // Creation //

    /** Create empty stack. */
    stack(void): items_()
    CONTRACT_CONSTRUCTOR(
            (public) (stack)( (void) )
    (postcondition) ({
        CONTRACT_ASSERT( count() == 0 );
    })
    (body) ({
    }) )

    /** Destroy stack. */
    ~stack(void)
    CONTRACT_DESTRUCTOR(
            (public) (stack)( (void) )
    (body) ({
    }) )

    // Basic Queries //

    /** Number of items. */
    int count(void) const
    CONTRACT_FUNCTION(
            (public) (int) (count)( (void) ) (const)
    (body) ({
        return items_.size();
    }) )

    /** Item at index in [1, count()] (Eiffel index starts at 1). */
    const T& item_at(const int& index) const
    CONTRACT_FUNCTION(
            (public) (const T&) (item_at)( (const int&)(index) )
            (const)
    (precondition) ({
        CONTRACT_ASSERT( index >= 1 );
        CONTRACT_ASSERT( index <= count() );
    })
    (body) ({
        return items_.at(index - 1);
    }) )

    // Derived Queries //

    /** If no items. */
    bool is_empty(void) const
    CONTRACT_FUNCTION(
            (public) (bool) (is_empty)( (void) ) (const)
    (postcondition) (result) ({
        CONTRACT_ASSERT( result == (count() == 0) );
    })
    (body) ({
        return items_.size() == 0;
    }) )

    /** Top item. */
    const T& item(void) const
    CONTRACT_FUNCTION(
            (public) (const T&) (item)( (void) ) (const)
    (precondition) ({
        CONTRACT_ASSERT( count() > 0 );
    })
    (postcondition) (result) ({
        CONTRACT_ASSERT( result == item_at(count()) );
    })
    (body) ({
        return items_.at(items_.size() - 1);
    }) )

    // Commands //

    /** Push new item to the top. */
    void put(const T& new_item)
    CONTRACT_FUNCTION(
            (public) (void) (put)( (const T&)(new_item) ) (copyable)
    (postcondition) ({
        CONTRACT_ASSERT( count() ==
                (CONTRACT_OLDOF(this)->count() + 1) );
        CONTRACT_ASSERT( item() == new_item );
    })
    (body) ({
        items_.push_back(new_item);
    }) )

    /** Pop top item. */
    void remove(void)
    CONTRACT_FUNCTION(
            (public) (void) (remove)( (void) ) (copyable)
    (precondition) ({
        CONTRACT_ASSERT( count() > 0 );
    })
    (postcondition) ({
        CONTRACT_ASSERT( count() ==
                (CONTRACT_OLDOF(this)->count() - 1) );
    })
    (body) ({
        items_.resize(items_.size() - 1);
    }) )

private:
    std::vector<T> items_;
};

#endif // #include guard

